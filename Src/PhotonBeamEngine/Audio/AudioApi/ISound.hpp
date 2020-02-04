#ifndef ISound_hpp
#define ISound_hpp

#include <memory>

namespace Pht {
    class ISound {
    public:
        virtual ~ISound() {}

        virtual void Play() = 0;
        virtual void Stop() = 0;
        virtual void SetGain(float gain) = 0;
        virtual void SetPitch(float pitch) = 0;
        virtual void SetLoop(bool loop) = 0;
        virtual bool IsPlaying() const = 0;
    };
}

#endif
