#include "Scene.hpp"

#include <assert.h>

#include "SceneObject.hpp"
#include "CameraComponent.hpp"
#include "LightComponent.hpp"
#include "TextComponent.hpp"
#include "Fnv1Hash.hpp"
#include "ISceneManager.hpp"

using namespace Pht;

Scene::Scene(ISceneManager& sceneManager, Name name) :
    mSceneManager {sceneManager},
    mName {name} {
    
    auto root = std::make_unique<SceneObject>(Hash::Fnv1a("root"));
    mRoot = root.get();
    mResources.AddSceneObject(std::move(root));
}

Scene::~Scene() {}

void Scene::Update() {
    GetRoot().Update(false);
}

void Scene::InitialUpdate() {
    GetRoot().InitialUpdate(false);
}

SceneObject& Scene::GetRoot() {
    assert(mRoot);
    return *mRoot;
}

const SceneObject& Scene::GetRoot() const {
    assert(mRoot);
    return *mRoot;
}

LightComponent& Scene::CreateGlobalLight() {
    auto sceneObject = std::make_unique<SceneObject>(Hash::Fnv1a("light"));
    sceneObject->SetIsVisible(false);
    
    auto lightComponent = std::make_unique<LightComponent>(*sceneObject);
    mGlobalLight = lightComponent.get();
    sceneObject->SetComponent<LightComponent>(std::move(lightComponent));
    
    mResources.AddSceneObject(std::move(sceneObject));
    return *mGlobalLight;
}

CameraComponent& Scene::CreateCamera() {
    auto sceneObject = std::make_unique<SceneObject>(Hash::Fnv1a("camera"));
    sceneObject->SetIsVisible(false);
    
    auto cameraComponent = std::make_unique<CameraComponent>(*sceneObject);
    mCamera = cameraComponent.get();
    sceneObject->SetComponent<CameraComponent>(std::move(cameraComponent));
    
    mResources.AddSceneObject(std::move(sceneObject));
    return *mCamera;
}

SceneObject& Scene::CreateSceneObject(const IMesh& mesh, const Material& material) {
    auto sceneObject = mSceneManager.CreateSceneObject(mesh, material, mResources);
    auto& retVal = *sceneObject;
    mResources.AddSceneObject(std::move(sceneObject));
    return retVal;
}

SceneObject& Scene::CreateSceneObject(SceneObject& parent) {
    auto& sceneObject = CreateSceneObject();
    parent.AddChild(sceneObject);
    return sceneObject;
}

SceneObject& Scene::CreateSceneObject() {
    auto sceneObject = std::make_unique<SceneObject>();
    auto& retVal = *sceneObject;
    mResources.AddSceneObject(std::move(sceneObject));
    return retVal;
}

TextComponent& Scene::CreateText(const std::string& text, const TextProperties& properties) {
    auto sceneObject = std::make_unique<SceneObject>();
    auto textComponent = std::make_unique<TextComponent>(*sceneObject, text, properties);
    auto& retVal = *textComponent;

    sceneObject->SetComponent<TextComponent>(std::move(textComponent));
    mResources.AddSceneObject(std::move(sceneObject));
    return retVal;
}

TextComponent& Scene::CreateText(const std::string& text,
                                 const TextProperties& properties,
                                 SceneObject& parent) {
    auto& textComponent = CreateText(text, properties);
    parent.AddChild(textComponent.GetSceneObject());
    return textComponent;
}

void Scene::ConvertSceneObjectToStaticBatch(SceneObject& sceneObject,
                                            const Optional<std::string>& batchVertexBufferName) {
    auto batchRenderable = mSceneManager.CreateStaticBatchRenderable(sceneObject,
                                                                     batchVertexBufferName);
    if (batchRenderable) {
        sceneObject.DetachChildren();
        sceneObject.SetRenderable(batchRenderable.get());
        mResources.AddRenderableObject(std::move(batchRenderable));
    }
}

void Scene::AddSceneObject(std::unique_ptr<SceneObject> sceneObject) {
    mResources.AddSceneObject(std::move(sceneObject));
}

void Scene::AddRenderableObject(std::unique_ptr<RenderableObject> renderableObject) {
    mResources.AddRenderableObject(std::move(renderableObject));
}

void Scene::AddRenderPass(const RenderPass& renderPass) {
    mRenderPasses.push_back(renderPass);
}

RenderPass* Scene::GetRenderPass(int layerIndex) {
    for (auto& renderPass: mRenderPasses) {
        if (renderPass.GetLayerMask() & (1 << layerIndex)) {
            return &renderPass;
        }
    }
    
    return nullptr;
}
