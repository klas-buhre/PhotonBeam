#ifndef SceneResources_hpp
#define SceneResources_hpp

#include <memory>
#include <vector>

#include "SceneObject.hpp"
#include "RenderableObject.hpp"

namespace Pht {
    class SceneResources {
    public:
        SceneObject& CreateSceneObject();
        void AddSceneObject(std::unique_ptr<SceneObject> sceneObject);
        void AddRenderableObject(std::unique_ptr<RenderableObject> renderableObject);
        
    private:
        std::vector<std::unique_ptr<SceneObject>> mSceneObjects;
        std::vector<std::unique_ptr<RenderableObject>> mRenderableObjects;
    };
}

#endif
