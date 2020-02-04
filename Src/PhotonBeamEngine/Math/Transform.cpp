#include "Transform.hpp"

using namespace Pht;

namespace {
    static const Vec3 defaultRotation {0.0f, 0.0f, 0.0f};
    static const Mat3 identity3x3;
}

void Transform::Reset() {
    mPosition = {0.0f, 0.0f, 0.0f};
    mScale = {1.0f, 1.0f, 1.0f};
    mRotation = defaultRotation;
    mHasChanged = true;
}

void Transform::SetPosition(const Vec3& position) {
    mPosition = position;
    mHasChanged = true;
}

void Transform::SetScale(const Vec3& scale) {
    mScale = scale;
    mHasChanged = true;
}

void Transform::SetScale(float scale) {
    mScale = Vec3 {scale, scale, scale};
    mHasChanged = true;
}

void Transform::SetRotation(const Vec3& rotation) {
    mRotation = rotation;
    mHasChanged = true;
}

void Transform::Translate(const Vec3& translation) {
    mPosition += translation;
    mHasChanged = true;
}

void Transform::Scale(const Vec3& scale) {
    mScale *= scale;
    mHasChanged = true;
}

void Transform::Rotate(const Vec3& rotation) {
    mRotation += rotation;
    mHasChanged = true;
}

Mat4 Transform::ToMatrix() const {
    Mat3 rotation3x3 {MatrixInit::No};
    if (mRotation == defaultRotation) {
        rotation3x3 = identity3x3;
    } else {
        if (mRotation.x != 0.0f) {
            rotation3x3 = Mat3::RotateX(mRotation.x);
        }

        if (mRotation.y != 0.0f) {
            if (mRotation.x != 0.0f) {
                rotation3x3 = rotation3x3 * Mat3::RotateY(mRotation.y);
            } else {
                rotation3x3 = Mat3::RotateY(mRotation.y);
            }
        }

        if (mRotation.z != 0.0f) {
            if (mRotation.x != 0.0f || mRotation.y != 0.0f) {
                rotation3x3 = rotation3x3 * Mat3::RotateZ(mRotation.z);
            } else {
                rotation3x3 = Mat3::RotateZ(mRotation.z);
            }
        }
    }
    
    Mat4 result {MatrixInit::No};
    result.x.x = mScale.x * rotation3x3.x.x;
    result.x.y = mScale.x * rotation3x3.x.y;
    result.x.z = mScale.x * rotation3x3.x.z;

    result.y.x = mScale.y * rotation3x3.y.x;
    result.y.y = mScale.y * rotation3x3.y.y;
    result.y.z = mScale.y * rotation3x3.y.z;
    
    result.z.x = mScale.z * rotation3x3.z.x;
    result.z.y = mScale.z * rotation3x3.z.y;
    result.z.z = mScale.z * rotation3x3.z.z;

    result.w.x = mPosition.x;
    result.w.y = mPosition.y;
    result.w.z = mPosition.z;
    
    result.x.w = 0.0f;
    result.y.w = 0.0f;
    result.z.w = 0.0f;
    result.w.w = 1.0f;
    
    return result;
}
