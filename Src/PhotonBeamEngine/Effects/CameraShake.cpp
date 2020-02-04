#include "CameraShake.hpp"

#include <cstdlib>
#include <array>

#include "MathUtils.hpp"

using namespace Pht;

namespace {
    constexpr auto speed = 19.0f;

    const std::array<float, 7> samplePointStart {
        90.0f,
        57.0f,
        63.0f,
        22.0f,
        186.0f,
        20.0f,
        194.0f
    };
}

CameraShake::CameraShake(ShakeKind shakeKind) :
    mShakeKind {shakeKind},
    mFastNoise {1337} {

    mFastNoise.SetFrequency(1.0f);
    Init();
}

void CameraShake::Init() {
    mState = State::Inactive;
    mCameraTranslation = {0.0f, 0.0f, 0.0f};
}

void CameraShake::Update(float dt) {
    switch (mState) {
        case State::Shaking:
            UpdateInShakingState(dt);
            break;
        case State::LastShake:
            mState = State::Inactive;
            mCameraTranslation = {0.0f, 0.0f, 0.0f};
            break;
        case State::Inactive:
            break;
    }
}

void CameraShake::UpdateInShakingState(float dt) {
    auto magnitude = mMagnitude * (mShakeTime - mElapsedTime) / mShakeTime;
    
    switch (mShakeKind) {
        case ShakeKind::Random:
            mCameraTranslation.x = magnitude * (NormalizedRand() - 0.5f);
            mCameraTranslation.y = magnitude * (NormalizedRand() - 0.5f);
            break;
        case ShakeKind::PerlinNoise: {
            mSamplePoint += speed * dt;
            auto noiseX {mFastNoise.GetPerlin(mSamplePoint, 0.0f)};
            auto noiseY {mFastNoise.GetPerlin(0.0f, mSamplePoint)};
            mCameraTranslation.x = magnitude * noiseX;
            mCameraTranslation.y = magnitude * noiseY;
            break;
        }
    }
    
    mCameraTranslation.z = 0.0f;
    
    mElapsedTime += dt;
    
    if (mElapsedTime >= mShakeTime) {
        mState = State::LastShake;
        mCameraTranslation = {0.0f, 0.0f, 0.0f};
    }
}

void CameraShake::StartShake(float shakeTime, float magnitude) {
    mElapsedTime = 0.0f;
    mShakeTime = shakeTime;
    mMagnitude = magnitude;
    
    if (mShakeKind == ShakeKind::PerlinNoise && mState != State::Shaking) {
        mSamplePoint = samplePointStart[std::rand() % samplePointStart.size()];
    }
    
    mState = State::Shaking;
}

bool CameraShake::IsShaking() const {
    switch (mState) {
        case State::Shaking:
        case State::LastShake:
            return true;
        case State::Inactive:
            return false;
    }
}
