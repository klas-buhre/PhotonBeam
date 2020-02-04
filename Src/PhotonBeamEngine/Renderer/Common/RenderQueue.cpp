#include "RenderQueue.hpp"

#include <assert.h>
#include <algorithm>

#include "SceneObject.hpp"
#include "TextComponent.hpp"

using namespace Pht;

namespace {
    const Vec4 modelSpaceOrigin {0.0f, 0.0f, 0.0f, 1.0f};

    int CalcNumSceneObjects(const SceneObject& sceneObject) {
        auto numObjects = 1;
        
        for (auto& child: sceneObject.GetChildren()) {
            numObjects += CalcNumSceneObjects(*child);
        }
        
        return numObjects;
    }
    
    struct CompareEntriesBySortKeyAndFrontToBack {
        inline bool operator() (const RenderQueue::Entry& a, const RenderQueue::Entry& b) {
            if (a.mSortKey == b.mSortKey) {
                return a.mDistance < b.mDistance;
            }
    
            return a.mSortKey > b.mSortKey;
        }
    };

    struct CompareEntriesFrontToBack {
        inline bool operator() (const RenderQueue::Entry& a, const RenderQueue::Entry& b) {
            if (a.mDistance == b.mDistance) {
                return a.mSortKey > b.mSortKey;
            }
    
            return a.mDistance < b.mDistance;
        }
    };
    
    struct CompareEntriesBackToFront {
        inline bool operator() (const RenderQueue::Entry& a, const RenderQueue::Entry& b) {
            if (a.mDistance == b.mDistance) {
                return a.mSortKey > b.mSortKey;
            }
            
            return a.mDistance > b.mDistance;
        }
    };
    
    RenderQueue::TextKind ToTextShaderKind(const TextProperties& textProperties) {
        if (textProperties.mTopGradientColorSubtraction.HasValue() &&
            textProperties.mMidGradientColorSubtraction.HasValue()) {
            
            return RenderQueue::TextKind::DoubleGradientShader;
        }

        if (textProperties.mTopGradientColorSubtraction.HasValue()) {
            return RenderQueue::TextKind::TopGradientShader;
        }

        if (textProperties.mMidGradientColorSubtraction.HasValue()) {
            return RenderQueue::TextKind::MidGradientShader;
        }

        return RenderQueue::TextKind::PlainShader;
    }
}

void RenderQueue::Init(const SceneObject& rootSceneObject) {
    mRootSceneObject = &rootSceneObject;
    auto numSceneObjects = CalcNumSceneObjects(rootSceneObject);
    mQueue.resize(numSceneObjects);
    mMaxSize = numSceneObjects;
}

void RenderQueue::Build(const Mat4& viewMatrix,
                        RenderOrder renderOrder,
                        DistanceFunction distanceFunction,
                        int layerMask) {
    mFirstPartitionSize = 0;
    mSecondPartitionSize = 0;
    mRenderOrder = renderOrder;
    mLayerMask = layerMask;

    assert(mRootSceneObject);
    ScanSubtree(*mRootSceneObject, false);
    CalculateDistances(viewMatrix, distanceFunction);
    Sort();
    BeginIteration();
}

void RenderQueue::Sort() {
    switch (mRenderOrder) {
        case RenderOrder::StateOptimized:
            // Sort depth-writing entries based on sort key and compare distances if keys are equal.
            SortFirstPartition<CompareEntriesBySortKeyAndFrontToBack>();
            // Sort non-depth-writing entries back to front.
            SortSecondPartition<CompareEntriesBackToFront>();
            break;
        case RenderOrder::PiexelOptimized:
            // Sort depth-writing entries front to back and check sort keys if distances are equal.
            SortFirstPartition<CompareEntriesFrontToBack>();
            // Sort non-depth-writing entries back to front.
            SortSecondPartition<CompareEntriesBackToFront>();
            break;
        case RenderOrder::BackToFront:
            // Sort all entries back to front.
            SortFirstPartition<CompareEntriesBackToFront>();
            break;
    }
}

template<typename Comparator>
void RenderQueue::SortFirstPartition() {
    std::sort(&mQueue[0], &mQueue[mFirstPartitionSize], Comparator{});
}

template<typename Comparator>
void RenderQueue::SortSecondPartition() {
    std::sort(&mQueue[mMaxSize - mSecondPartitionSize], &mQueue[mMaxSize], Comparator{});
}

void RenderQueue::ScanSubtree(const SceneObject& sceneObject, bool ancestorMatchedLayerMask) {
    if (!sceneObject.IsVisible()) {
        return;
    }
    
    auto thisObjectOrAncestorMatchedLayerMask = ancestorMatchedLayerMask;
    auto sceneObjectLayerMask = sceneObject.GetLayerMask();
    if (!ancestorMatchedLayerMask) {
        if (sceneObjectLayerMask & mLayerMask) {
            thisObjectOrAncestorMatchedLayerMask = true;
        }
    }
    
    if (sceneObjectLayerMask && !thisObjectOrAncestorMatchedLayerMask) {
        return;
    }
    
    if (thisObjectOrAncestorMatchedLayerMask) {
        ScanSceneObject(sceneObject);
    }
    
    for (auto& child: sceneObject.GetChildren()) {
        ScanSubtree(*child, thisObjectOrAncestorMatchedLayerMask);
    }
}

