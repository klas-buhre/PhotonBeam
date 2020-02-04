#include "Animation.hpp"

#include <assert.h>

#include "AnimationSystem.hpp"
#include "Fnv1Hash.hpp"
#include "SceneObject.hpp"

using namespace Pht;

const ComponentId Animation::id {Hash::Fnv1a("Animation")};

Animation::Animation(SceneObject& sceneObject,
                     const std::vector<Keyframe>& keyframes,
                     IAnimationSystem& animationSystem) :
    mSceneObject {sceneObject},
    mAnimationSystem {animationSystem} {
    
    CreateClip(keyframes, 0);
    SetDefaultClip(0);
}

Animation::~Animation() {
    mAnimationSystem.RemoveAnimation(*this);
}

AnimationClip& Animation::CreateClip(const std::vector<Keyframe>& keyframes,
                                     AnimationClipId clipId) {
    auto clip = std::make_unique<AnimationClip>(keyframes, mSceneObject);
    auto& retVal = *clip;
    mClips.insert(std::make_pair(clipId, std::move(clip)));
    return retVal;
}

void Animation::SetDefaultClip(AnimationClipId clipId) {
    auto* clip = GetClip(clipId);
    assert(clip);
    mDefaultClip = clip;
}

void Animation::SetInterpolation(Interpolation interpolation, AnimationClipId clipId) {
    auto* clip = GetClip(clipId);
    assert(clip);
    clip->SetInterpolation(interpolation);
}

AnimationClip* Animation::GetClip(AnimationClipId clipId) {
    auto i = mClips.find(clipId);
    if (i != std::end(mClips)) {
        return i->second.get();
    }

    return nullptr;
}

void Animation::Update(float dt) {
    for (auto& entry: mClips) {
        auto* clip = entry.second.get();
        if (clip->IsPlaying()) {
            clip->Update(dt);
        }
    }
    
    PerformActionOnChildAnimations(Action::Update, mSceneObject, dt);
}

void Animation::Play() {
    assert(mDefaultClip);
    mDefaultClip->Play();
    
    PerformActionOnChildAnimations(Action::Play, mSceneObject);
}

void Animation::Play(AnimationClipId clipId) {
    auto* clip = GetClip(clipId);
    assert(clip);
    clip->Play();
}

void Animation::Pause() {
    for (auto& entry: mClips) {
        entry.second->Pause();
    }

    PerformActionOnChildAnimations(Action::Pause, mSceneObject);
}

void Animation::Stop() {
    for (auto& entry: mClips) {
        entry.second->Stop();
    }
    
    PerformActionOnChildAnimations(Action::Stop, mSceneObject);
}

void Animation::PerformActionOnChildAnimations(Action action, SceneObject& sceneObject, float dt) {
    for (auto* childSceneObject: sceneObject.GetChildren()) {
        auto* childAnimation = childSceneObject->GetComponent<Animation>();
        if (childAnimation) {
            switch (action) {
                case Action::Update:
                    childAnimation->Update(dt);
                    break;
                case Action::Play:
                    childAnimation->Play();
                    break;
                case Action::Pause:
                    childAnimation->Pause();
                    break;
                case Action::Stop:
                    childAnimation->Stop();
                    break;
            }
        } else {
            PerformActionOnChildAnimations(action, *childSceneObject, dt);
        }
    }
}

bool Animation::IsPlaying() const {
    for (auto& entry: mClips) {
        auto* clip = entry.second.get();
        if (clip->IsPlaying()) {
            return true;
        }
    }
    
    return false;
}
