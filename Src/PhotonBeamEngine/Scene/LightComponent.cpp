#include "LightComponent.hpp"

#include "Fnv1Hash.hpp"

using namespace Pht;

const ComponentId LightComponent::id {Hash::Fnv1a("LightComponent")};

LightComponent::LightComponent(SceneObject& sceneObject) :
    mSceneObject {sceneObject} {}
