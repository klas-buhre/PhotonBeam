#include "SceneObjectUtils.hpp"

#include "SceneObject.hpp"
#include "TextComponent.hpp"

using namespace Pht;

namespace {
    void ScaleTextRecursively(Pht::SceneObject& sceneObject, float scale) {
        if (auto* textComponent = sceneObject.GetComponent<Pht::TextComponent>()) {
            textComponent->GetProperties().mScale = scale;
        }

        for (auto* child: sceneObject.GetChildren()) {
            ScaleTextRecursively(*child, scale);
        }
    }
}

void SceneObjectUtils::ScaleRecursively(Pht::SceneObject& sceneObject, float scale) {
    sceneObject.GetTransform().SetScale(scale);
    ScaleTextRecursively(sceneObject, scale);
}

void SceneObjectUtils::SetAlphaRecursively(Pht::SceneObject& sceneObject, float alpha) {
    for (auto& child: sceneObject.GetChildren()) {
        SetAlphaRecursively(*child, alpha);
    }
    
    if (auto* renderable = sceneObject.GetRenderable()) {
        renderable->GetMaterial().SetOpacity(alpha);
    }
}

void SceneObjectUtils::SetEmissiveInRenderable(Pht::RenderableObject& renderableObject,
                                               float emissive) {
    Pht::Color emissiveColor {emissive, emissive, emissive};
    renderableObject.GetMaterial().SetEmissive(emissiveColor);
}
