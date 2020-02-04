#include "OpenALContext.hpp"

#include <iostream>

using namespace Pht;

OpenALContext::OpenALContext(ALCdevice *device) {
    if (device) {
        mHandle = alcCreateContext(device, 0);
    }
    
    if (mHandle == nullptr) {
        std::cout << "OpenALContext: ERROR: Could not create context." << std::endl;
    }
}

OpenALContext::~OpenALContext() {
    if (mHandle) {
        if (IsCurrent()) {
            SetIsCurrent(false);
        }
        
        alcDestroyContext(mHandle);
    }
}

void OpenALContext::SetIsCurrent(bool isCurrent) {
    alcMakeContextCurrent(isCurrent ? mHandle : nullptr);
}

bool OpenALContext::IsCurrent() const {
    return mHandle && (alcGetCurrentContext() == mHandle);
}

void OpenALContext::SetIsSuspended(bool isSuspended) {
    if (mHandle && (isSuspended != mIsSuspended)) {
        if (isSuspended) {
            alcSuspendContext(mHandle);
        } else {
            alcProcessContext(mHandle);
        }
        
        mIsSuspended = isSuspended;
    }
}