void RenderQueue::ScanSceneObject(const SceneObject& sceneObject) {
    if (auto* renderable = sceneObject.GetRenderable()) {
        auto& material = renderable->GetMaterial();
        auto isDepthWriting = material.GetDepthState().mDepthWrite;
        
        uint64_t sortKey {0};
        sortKey |= static_cast<uint64_t>(material.GetShaderId()) << shaderIdShift;
        sortKey |= uint64_t{material.GetId()} << materialIdShift;
        sortKey |= uint64_t{renderable->GetGpuVertexBuffer().GetId()} << vertexBufferIdShift;
        
        AddEntry(sortKey, isDepthWriting, sceneObject);
    }
    
    if (auto* textComponent = sceneObject.GetComponent<TextComponent>()) {
        auto isDepthWriting = false;
        auto& textProperties = textComponent->GetProperties();
        if (textProperties.mSecondShadow == TextShadow::Yes) {
            auto sortKey = static_cast<uint64_t>(TextKind::SecondShadow) << textKindShift;
            AddEntry(sortKey, isDepthWriting, sceneObject);
        }
        
        if (textProperties.mShadow == TextShadow::Yes) {
            auto sortKey = static_cast<uint64_t>(TextKind::Shadow) << textKindShift;
            AddEntry(sortKey, isDepthWriting, sceneObject);
        }
        
        if (textProperties.mSpecular == TextSpecular::Yes) {
            auto sortKey = static_cast<uint64_t>(TextKind::Specular) << textKindShift;
            AddEntry(sortKey, isDepthWriting, sceneObject);
        }

        auto sortKey = static_cast<uint64_t>(ToTextShaderKind(textProperties)) << textKindShift;
        AddEntry(sortKey, isDepthWriting, sceneObject);
    }
}

void RenderQueue::AddEntry(uint64_t sortKey, bool isDepthWriting, const SceneObject& sceneObject) {
    if (isDepthWriting) {
        sortKey |= depthWriteShiftedMask;
    }

    Entry entry {.mSortKey = sortKey, .mDistance = 0.0f, .mSceneObject = &sceneObject};

    assert(mFirstPartitionSize + mSecondPartitionSize < mMaxSize);

    switch (mRenderOrder) {
        case RenderOrder::StateOptimized:
        case RenderOrder::PiexelOptimized:
            if (isDepthWriting) {
                mQueue[mFirstPartitionSize] = entry;
                ++mFirstPartitionSize;
            } else {
                mQueue[mMaxSize - mSecondPartitionSize - 1] = entry;
                ++mSecondPartitionSize;
            }
            break;
        case RenderOrder::BackToFront:
            mQueue[mFirstPartitionSize] = entry;
            ++mFirstPartitionSize;
            break;
    }
}

void RenderQueue::CalculateDistances(const Mat4& viewMatrix, DistanceFunction distanceFunction) {
    // Since the matrix is row-major it has to be transposed in order to multiply with the vector.
    auto transposedViewMatrix = viewMatrix.Transposed();
    
    for (BeginIteration(); HasMoreEntries();) {
        auto& renderEntry = GetNextEntry();
        auto sceneObjectPos = renderEntry.mSceneObject->GetWorldSpacePosition();
        
        switch (distanceFunction) {
            case DistanceFunction::CameraSpaceZ: {
                auto sceneObjectPosCamSpace = transposedViewMatrix * Vec4{sceneObjectPos, 1.0f};
                renderEntry.mDistance = -sceneObjectPosCamSpace.z;
                break;
            }
            case DistanceFunction::WorldSpaceZ:
                renderEntry.mDistance = sceneObjectPos.z;
                break;
            case DistanceFunction::WorldSpaceNegativeZ: {
                renderEntry.mDistance = -sceneObjectPos.z;
                break;
            }
        }
    }
}

void RenderQueue::BeginIteration() {
    if (mFirstPartitionSize > 0) {
        mIteratorIndex = 0;
        mCurrentPartitionEndIndex = mFirstPartitionSize;
    } else {
        mIteratorIndex = mMaxSize - mSecondPartitionSize;
        mCurrentPartitionEndIndex = mMaxSize;
    }
}

RenderQueue::Entry& RenderQueue::GetNextEntry() {
    assert(HasMoreEntries());
    auto& entry = mQueue[mIteratorIndex];
    
    ++mIteratorIndex;
    if (mIteratorIndex >= mCurrentPartitionEndIndex &&
        mCurrentPartitionEndIndex == mFirstPartitionSize) {
        
        mIteratorIndex = mMaxSize - mSecondPartitionSize;
        mCurrentPartitionEndIndex = mMaxSize;
    }
    
    return entry;
}
