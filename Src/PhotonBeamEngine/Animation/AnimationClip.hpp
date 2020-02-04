#ifndef AnimationClip_hpp
#define AnimationClip_hpp

#include <vector>
#include <functional>

#include "Vector.hpp"
#include "Optional.hpp"
#include "Noncopyable.hpp"

namespace Pht {
    class SceneObject;

    enum class Interpolation {
        None,
        Linear,
        Cosine
    };
    
    enum class WrapMode {
        Once,
        Loop
    };

    struct Keyframe {
        float mTime {0.0f};
        Optional<Vec3> mPosition;
        Optional<Vec3> mScale;
        Optional<Vec3> mRotation;
        Optional<float> mTextScale;
        Optional<bool> mIsVisible;
        std::function<void()> mCallback;
    };

    using AnimationClipId = uint32_t;
    
    class AnimationClip: public Noncopyable {
    public:
        AnimationClip(const std::vector<Keyframe>& keyframes, SceneObject& sceneObject);
        
        void SetInterpolation(Interpolation interpolation) {
            mInterpolation = interpolation;
        }
        
        void SetWrapMode(WrapMode wrapMode) {
            mWrapMode = wrapMode;
        }
                
        bool IsPlaying() const {
            return mIsPlaying;
        }
        
    private:
        friend class Animation;

        void Update(float dt);
        void Play();
        void Pause();
        void Stop();
        void Rewind();
        bool CalculateKeyframe();
        void HandleKeyframeTransition(const Keyframe& newKeyframe);
        void UpdateInterpolation();

        SceneObject& mSceneObject;
        Interpolation mInterpolation {Interpolation::Linear};
        WrapMode mWrapMode {WrapMode::Loop};
        std::vector<Keyframe> mKeyframes;
        int mKeyframeIndex {0};
        const Keyframe* mKeyframe {nullptr};
        const Keyframe* mNextKeyframe {nullptr};
        float mElapsedTime {0.0f};
        bool mIsPlaying {false};
    };
}

#endif
