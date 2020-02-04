#ifndef Camera_hpp
#define Camera_hpp

#include "Vector.hpp"
#include "Matrix.hpp"

namespace Pht {
    class Camera {
    public:
        Camera();
        Camera(const IVec2& renderBufferSize, float frustumHeight, float nearClip, float farClip);
    
        void SetOrthographicProjection(const IVec2& renderBufferSize,
                                       float frustumHeight,
                                       float nearClip,
                                       float farClip);
        void LookAt(const Vec3& cameraPosition, const Vec3& target, const Vec3& up);
        
        const Mat4& GetProjectionMatrix() const {
            return mProjection;
        }

        const Mat4& GetOrthographicProjectionMatrix() const {
            return mOrthographicProjection;
        }
        
        const Mat4& GetViewMatrix() const {
            return mView;
        }
        
        const Vec3& GetPosition() const {
            return mPosition;
        }

    private:
        Mat4 mProjection;
        Mat4 mOrthographicProjection;
        Mat4 mView;
        Vec3 mPosition;
    };
}

#endif
