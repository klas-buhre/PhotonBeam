#ifndef ParticleEmitter_hpp
#define ParticleEmitter_hpp

#include <vector>
#include <functional>

#include "Vector.hpp"
#include "Optional.hpp"

namespace Pht {
    struct EmitterSettings {
        Vec3 mPosition {0.0f, 0.0f, 0.0f};
        Vec3 mSize {0.0f, 0.0f, 0.0f};
        float mTimeToLive {2.0f};
        float mFrequency {10.0f};
        int mBurst {0};
    };
    
    struct ParticleSettings {
        std::function<Vec3()> mVelocityFunction;
        Vec3 mVelocity {0.0f, 0.0f, 0.0f};
        Vec3 mVelocityRandomPart {0.0f, 0.0f, 0.0f};
        Vec3 mAcceleration {0.0f, 0.0f, 0.0f};
        Optional<float> mDragCoefficient;
        Vec4 mColor {1.0f, 1.0f, 1.0f, 1.0f};
        Vec4 mColorRandomPart {1.0f, 1.0f, 1.0f, 1.0f};
        std::string mTextureFilename;
        float mTimeToLive {2.0f};
        float mTimeToLiveRandomPart {0.5f};
        float mFadeOutDuration {1.0f};
        float mZAngularVelocity {0.0f};
        float mZAngularVelocityRandomPart {0.0f};
        Optional<Vec2> mSize;
        Optional<float> mSizeRandomPart;
        Optional<Vec2> mInitialSize;
        Optional<int> mPointSize;            // In pixels.
        Optional<int> mPointSizeRandomPart;  // In pixels.
        Optional<int> mInitialPointSize;     // In pixels.
        float mGrowDuration {0.1f};
        float mShrinkDuration {1.0f};
        Vec2 mShrinkScale {1.0f, 1.0f};
    };

    struct Particle {
        Vec3 mPosition;
        Vec3 mVelocity;
        Vec4 mColor;
        Vec2 mSize;
        Vec2 mFullSize;
        float mZAngle;
        float mZAngularVelocity;
        float mAge;
        float mTimeToLive;
        bool mIsActive {false};
    };
    
    class ParticleEmitter {
    public:
        ParticleEmitter(const ParticleSettings& particleSettings,
                        const EmitterSettings& emitterSettings);
        
        void Start();
        void Update(float dt, std::vector<Particle>& particles);
        bool IsActive() const;
        
        ParticleSettings& GetParticleSettings() {
            return mParticleSettings;
        }
        
    private:
        Particle* FindInactiveParticle(std::vector<Particle>& particles) const;
        void EmitParticle(Particle& particle);
        void EmitBurst(std::vector<Particle>& particles);
    
        ParticleSettings mParticleSettings;
        EmitterSettings mEmitterSettings;
        float mAge {0.0f};
        float mTimeSinceLastSpawn {0.0f};
    };
}

#endif
