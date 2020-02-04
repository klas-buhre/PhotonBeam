#ifndef OpenALContext_hpp
#define OpenALContext_hpp

#include <OpenAl/alc.h>

namespace Pht {
    class OpenALContext {
    public:
        explicit OpenALContext(ALCdevice *device);
        ~OpenALContext();
        
        void SetIsCurrent(bool isCurrent);
        void SetIsSuspended(bool isSuspended);
        
        bool IsSuspended() const {
            return mIsSuspended;
        }
        
    private:
        bool IsCurrent() const;
        
        ALCcontext* mHandle {nullptr};
        bool mIsSuspended {false};
    };
}

#endif
