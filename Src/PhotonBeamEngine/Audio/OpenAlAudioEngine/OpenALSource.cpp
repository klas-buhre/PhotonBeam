#include "OpenALSource.hpp"

#include <iostream>

#include "OpenALBuffer.hpp"

using namespace Pht;

OpenALSource::OpenALSource(ALuint handle) : mHandle {handle} {}

OpenALSource::~OpenALSource() {
    if (IsPlaying()) {
        Stop();
    }
    
    alSourcei(mHandle, AL_BUFFER, 0 /* detach */);
    alDeleteSources(1, &mHandle);
}

void OpenALSource::Play() {
    if (IsPlaying()) {
        Stop();
    }
    
    alSourcePlay(mHandle);
}

void OpenALSource::Stop() {
    if (IsPlaying()) {
        alSourceStop(mHandle);
    }
}

bool OpenALSource::IsPlaying() const {
    ALint state;
    alGetSourcei(mHandle, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

void OpenALSource::SetGain(float gain) {
    alSourcef(mHandle, AL_GAIN, gain);
}

void OpenALSource::SetPitch(float pitch) {
    alSourcef(mHandle, AL_PITCH, pitch);
}

void OpenALSource::SetLoop(bool loop) {
    alSourcei(mHandle, AL_LOOPING, loop);
}

std::unique_ptr<OpenALSource> OpenALSource::Create(const OpenALBuffer& buffer) {
    ALuint handle {0};
    
    alGetError();
    alGenSources(1, &handle);
    alSourcei(handle, AL_BUFFER, buffer.GetHandle());
    ALenum error {alGetError()};
    if (error) {
        std::cout << "OpenALBuffer: ERROR: Could not create OpenAL source." << std::endl;
        return nullptr;
    }
    
    return std::make_unique<OpenALSource>(handle);
}
