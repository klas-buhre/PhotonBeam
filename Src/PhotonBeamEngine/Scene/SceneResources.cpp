#include "SceneResources.hpp"

using namespace Pht;

SceneObject& SceneResources::CreateSceneObject() {
    auto sceneObject = std::make_unique<SceneObject>();
    auto& retVal = *sceneObject;
    mSceneObjects.push_back(std::move(sceneObject));
    return retVal;
}

void SceneResources::AddSceneObject(std::unique_ptr<SceneObject> sceneObject) {
    mSceneObjects.push_back(std::move(sceneObject));
}

void SceneResources::AddRenderableObject(std::unique_ptr<RenderableObject> renderableObject) {
    mRenderableObjects.push_back(std::move(renderableObject));
}
