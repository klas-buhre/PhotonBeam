#ifndef CameraShake_hpp
#define CameraShake_hpp

// FastNoise include.
#include "FastNoise.h"

#include "Vector.hpp"

namespace Pht {
    class CameraShake {
    public:
        enum class ShakeKind {
            Random,
            PerlinNoise
        };

        CameraShake(ShakeKind shakeKind);
        
        void Init();
        void Update(float dt);
        void StartShake(float shakeTime, float magnitude);
        bool IsShaking() const;
        
        const Vec3& GetCameraTranslation() const {
            return mCameraTranslation;
        }

    private:
        void UpdateInShakingState(float dt);
        
        enum class State {
            Shaking,
            LastShake,
            Inactive
        };
        
        ShakeKind mShakeKind {ShakeKind::PerlinNoise};
        State mState {State::Inactive};
        float mElapsedTime {0.0f};
        float mShakeTime {0.0f};
        float mMagnitude {0.0f};
        Vec3 mCameraTranslation;
        FastNoise mFastNoise;
        float mSamplePoint {0.0f};
    };
}

#endif
