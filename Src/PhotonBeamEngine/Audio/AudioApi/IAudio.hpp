#ifndef IAudio_hpp
#define IAudio_hpp

#include <string>

namespace Pht {
    class ISound;
    class IMusicTrack;
    
    using AudioResourceId = uint32_t;

    class IAudio {
    public:
        virtual ~IAudio() {}

        virtual void LoadSound(const std::string& filename,
                               int maxSources,
                               float gain,
                               AudioResourceId resourceId) = 0;
        virtual ISound* GetSound(AudioResourceId resourceId) = 0;
        virtual void PlaySound(AudioResourceId resourceId) = 0;
        virtual void PlaySoundWithDelay(AudioResourceId resourceId, float delay) = 0;
        virtual void LoadMusicTrack(const std::string& filename, AudioResourceId resourceId) = 0;
        virtual void FreeMusicTrack(AudioResourceId resourceId) = 0;
        virtual IMusicTrack* GetMusicTrack(AudioResourceId resourceId) const = 0;
        virtual void PlayMusicTrack(AudioResourceId resourceId, float fadeInDuration) = 0;
        virtual void SetMusicVolume(float volume) = 0;
        virtual void FadeOutActiveTrack(float fadeOutDuration) = 0;
        virtual void EnableSound() = 0;
        virtual void DisableSound() = 0;
        virtual void EnableMusic() = 0;
        virtual void DisableMusic() = 0;
        virtual bool IsSoundEnabled() = 0;
        virtual bool IsMusicEnabled() = 0;
    };
}

#endif
