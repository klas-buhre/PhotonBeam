#ifndef IRendererInternal_hpp
#define IRendererInternal_hpp

#include <memory>

#include "IRenderer.hpp"
#include "RenderableObject.hpp"

namespace Pht {
    class Scene;

    class IRendererInternal: public IRenderer {
    public:
        virtual ~IRendererInternal() {}
        
        virtual void Init(bool createFrameBuffer) = 0;
        virtual void InitCamera(float narrowFrustumHeightFactor) = 0;
        virtual void InitRenderQueue(const Scene& scene) = 0;
        virtual std::unique_ptr<RenderableObject> CreateRenderableObject(const IMesh& mesh,
                                                                         const Material& material,
                                                                         VertexBufferLocation bufferLocation) = 0;
        virtual void ClearFrameBuffer() = 0;
        virtual void RenderScene(const Scene& scene, float frameSeconds) = 0;
    };
    
    std::unique_ptr<IRendererInternal> CreateRenderer(bool createFrameBuffer);
}

#endif
