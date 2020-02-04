#ifndef IRenderer_hpp
#define IRenderer_hpp

#include "Matrix.hpp"
#include "Material.hpp"

namespace Pht {
    enum class ProjectionMode {
        Perspective,
        Orthographic
    };

    class IRenderer {
    public:
        virtual ~IRenderer() {}
        
        virtual void DisableShader(ShaderId shaderId) = 0;
        virtual void EnableShader(ShaderId shaderId) = 0;
        virtual void SetClearColorBuffer(bool clearColorBuffer) = 0;
        virtual void SetHudMode(bool hudMode) = 0;
        virtual void SetProjectionMode(ProjectionMode projectionMode) = 0;
        virtual int GetAdjustedNumPixels(int numPixels) const = 0;
        virtual const Vec2& GetHudFrustumSize() const = 0;
        virtual const Vec2& GetOrthographicFrustumSize() const = 0;
        virtual float GetFrustumHeightFactor() const = 0;
        virtual const IVec2& GetRenderBufferSize() const = 0;
        virtual float GetTopPaddingHeight() const = 0;
        virtual float GetBottomPaddingHeight() const = 0;
        virtual const Mat4& GetViewMatrix() const = 0;
        virtual const Mat4& GetProjectionMatrix() const = 0;
    };
}

#endif
