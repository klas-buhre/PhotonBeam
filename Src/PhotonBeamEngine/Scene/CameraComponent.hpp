#ifndef CameraComponent_hpp
#define CameraComponent_hpp

#include "ISceneObjectComponent.hpp"
#include "Vector.hpp"

namespace Pht {
    class SceneObject;

    class CameraComponent: public ISceneObjectComponent {
    public:
        static const ComponentId id;
        
        explicit CameraComponent(SceneObject& sceneObject);
        
        void SetTarget(const Vec3& target, const Vec3& up);

        SceneObject& GetSceneObject() {
            return mSceneObject;
        }
        
        const SceneObject& GetSceneObject() const {
            return mSceneObject;
        }

        const Vec3& GetTarget() const {
            return mTarget;
        }
        
        const Vec3& GetUp() const {
            return mUp;
        }
        
    private:
        SceneObject& mSceneObject;
        Vec3 mTarget {0.0f, 0.0f, 0.0f};
        Vec3 mUp {0.0f, 1.0f, 0.0f};
    };
}

#endif
