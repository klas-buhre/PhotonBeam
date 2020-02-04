#ifndef VertexBufferCache_hpp
#define VertexBufferCache_hpp

#include <string>
#include <memory>

#include "VertexBuffer.hpp"

namespace Pht {
    enum class GenerateIndexBuffer {
        Yes,
        No
    };
    
    enum class BufferUsage {
        StaticDraw,
        DynamicDraw
    };
    
    class GpuVertexBufferHandles;
    
    class GpuVertexBuffer {
    public:
        GpuVertexBuffer(GenerateIndexBuffer generateIndexBuffer);
        ~GpuVertexBuffer();
        
        void UploadTriangles(const VertexBuffer& vertexBuffer, BufferUsage bufferUsage);
        void UploadPoints(const VertexBuffer& vertexBuffer, BufferUsage bufferUsage);
        
        int GetIndexCount() const {
            return mIndexCount;
        }
        
        int GetPointCount() const {
            return mPointCount;
        }
        
        const GpuVertexBufferHandles* GetHandles() const {
            return mHandles.get();
        }
        
        uint32_t GetId() const {
            return mId;
        }
        
        void SetCpuSideBuffer(std::unique_ptr<VertexBuffer> buffer) {
            mCpuSideBuffer = std::move(buffer);
        }
        
        const VertexBuffer* GetCpuSideBuffer() const {
            return mCpuSideBuffer.get();
        }

    private:
        static uint32_t mIdCounter;
        
        uint32_t mId {mIdCounter++};
        int mIndexCount {0};
        int mPointCount {0};
        std::unique_ptr<GpuVertexBufferHandles> mHandles;
        std::unique_ptr<VertexBuffer> mCpuSideBuffer;
    };
    
    namespace VertexBufferCache {
        std::shared_ptr<GpuVertexBuffer> Get(const std::string& meshName);
        void Add(const std::string& meshName, std::shared_ptr<GpuVertexBuffer> buffer);
    }
}

#endif
