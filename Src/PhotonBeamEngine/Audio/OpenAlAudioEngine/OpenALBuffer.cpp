#include "OpenALBuffer.hpp"

#include <iostream>

#include <OpenAl/alc.h>

#include "AudioFileDecoder.hpp"

using namespace Pht;

namespace {
    ALenum ToOpenALFormat(int numberOfChannels, int bitsPerChannel) {
        if (numberOfChannels == 1 && bitsPerChannel == 8) {
            return AL_FORMAT_MONO8;
        } else if (numberOfChannels == 1 && bitsPerChannel == 16) {
            return AL_FORMAT_MONO16;
        } if (numberOfChannels == 2 && bitsPerChannel == 8) {
            return AL_FORMAT_STEREO8;
        } else if (numberOfChannels == 2 && bitsPerChannel == 16) {
            return AL_FORMAT_STEREO16;
        } else {
            assert(!"Invalid format.");
        }
    }
}

OpenALBuffer::OpenALBuffer(ALuint handle) : mHandle {handle} {}

OpenALBuffer::~OpenALBuffer() {
    alDeleteBuffers(1, &mHandle);
}

std::unique_ptr<OpenALBuffer> OpenALBuffer::Create(const std::string& filename) {
    auto audioData = Pht::DecodeAudioFile(filename);
    if (audioData == nullptr) {
        return nullptr;
    }
    
    if (!alcGetCurrentContext()) {
        std::cout << "OpenALBuffer: ERROR: No context." << std::endl;
        return nullptr;
    }
    
    ALenum error {ALC_NO_ERROR};
    ALuint handle {0};
    
    alGetError();
    alGenBuffers(1, &handle);
    error = alGetError();
    if (error) {
        std::cout << "OpenALBuffer: ERROR: Could not create OpenAL buffer." << std::endl;
        return nullptr;
    }

    alGetError();
    alBufferData(handle,
                 ToOpenALFormat(audioData->mNumberOfChannels, audioData->mBitsPerChannel),
                 audioData->mSampleData.data(),
                 static_cast<ALsizei>(audioData->mSampleData.size()),
                 audioData->mSampleRate);
    error = alGetError();
    if (error) {
        std::cout << "OpenALBuffer: ERROR: Could not copy samples to OpenAL buffer." << std::endl;
        return nullptr;
    }

    return std::make_unique<OpenALBuffer>(handle);
}
