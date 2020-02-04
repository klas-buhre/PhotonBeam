#ifndef IMusicTrack_hpp
#define IMusicTrack_hpp

#include <memory>
#include <string>

namespace Pht {
    class IMusicTrack {
    public:
        virtual ~IMusicTrack() {}

        virtual void Play() = 0;
        virtual void Pause() = 0;
        virtual void Stop() = 0;
        virtual void SetVolume(float volume) = 0;
        virtual void SetVolume(float volume, float fadeDuration) = 0;
    };
    
    std::unique_ptr<IMusicTrack> LoadMusicTrack(const std::string& filename);
}

#endif
