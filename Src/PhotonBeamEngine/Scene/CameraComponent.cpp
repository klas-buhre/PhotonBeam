#include "CameraComponent.hpp"

#include "Fnv1Hash.hpp"

using namespace Pht;

const ComponentId CameraComponent::id {Hash::Fnv1a("CameraComponent")};

CameraComponent::CameraComponent(SceneObject& sceneObject) :
    mSceneObject {sceneObject} {}

void CameraComponent::SetTarget(const Vec3& target, const Vec3& up) {
    mTarget = target;
    mUp = up;
}
