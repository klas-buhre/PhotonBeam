#ifndef IAnimationSystem_hpp
#define IAnimationSystem_hpp

#include "Animation.hpp"

namespace Pht {
    class SceneObject;
    
    class IAnimationSystem {
    public:
        virtual ~IAnimationSystem() {}

        virtual void AddAnimation(Animation& animation) = 0;
        virtual void RemoveAnimation(Animation& animation) = 0;
        virtual Animation& CreateAnimation(SceneObject& sceneObject,
                                           const std::vector<Keyframe>& keyframes) = 0;
    };
}

#endif
