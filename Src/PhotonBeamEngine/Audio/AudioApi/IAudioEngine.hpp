#ifndef IAudioEngine_hpp
#define IAudioEngine_hpp

#include <memory>
#include <string>

#include "ISound.hpp"

namespace Pht {
    class IAudioEngine {
    public:
        virtual ~IAudioEngine() {}

        virtual std::unique_ptr<ISound> LoadSound(const std::string& filename, int maxSources) = 0;
        virtual void SetIsSuspended(bool isSuspended) = 0;
        virtual bool IsSuspended() const = 0;
    };
    
    std::unique_ptr<IAudioEngine> CreateAudioEngine();
}

#endif
