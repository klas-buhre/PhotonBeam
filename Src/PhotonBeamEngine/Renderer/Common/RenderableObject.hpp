#ifndef RenderableObject_hpp
#define RenderableObject_hpp

#include "Material.hpp"
#include "Optional.hpp"
#include "VertexBufferCache.hpp"

namespace Pht {
    class VertexBuffer;
    class IMesh;
    class VertexFlags;
    
    enum class RenderMode {
        Points,
        Triangles
    };
        
    enum class VertexBufferLocation {
        AtGpuOnly,
        AtGpuAndCpu
    };
    
    class RenderableObject {
    public:
        RenderableObject(const Material& material,
                         const IMesh& mesh,
                         const VertexFlags& attributeFlags,
                         VertexBufferLocation bufferLocation);
        RenderableObject(const Material& material,
                         std::unique_ptr<VertexBuffer> cpuSideBuffer,
                         RenderMode renderMode);
        RenderableObject(const Material& material,
                         const VertexBuffer& fromBuffer,
                         const Optional<std::string>& bufferName);
        RenderableObject(const Material& material, std::shared_ptr<GpuVertexBuffer> buffer);

        ~RenderableObject();
        
        void UploadTriangles(BufferUsage bufferUsage);
        void UploadPoints(BufferUsage bufferUsage);
        
        const GpuVertexBuffer& GetGpuVertexBuffer() const {
            return *mGpuVertexBuffer;
        }

        RenderMode GetRenderMode() const {
            return mRenderMode;
        }

        const Material& GetMaterial() const {
            return mMaterial;
        }
        
        Material& GetMaterial() {
            return mMaterial;
        }
        
    private:
        void UploadMeshVertexData(const IMesh& mesh,
                                  const VertexFlags& attributeFlags,
                                  VertexBufferLocation bufferLocation);
        
        RenderMode mRenderMode {RenderMode::Triangles};
        Material mMaterial;
        std::shared_ptr<GpuVertexBuffer> mGpuVertexBuffer;
    };
}

#endif
