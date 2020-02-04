#ifndef SceneObject_hpp
#define SceneObject_hpp

#include <vector>

#include "Matrix.hpp"
#include "RenderableObject.hpp"
#include "ISceneObjectComponent.hpp"
#include "Transform.hpp"
#include "Noncopyable.hpp"

namespace Pht {
    class SceneObject: public Noncopyable {
    public:
        using Name = uint32_t;
        
        SceneObject();
        explicit SceneObject(Name name);
        explicit SceneObject(RenderableObject* renderable);
        
        Vec3 GetWorldSpacePosition() const;
        void Update(bool parentMatrixChanged);
        void InitialUpdate(bool parentMatrixChanged);
        void AddChild(SceneObject& child);
        void DetachChild(const SceneObject* child);
        void DetachChildren();
        SceneObject* Find(Name name);
        void SetLayer(int layerIndex);
        
        template<typename T>
        void SetComponent(std::unique_ptr<T> component) {
            static_assert(std::is_base_of<ISceneObjectComponent, T>::value, "T must be a component");
            for (auto& entry: mComponents) {
                if (entry.first == T::id) {
                    entry.second = std::move(component);
                    return;
                }
            }
            mComponents.emplace_back(T::id, std::move(component));
        }
        
        template<typename T>
        T* GetComponent() {
            static_assert(std::is_base_of<ISceneObjectComponent, T>::value, "T must be a component");
            for (const auto& entry: mComponents) {
                if (entry.first == T::id) {
                    return static_cast<T*>(entry.second.get());
                }
            }
            return nullptr;
        }

        template<typename T>
        const T* GetComponent() const {
            static_assert(std::is_base_of<ISceneObjectComponent, T>::value, "T must be a component");
            for (const auto& entry: mComponents) {
                if (entry.first == T::id) {
                    return static_cast<const T*>(entry.second.get());
                }
            }
            return nullptr;
        }

        void SetName(Name name) {
            mName = name;
        }

        const Transform& GetTransform() const {
            return mTransform;
        }

        Transform& GetTransform() {
            return mTransform;
        }
        
        const RenderableObject* GetRenderable() const {
            return mRenderable;
        }
        
        void SetRenderable(RenderableObject* renderable) {
            mRenderable = renderable;
        }
        
        RenderableObject* GetRenderable() {
            return mRenderable;
        }
        
        const Mat4& GetMatrix() const {
            return mMatrix;
        }
        
        bool IsVisible() const {
            return mIsVisible;
        }
        
        void SetIsVisible(bool isVisible) {
            mIsVisible = isVisible;
        }

        bool IsStatic() const {
            return mIsStatic;
        }
        
        void SetIsStatic(bool isStatic) {
            mIsStatic = isStatic;
        }
        
        const std::vector<SceneObject*>& GetChildren() const {
            return mChildren;
        }
        
        int GetLayerMask() const {
            return mLayerMask;
        }
        
    private:
        Name mName {0};
        Transform mTransform;
        Mat4 mMatrix;
        int mLayerMask {0};
        RenderableObject* mRenderable {nullptr};
        SceneObject* mParent {nullptr};
        std::vector<SceneObject*> mChildren;
        std::vector<std::pair<ComponentId, std::unique_ptr<ISceneObjectComponent>>> mComponents;
        bool mIsVisible {true};
        bool mIsStatic {false};
    };
}

#endif
