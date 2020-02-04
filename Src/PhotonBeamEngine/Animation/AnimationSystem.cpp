#include "AnimationSystem.hpp"

#include "SceneObject.hpp"

using namespace Pht;

void AnimationSystem::AddAnimation(Animation& animation) {
    if (std::find(std::begin(mAnimations), std::end(mAnimations), &animation) ==
        std::end(mAnimations)) {

        mAnimations.push_back(&animation);
    }
}

void AnimationSystem::RemoveAnimation(Animation& animation) {
    mAnimations.erase(
        std::remove(std::begin(mAnimations), std::end(mAnimations), &animation),
        std::end(mAnimations));
}

Animation& AnimationSystem::CreateAnimation(SceneObject& sceneObject,
                                            const std::vector<Keyframe>& keyframes) {
    auto animation = std::make_unique<Animation>(sceneObject, keyframes, *this);
    auto& retVal = *animation;
    
    sceneObject.SetComponent<Animation>(std::move(animation));
    
    return retVal;
}

void AnimationSystem::Update(float dt) {
    for (auto* animation: mAnimations) {
        animation->Update(dt);
    }
}
