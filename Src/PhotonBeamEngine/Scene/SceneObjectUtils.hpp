#ifndef SceneObjectUtils_hpp
#define SceneObjectUtils_hpp

namespace Pht {
    class SceneObject;
    class RenderableObject;

    namespace SceneObjectUtils {
        void ScaleRecursively(Pht::SceneObject& sceneObject, float scale);
        void SetAlphaRecursively(Pht::SceneObject& sceneObject, float alpha);
        void SetEmissiveInRenderable(Pht::RenderableObject& renderableObject, float emissive);
    }
}

#endif
