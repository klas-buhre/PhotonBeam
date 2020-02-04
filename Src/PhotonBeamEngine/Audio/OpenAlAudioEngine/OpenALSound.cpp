#include "OpenALSound.hpp"

using namespace Pht;

void OpenALSound::Play() {
    mCurrentSourceIndex = (mCurrentSourceIndex + 1) % mSources.size();
    mSources[mCurrentSourceIndex]->Play();
}

void OpenALSound::Stop() {
    for (auto& source: mSources) {
        source->Stop();
    }
}

void OpenALSound::SetGain(float gain) {
    for (auto& source: mSources) {
        source->SetGain(gain);
    }
}

void OpenALSound::SetPitch(float pitch) {
    for (auto& source: mSources) {
        source->SetPitch(pitch);
    }
}

void OpenALSound::SetLoop(bool loop) {
    for (auto& source: mSources) {
        source->SetLoop(loop);
    }
}

bool OpenALSound::IsPlaying() const {
    for (auto& source: mSources) {
        if (source->IsPlaying()) {
            return true;
        }
    }

    return false;
}

std::unique_ptr<OpenALSound> OpenALSound::Create(const std::string& filename, int maxSources) {
    auto buffer = OpenALBuffer::Create(filename);
    if (buffer == nullptr) {
        return nullptr;
    }
    
    auto sound = std::make_unique<OpenALSound>();

    for (auto i = 0; i < maxSources; ++i) {
        auto source = OpenALSource::Create(*buffer);
        if (source == nullptr) {
            return nullptr;
        }
        
        sound->mSources.push_back(std::move(source));
    }
    
    sound->mBuffer = std::move(buffer);
    return sound;
}
