#ifndef OpenALSound_hpp
#define OpenALSound_hpp

#include <memory>
#include <vector>

#include "ISound.hpp"
#include "OpenALBuffer.hpp"
#include "OpenALSource.hpp"

namespace Pht {
    class OpenALSound: public ISound {
    public:
        static std::unique_ptr<OpenALSound> Create(const std::string& filename, int maxSources);
        
        void Play() override;
        void Stop() override;
        void SetGain(float gain) override;
        void SetPitch(float pitch) override;
        void SetLoop(bool loop) override;
        bool IsPlaying() const override;

    private:
        std::unique_ptr<OpenALBuffer> mBuffer;
        std::vector<std::unique_ptr<OpenALSource>> mSources;
        int mCurrentSourceIndex {0};
    };
}

#endif
