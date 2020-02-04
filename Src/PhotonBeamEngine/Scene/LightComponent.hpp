#ifndef LightComponent_hpp
#define LightComponent_hpp

#include "ISceneObjectComponent.hpp"
#include "Vector.hpp"

namespace Pht {
    class SceneObject;
    
    class LightComponent: public ISceneObjectComponent {
    public:
        static const ComponentId id;
        
        explicit LightComponent(SceneObject& sceneObject);
        
        SceneObject& GetSceneObject() {
            return mSceneObject;
        }
        
        const SceneObject& GetSceneObject() const {
            return mSceneObject;
        }
        
        void SetDirection(const Vec3& direction) {
            mDirection = direction;
        }
        
        const Vec3& GetDirection() const {
            return mDirection;
        }
        
        void SetDirectionalIntensity(float intesity) {
            mDirectionalIntensity = intesity;
        }

        float GetDirectionalIntensity() const {
            return mDirectionalIntensity;
        }

        void SetAmbientIntensity(float intesity) {
            mAmbientIntensity = intesity;
        }

        float GetAmbientIntensity() const {
            return mAmbientIntensity;
        }

    private:
        SceneObject& mSceneObject;
        Vec3 mDirection {1.0f, 1.0f, 1.0f};
        float mDirectionalIntensity {1.0f};
        float mAmbientIntensity {1.0f};
    };
}

#endif
