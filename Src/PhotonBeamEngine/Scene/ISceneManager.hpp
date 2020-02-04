#ifndef ISceneManager_hpp
#define ISceneManager_hpp

#include <memory>

#include "Scene.hpp"

namespace Pht {
    class RenderableObject;
    class IMesh;
    class Material;
    class SceneObject;
    class SceneResources;

    class ISceneManager {
    public:
        virtual ~ISceneManager() {}
        
        virtual std::unique_ptr<Scene> CreateScene(Scene::Name name) = 0;
        virtual std::unique_ptr<Scene> CreateScene(Scene::Name name, float narrowFrustumHeightFactor) = 0;
        virtual void InitSceneSystems(float narrowFrustumHeightFactor) = 0;
        virtual void SetLoadedScene(std::unique_ptr<Scene> scene) = 0;
        virtual Scene* GetActiveScene() = 0;
        virtual std::unique_ptr<RenderableObject> CreateRenderableObject(const IMesh& mesh,
                                                                         const Material& material) = 0;
        virtual std::unique_ptr<RenderableObject> CreateBatchableRenderableObject(const IMesh& mesh,
                                                                                  const Material& material) = 0;
        virtual std::unique_ptr<RenderableObject> CreateStaticBatchRenderable(const SceneObject& sceneObject,
                                                                              const Optional<std::string>& batchVertexBufferName) = 0;
        virtual std::unique_ptr<SceneObject> CreateSceneObject(const IMesh& mesh,
                                                               const Material& material,
                                                               SceneResources& sceneResources) = 0;

        static constexpr auto defaultNarrowFrustumHeightFactor {1.217f};
    };
}

#endif
