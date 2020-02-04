#include "GuiView.hpp"

#include "TextComponent.hpp"
#include "ISceneManager.hpp"

using namespace Pht;

GuiView::GuiView() :
    mRoot {std::make_unique<Pht::SceneObject>()} {}

void GuiView::SetPosition(const Vec2& position) {
    SetPosition({position.x, position.y, 0.0f});
}

void GuiView::SetPosition(const Vec3& position) {
    mRoot->GetTransform().SetPosition(position);
}

void GuiView::SetIsActive(bool isActive) {
    if (isActive) {
        mRoot->SetIsVisible(true);
        mRoot->SetIsStatic(false);
    } else {
        mRoot->SetIsVisible(false);
        mRoot->SetIsStatic(true);
    }
}

void GuiView::AddSceneObject(std::unique_ptr<SceneObject> sceneObject) {
    mRoot->AddChild(*sceneObject);
    mSceneResources.AddSceneObject(std::move(sceneObject));
}

TextComponent& GuiView::CreateText(const Vec3& position,
                                   const std::string& text,
                                   const TextProperties& properties) {
    return CreateText(position, text, properties, *mRoot);
}

TextComponent& GuiView::CreateText(const Vec3& position,
                                   const std::string& text,
                                   const TextProperties& properties,
                                   Pht::SceneObject& parent) {
    auto sceneObject = std::make_unique<SceneObject>();
    auto textComponent = std::make_unique<TextComponent>(*sceneObject, text, properties);
    
    auto& retVal = *textComponent;
    sceneObject->SetComponent<TextComponent>(std::move(textComponent));
    sceneObject->GetTransform().SetPosition(position);
    
    parent.AddChild(*sceneObject);
    mSceneResources.AddSceneObject(std::move(sceneObject));
    return retVal;
}

SceneObject& GuiView::CreateSceneObject(const IMesh& mesh,
                                        const Material& material,
                                        ISceneManager& sceneManager) {
    auto sceneObject = sceneManager.CreateSceneObject(mesh, material, mSceneResources);
    auto& retVal = *sceneObject;
    mSceneResources.AddSceneObject(std::move(sceneObject));
    return retVal;
}

SceneObject& GuiView::CreateSceneObject() {
    auto sceneObject = std::make_unique<SceneObject>();
    auto& retVal = *sceneObject;
    mSceneResources.AddSceneObject(std::move(sceneObject));
    return retVal;
}

Vec2 GuiView::GetPosition() const {
    auto& position = mRoot->GetTransform().GetPosition();
    return {position.x, position.y};
}
