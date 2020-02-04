#ifndef Audio_hpp
#define Audio_hpp

#include <memory>
#include <unordered_map>

#include "IAudio.hpp"
#include "ISound.hpp"
#include "IMusicTrack.hpp"
#include "IAudioEngine.hpp"
#include "StaticVector.hpp"

namespace Pht {
    class Audio: public IAudio {
    public:
        Audio();

        void LoadSound(const std::string& filename,
                       int maxSources,
                       float gain,
                       AudioResourceId resourceId) override;
        ISound* GetSound(AudioResourceId resourceId) override;
        void PlaySound(AudioResourceId resourceId) override;
        void PlaySoundWithDelay(AudioResourceId resourceId, float delay) override;
        void LoadMusicTrack(const std::string& filename, AudioResourceId resourceId) override;
        void FreeMusicTrack(AudioResourceId resourceId) override;
        IMusicTrack* GetMusicTrack(AudioResourceId resourceId) const override;
        void PlayMusicTrack(AudioResourceId resourceId, float fadeInDuration) override;
        void SetMusicVolume(float volume) override;
        void FadeOutActiveTrack(float fadeOutDuration) override;
        void EnableSound() override;
        void DisableSound() override;
        void EnableMusic() override;
        void DisableMusic() override;
        bool IsSoundEnabled() override;
        bool IsMusicEnabled() override;
        
        void Update(float dt);
        void OnAudioSessionInterrupted();
        void OnAudioSessionInterruptionEnded();
        void OnApplicationBecameActive();

    private:
        void ResumeAudio();
        
        struct DelayedSoundJob {
            float mElapsedTime {0.0f};
            float mDelay {0.0f};
            AudioResourceId mAudioResourceId;
        };
        
        std::unique_ptr<IAudioEngine> mAudioEngine;
        std::unordered_map<AudioResourceId, std::unique_ptr<ISound>> mSounds;
        std::unordered_map<AudioResourceId, std::unique_ptr<IMusicTrack>> mTracks;
        IMusicTrack* mActiveTrack {nullptr};
        float mMusicVolume {1.0f};
        bool mIsSoundEnabled {true};
        bool mIsMusicEnabled {true};
        StaticVector<DelayedSoundJob, 100> mDelayedSoundJobs;
    };
}

#endif
