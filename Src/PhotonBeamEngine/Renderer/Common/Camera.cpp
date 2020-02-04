#include "Camera.hpp"

using namespace Pht;

Camera::Camera() {}

Camera::Camera(const IVec2& renderBufferSize, float frustumHeight, float nearClip, float farClip) {
    float w {frustumHeight * renderBufferSize.x / renderBufferSize.y};
    mProjection = Mat4::Frustum(-w / 2.0f,
                                w / 2.0f,
                                -frustumHeight / 2.0f,
                                frustumHeight / 2.0f,
                                nearClip,
                                farClip);
}

void Camera::SetOrthographicProjection(const IVec2& renderBufferSize,
                                       float frustumHeight,
                                       float nearClip,
                                       float farClip) {
    float w {frustumHeight * renderBufferSize.x / renderBufferSize.y};
    mOrthographicProjection = Mat4::OrthographicProjection(-w / 2.0f,
                                                           w / 2.0f,
                                                           -frustumHeight / 2.0f,
                                                           frustumHeight / 2.0f,
                                                           nearClip,
                                                           farClip);
}

void Camera::LookAt(const Vec3& cameraPosition, const Vec3& target, const Vec3& up) {
    mPosition = cameraPosition;
    mView = Mat4::LookAt(cameraPosition, target, up);
}
