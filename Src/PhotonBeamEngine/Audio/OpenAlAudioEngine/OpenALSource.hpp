#ifndef OpenALSource_hpp
#define OpenALSource_hpp

#include <memory>

#include <OpenAl/al.h>

namespace Pht {
    class OpenALBuffer;

    class OpenALSource {
    public:
        static std::unique_ptr<OpenALSource> Create(const OpenALBuffer& buffer);
    
        ~OpenALSource();
        
        void Play();
        void Stop();
        void SetGain(float gain);
        void SetPitch(float pitch);
        void SetLoop(bool loop);
        bool IsPlaying() const;
        
        ALuint GetHandle() const {
            return mHandle;
        }
        
        explicit OpenALSource(ALuint handle);
        
    private:
        ALuint mHandle {0};
    };
}

#endif
