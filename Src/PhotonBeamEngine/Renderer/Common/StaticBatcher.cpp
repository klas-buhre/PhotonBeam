#include "StaticBatcher.hpp"

#include <assert.h>

#include "SceneObject.hpp"
#include "RenderableObject.hpp"
#include "TextComponent.hpp"
#include "VertexBufferCache.hpp"

using namespace Pht;

namespace {
    constexpr auto maxNumVertices = 64000;
    constexpr auto maxNumIndices = 48000;
    const static Vec3 defaultRotation {0.0f, 0.0f, 0.0f};
    
    struct StaticBatchSetup {
        std::unique_ptr<VertexBuffer> mBatchVertexBuffer;
        const Material* mBatchMaterial {nullptr};
    };
    
    StaticBatchSetup SetUpStaticBatchIfPossible(const SceneObject& sourceSceneObject) {
        if (sourceSceneObject.GetRenderable() || sourceSceneObject.GetComponent<TextComponent>()) {
            return {nullptr, nullptr};
        }
        
        auto numBatchableSceneObjects = 0;
        auto totalNumVertices = 0;
        auto totalNumIndices = 0;
        RenderableObject* previousRenderable {nullptr};
        
        for (auto* sceneObject: sourceSceneObject.GetChildren()) {
            if (sceneObject->GetComponent<TextComponent>() || !sceneObject->GetChildren().empty()) {
                return {nullptr, nullptr};
            }
            
            if (!sceneObject->IsVisible()) {
                continue;
            }
            
            auto* renderable = sceneObject->GetRenderable();
            if (renderable == nullptr) {
                continue;
            }
            
            auto& material = renderable->GetMaterial();
            if (material.GetOpacity() != 1.0f) {
                // Transparent objects need depth sorting based on camera position but the camera is
                // unknown during static batching.
                return {nullptr, nullptr};
            }
            
            auto& gpuVertexBuffer = renderable->GetGpuVertexBuffer();
            auto* cpuSideBuffer = gpuVertexBuffer.GetCpuSideBuffer();
            if (cpuSideBuffer == nullptr) {
                return {nullptr, nullptr};
            }
            
            if (previousRenderable) {
                auto& previousMaterial = previousRenderable->GetMaterial();
                if (material.GetId() != previousMaterial.GetId() ||
                    material.GetShaderId() != previousMaterial.GetShaderId()) {
                    
                    return {nullptr, nullptr};
                }
                
                auto& previousGpuVertexBuffer = previousRenderable->GetGpuVertexBuffer();
                if (cpuSideBuffer->GetAttributeFlags() !=
                    previousGpuVertexBuffer.GetCpuSideBuffer()->GetAttributeFlags()) {
                    
                    return {nullptr, nullptr};
                }
            }
            
            totalNumVertices += cpuSideBuffer->GetNumVertices();
            totalNumIndices += cpuSideBuffer->GetIndexBufferSize();
            ++numBatchableSceneObjects;
            previousRenderable = renderable;
        }
        
        if (totalNumVertices > maxNumVertices || totalNumIndices > maxNumIndices ||
            numBatchableSceneObjects <= 1) {

            return {nullptr, nullptr};
        }

        if (totalNumVertices > 0 && totalNumIndices > 0 && previousRenderable) {
            auto& batchAttributeFlags =
                previousRenderable->GetGpuVertexBuffer().GetCpuSideBuffer()->GetAttributeFlags();
            auto* batchMaterial = &previousRenderable->GetMaterial();
            return {
                std::make_unique<VertexBuffer>(totalNumVertices,
                                               totalNumIndices,
                                               batchAttributeFlags),
                batchMaterial
            };
        }
        
        return {nullptr, nullptr};
    }

    std::unique_ptr<RenderableObject>
    CreateStaticBatchRenderable(VertexBuffer& batchVertexBuffer,
                                const SceneObject& sourceSceneObject,
                                const Material& batchMaterial,
                                const Optional<std::string>& batchVertexBufferName) {
        for (auto* sceneObject: sourceSceneObject.GetChildren()) {
            if (!sceneObject->IsVisible()) {
                continue;
            }
            
            auto* renderable = sceneObject->GetRenderable();
            if (renderable == nullptr) {
                continue;
            }
            
            auto* sceneObjectVertexBuffer = renderable->GetGpuVertexBuffer().GetCpuSideBuffer();
            auto& sceneObjectTransform = sceneObject->GetTransform();
            if (sceneObjectTransform.GetRotation() == defaultRotation) {
                batchVertexBuffer.TransformAndAppendVertices(*sceneObjectVertexBuffer,
                                                             sceneObjectTransform.GetPosition(),
                                                             sceneObjectTransform.GetScale());
            } else {
                // Since the matrix is row-major it has to be transposed in order to multiply with
                // the vectors.
                auto untransposedMatrix = sceneObjectTransform.ToMatrix();
                auto localTransformMatrix = untransposedMatrix.Transposed();
                auto normalMatrix = untransposedMatrix.ToMat3().Transposed();
                batchVertexBuffer.TransformWithRotationAndAppendVertices(*sceneObjectVertexBuffer,
                                                                         localTransformMatrix,
                                                                         normalMatrix);
            }
        }

        return std::make_unique<RenderableObject>(batchMaterial,
                                                  batchVertexBuffer,
                                                  batchVertexBufferName);
    }
}

std::unique_ptr<RenderableObject>
StaticBatcher::CreateBatch(const SceneObject& sourceSceneObject,
                           const Optional<std::string>& batchVertexBufferName) {
    auto staticBatchSetup = SetUpStaticBatchIfPossible(sourceSceneObject);
    if (staticBatchSetup.mBatchVertexBuffer == nullptr ||
        staticBatchSetup.mBatchMaterial == nullptr) {
        
        return nullptr;
    }
    
    if (batchVertexBufferName.HasValue()) {
        auto gpuVertexBuffer = VertexBufferCache::Get(batchVertexBufferName.GetValue());
        if (gpuVertexBuffer == nullptr) {
            return CreateStaticBatchRenderable(*staticBatchSetup.mBatchVertexBuffer,
                                               sourceSceneObject,
                                               *staticBatchSetup.mBatchMaterial,
                                               batchVertexBufferName);
        } else {
            return std::make_unique<RenderableObject>(*staticBatchSetup.mBatchMaterial,
                                                      gpuVertexBuffer);
        }
    }
    
    return CreateStaticBatchRenderable(*staticBatchSetup.mBatchVertexBuffer,
                                       sourceSceneObject,
                                       *staticBatchSetup.mBatchMaterial,
                                       batchVertexBufferName);
}
