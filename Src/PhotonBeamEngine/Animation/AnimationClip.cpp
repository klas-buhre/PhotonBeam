#include "AnimationClip.hpp"

#include <assert.h>

#include "SceneObject.hpp"
#include "SceneObjectUtils.hpp"

using namespace Pht;

namespace {
    template<typename T>
    T CosineInterpolateBetweenKeyframes(const T& keyframeValue,
                                        const T& nextKeyframeValue,
                                        float normalizedTime) {
        auto t = std::cos(3.1415f + normalizedTime * 3.1415f) * 0.5f + 0.5f;
        return keyframeValue + (nextKeyframeValue - keyframeValue) * t;
    }

    template<typename T>
    T InterpolateBetweenKeyframes(const T& keyframeValue,
                                  const T& nextKeyframeValue,
                                  float normalizedTime,
                                  Interpolation interpolation) {
        switch (interpolation) {
            case Interpolation::Linear:
                return keyframeValue + (nextKeyframeValue - keyframeValue) * normalizedTime;
            case Interpolation::Cosine:
                return CosineInterpolateBetweenKeyframes(keyframeValue,
                                                         nextKeyframeValue,
                                                         normalizedTime);
            case Interpolation::None:
                assert(false);
                break;
        }
    }
}

AnimationClip::AnimationClip(const std::vector<Keyframe>& keyframes, SceneObject& sceneObject) :
    mSceneObject {sceneObject} {

    for (auto& keyframe: keyframes) {
        if (mKeyframes.empty()) {
            assert(keyframe.mTime == 0.0f);
        } else {
            assert(keyframe.mTime > mKeyframes.back().mTime);
        }
        
        mKeyframes.push_back(keyframe);
    }
    
    Rewind();
}

void AnimationClip::Update(float dt) {
    if (!mIsPlaying) {
        return;
    }
    
    if (mElapsedTime == 0.0f) {
        HandleKeyframeTransition(mKeyframes.front());
    }
    
    mElapsedTime += dt;
    if (mElapsedTime >= mNextKeyframe->mTime) {
        if (!CalculateKeyframe()) {
            switch (mWrapMode) {
                case WrapMode::Once:
                    Stop();
                    return;
                case WrapMode::Loop:
                    Rewind();
                    break;
            }
        }
    }
    
    if (mInterpolation != Interpolation::None) {
        UpdateInterpolation();
    }
}

bool AnimationClip::CalculateKeyframe() {
    for (auto i = 0; i < mKeyframes.size() - 1; ++i) {
        auto& keyframe = mKeyframes[i];
        auto& nextKeyframe = mKeyframes[i + 1];
        if (mElapsedTime >= keyframe.mTime && mElapsedTime <= nextKeyframe.mTime) {
            for (auto j = mKeyframeIndex + 1; j <= i; ++j) {
                HandleKeyframeTransition(mKeyframes[j]);
            }
            
            mKeyframe = &keyframe;
            mNextKeyframe = &nextKeyframe;
            mKeyframeIndex = i;
            return true;
        }
    }
    
    for (auto j = mKeyframeIndex + 1; j < mKeyframes.size(); ++j) {
        HandleKeyframeTransition(mKeyframes[j]);
    }
    
    return false;
}

void AnimationClip::HandleKeyframeTransition(const Keyframe& newKeyframe) {
    auto& transform = mSceneObject.GetTransform();
    auto& position = newKeyframe.mPosition;
    if (position.HasValue()) {
        transform.SetPosition(position.GetValue());
    }
    
    auto& scale = newKeyframe.mScale;
    if (scale.HasValue()) {
        transform.SetScale(scale.GetValue());
    }

    auto& rotation = newKeyframe.mRotation;
    if (rotation.HasValue()) {
        transform.SetRotation(rotation.GetValue());
    }

    auto textScale = newKeyframe.mTextScale;
    if (textScale.HasValue()) {
        SceneObjectUtils::ScaleRecursively(mSceneObject, textScale.GetValue());
    }

    auto isVisible = newKeyframe.mIsVisible;
    if (isVisible.HasValue()) {
        mSceneObject.SetIsVisible(isVisible.GetValue());
    }
    
    auto& callback = newKeyframe.mCallback;
    if (callback) {
        callback();
    }
}

void AnimationClip::UpdateInterpolation() {
    if (mKeyframe == nullptr || mNextKeyframe == nullptr) {
        return;
    }
    
    auto timeBetweenKeyframes = mNextKeyframe->mTime - mKeyframe->mTime;
    auto elapsedInBetweenTime = std::fmax(mElapsedTime - mKeyframe->mTime, 0.0f);
    auto normalizedTime = elapsedInBetweenTime / timeBetweenKeyframes;

    auto& transform = mSceneObject.GetTransform();
    auto& position = mKeyframe->mPosition;
    auto& nextPosition = mNextKeyframe->mPosition;
    if (position.HasValue() && nextPosition.HasValue()) {
        auto interpolatedPosition =
            InterpolateBetweenKeyframes(position.GetValue(),
                                        nextPosition.GetValue(),
                                        normalizedTime,
                                        mInterpolation);
        transform.SetPosition(interpolatedPosition);
    }

    auto& scale = mKeyframe->mScale;
    auto& nextScale = mNextKeyframe->mScale;
    if (scale.HasValue() && nextScale.HasValue()) {
        auto interpolatedScale =
            InterpolateBetweenKeyframes(scale.GetValue(),
                                        nextScale.GetValue(),
                                        normalizedTime,
                                        mInterpolation);
        transform.SetScale(interpolatedScale);
    }

    auto& rotation = mKeyframe->mRotation;
    auto& nextRotation = mNextKeyframe->mRotation;
    if (rotation.HasValue() && nextRotation.HasValue()) {
        auto interpolatedRotation =
            InterpolateBetweenKeyframes(rotation.GetValue(),
                                        nextRotation.GetValue(),
                                        normalizedTime,
                                        mInterpolation);
        transform.SetRotation(interpolatedRotation);
    }
    
    auto textScale = mKeyframe->mTextScale;
    auto nextTextScale = mNextKeyframe->mTextScale;
    if (textScale.HasValue() && nextTextScale.HasValue()) {
        auto interpolatedScale =
            InterpolateBetweenKeyframes(textScale.GetValue(),
                                        nextTextScale.GetValue(),
                                        normalizedTime,
                                        mInterpolation);
        SceneObjectUtils::ScaleRecursively(mSceneObject, interpolatedScale);
    }
}

void AnimationClip::Play() {
    assert(mKeyframes.size() >= 2);
    mIsPlaying = true;
}

void AnimationClip::Pause() {
    mIsPlaying = false;
}

void AnimationClip::Stop() {
    mIsPlaying = false;
    Rewind();
}

void AnimationClip::Rewind() {
    assert(mKeyframes.size() >= 2);
    mElapsedTime = 0.0f;
    mKeyframeIndex = 0;
    mKeyframe = &mKeyframes[0];
    mNextKeyframe = &mKeyframes[1];
}
