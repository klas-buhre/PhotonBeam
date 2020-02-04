#ifndef RenderQueue_hpp
#define RenderQueue_hpp

#include <vector>

#include "Vector.hpp"
#include "Matrix.hpp"
#include "Scene.hpp"
#include "RenderPass.hpp"

namespace Pht {
    class SceneObject;
    
    class RenderQueue {
    public:
        // Sort key bit layout.
        // Depth write flag: bit 60.
        static constexpr int depthWriteShift {60};
        static constexpr uint64_t depthWriteShiftedMask {uint64_t{1} << depthWriteShift};

        // Text kind: bits 59-56:
        static constexpr int textKindShift {56};
        static constexpr uint64_t textKindMask {0xF};

        // Shader id: bits 55-48:
        static constexpr int shaderIdShift {48};
        
        // Material id: bits 47-24:
        static constexpr int materialIdShift {24};
        
        // Vertex buffer id: bits 23-0:
        static constexpr int vertexBufferIdShift {0};
        
        enum class TextKind: uint8_t {
            None = 0,
            PlainShader,            // Render last.
            DoubleGradientShader,
            TopGradientShader,
            MidGradientShader,
            Specular,
            Shadow,
            SecondShadow            // Render first.
        };
        
        struct Entry {
            uint64_t mSortKey;
            float mDistance;
            const SceneObject* mSceneObject;

            bool IsDepthWriting() const {
                return static_cast<bool>(mSortKey & depthWriteShiftedMask);
            }
            
            TextKind GetTextKind() const {
                return static_cast<TextKind>((mSortKey >> textKindShift) & textKindMask);
            }
        };
        
        void Init(const SceneObject& rootSceneObject);
        void Build(const Mat4& viewMatrix,
                   RenderOrder renderOrder,
                   DistanceFunction distanceFunction,
                   int layerMask);
        void BeginIteration();
        Entry& GetNextEntry();
        
        bool HasMoreEntries() const {
            return mIteratorIndex < mCurrentPartitionEndIndex;
        }

    private:
        void Sort();
        void ScanSubtree(const SceneObject& sceneObject, bool ancestorMatchedLayerMask);
        void ScanSceneObject(const SceneObject& sceneObject);
        void AddEntry(uint64_t sortKey, bool isDepthWriting, const SceneObject& sceneObject);
        void CalculateDistances(const Mat4& viewMatrix, DistanceFunction distanceFunction);
        
        template<typename Comparator>
        void SortFirstPartition();

        template<typename Comparator>
        void SortSecondPartition();

        const SceneObject* mRootSceneObject;
        RenderOrder mRenderOrder {RenderOrder::StateOptimized};
        int mLayerMask {0};
        std::vector<Entry> mQueue;
        int mFirstPartitionSize {0};
        int mSecondPartitionSize {0};
        int mMaxSize {0};
        int mIteratorIndex {0};
        int mCurrentPartitionEndIndex {0};
    };
}

#endif
