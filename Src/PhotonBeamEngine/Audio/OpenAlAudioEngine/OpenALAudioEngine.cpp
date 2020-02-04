#include "IAudioEngine.hpp"

#include <iostream>

#include <OpenAl/al.h>
#include <OpenAl/alc.h>

#include "OpenALContext.hpp"
#include "OpenALSound.hpp"

using namespace Pht;

namespace {
    class OpenALDevice {
    public:
        OpenALDevice() : mHandle {alcOpenDevice(nullptr)} {
            if (mHandle == nullptr) {
                std::cout << "OpenALDevice: ERROR: Could not open device." << std::endl;
            }
        }
        
        ~OpenALDevice() {
            if (mHandle) {
                alcCloseDevice(mHandle);
            }
        }
        
        ALCdevice* GetHandle() {
            return mHandle;
        }
        
    private:
        ALCdevice* mHandle {nullptr};
    };
    
    class OpenALAudioEngine: public IAudioEngine {
    public:
        OpenALAudioEngine() :
            mDevice {std::make_unique<OpenALDevice>()},
            mContext {std::make_unique<OpenALContext>(mDevice->GetHandle())} {
            
            mContext->SetIsCurrent(true);
        }
        
        std::unique_ptr<ISound> LoadSound(const std::string& filename, int maxSources) override {
            return OpenALSound::Create(filename, maxSources);
        }
        
        void SetIsSuspended(bool isSuspended) override {
            if (isSuspended != mContext->IsSuspended()) {
                mContext->SetIsSuspended(isSuspended);
                
                if (isSuspended) {
                    mContext->SetIsCurrent(false);
                } else {
                    mContext->SetIsCurrent(true);
                }
            }
        }
    
        bool IsSuspended() const override {
            return mContext->IsSuspended();
        }

    private:
        std::unique_ptr<OpenALDevice> mDevice;
        std::unique_ptr<OpenALContext> mContext;
    };
}

std::unique_ptr<Pht::IAudioEngine> Pht::CreateAudioEngine() {
    return std::make_unique<OpenALAudioEngine>();
}
