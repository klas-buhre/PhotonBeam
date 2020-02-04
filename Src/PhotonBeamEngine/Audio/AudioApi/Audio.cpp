#include "Audio.hpp"

#include <iostream>

#include "ISound.hpp"

using namespace Pht;

Audio::Audio() {
    std::cout << "Pht::Audio: Initializing..." << std::endl;
    
    mAudioEngine = CreateAudioEngine();
}

void Audio::LoadSound(const std::string& filename,
                      int maxSources,
                      float gain,
                      AudioResourceId resourceId) {
    auto sound = mAudioEngine->LoadSound(filename, maxSources);
    if (sound) {
        sound->SetGain(gain);
        mSounds[resourceId] = std::move(sound);
    }
}

ISound* Audio::GetSound(AudioResourceId resourceId) {
    auto i = mSounds.find(resourceId);
    if (i != std::end(mSounds)) {
        return i->second.get();
    }
    
    return nullptr;
}

void Audio::PlaySound(AudioResourceId resourceId) {
    if (!mIsSoundEnabled) {
        return;
    }
    
    auto* sound = GetSound(resourceId);
    if (sound) {
        sound->Play();
    }
}

void Audio::PlaySoundWithDelay(AudioResourceId resourceId, float delay) {
    if (mDelayedSoundJobs.Size() >= mDelayedSoundJobs.GetCapacity()) {
        return;
    }

    DelayedSoundJob job {.mElapsedTime = 0.0f, .mDelay = delay, .mAudioResourceId = resourceId};
    mDelayedSoundJobs.PushBack(job);
}

void Audio::LoadMusicTrack(const std::string& filename, AudioResourceId resourceId) {
    auto track = Pht::LoadMusicTrack(filename);
    mTracks[resourceId] = std::move(track);
}

void Audio::FreeMusicTrack(AudioResourceId resourceId) {
    mTracks.erase(resourceId);
}

IMusicTrack* Audio::GetMusicTrack(AudioResourceId resourceId) const {
    auto i = mTracks.find(resourceId);
    if (i != std::end(mTracks)) {
        return i->second.get();
    }
    
    return nullptr;
}

void Audio::PlayMusicTrack(AudioResourceId resourceId, float fadeInDuration) {
    auto* track = GetMusicTrack(resourceId);
    if (track == nullptr) {
        return;
    }

    if (mActiveTrack) {
        mActiveTrack->Stop();
    }

    mActiveTrack = track;

    if (mIsMusicEnabled) {
        if (fadeInDuration == 0.0f) {
            track->SetVolume(mMusicVolume);
            track->Play();
        } else {
            track->SetVolume(0.0f);
            track->Play();
            track->SetVolume(mMusicVolume, fadeInDuration);
        }
    }
}

void Audio::SetMusicVolume(float volume) {
    mMusicVolume = volume;
    
    if (mActiveTrack) {
        mActiveTrack->SetVolume(volume);
    }
}

void Audio::FadeOutActiveTrack(float fadeOutDuration) {
    if (mActiveTrack) {
        mActiveTrack->SetVolume(0.0f, fadeOutDuration);
    }
}

void Audio::EnableSound() {
    mIsSoundEnabled = true;
}

void Audio::DisableSound()  {
    mIsSoundEnabled = false;
}

void Audio::EnableMusic()  {
    mIsMusicEnabled = true;
    
    if (mActiveTrack) {
        mActiveTrack->SetVolume(mMusicVolume);
        mActiveTrack->Play();
    }
}

void Audio::DisableMusic()  {
    mIsMusicEnabled = false;
    
    if (mActiveTrack) {
        mActiveTrack->Pause();
    }
}

bool Audio::IsSoundEnabled()  {
    return mIsSoundEnabled;
}

bool Audio::IsMusicEnabled()  {
    return mIsMusicEnabled;
}

void Audio::Update(float dt) {
    for (auto i = 0; i < mDelayedSoundJobs.Size();) {
        auto& job = mDelayedSoundJobs.At(i);
        
        job.mElapsedTime += dt;
        if (job.mElapsedTime >= job.mDelay) {
            PlaySound(job.mAudioResourceId);
            mDelayedSoundJobs.Erase(i);
        } else {
            ++i;
        }
    }
}

void Audio::OnAudioSessionInterrupted() {
    std::cout << "Pht::Audio: Session interrupted." << std::endl;
    
    mAudioEngine->SetIsSuspended(true);
}

void Audio::OnAudioSessionInterruptionEnded() {
    std::cout << "Pht::Audio: Session interruption ended." << std::endl;
    
    ResumeAudio();
}

void Audio::OnApplicationBecameActive() {
    if (mAudioEngine->IsSuspended()) {
        std::cout << "Pht::Audio: Interruption ended while application was inactive." << std::endl;
    
        ResumeAudio();
    }
}

void Audio::ResumeAudio() {
    std::cout << "Pht::Audio: Resuming audio." << std::endl;
    
    mAudioEngine->SetIsSuspended(false);
    
    if (mIsMusicEnabled && mActiveTrack) {
        mActiveTrack->SetVolume(0.0f);
        mActiveTrack->Play();
        mActiveTrack->SetVolume(mMusicVolume, 1.0f);
    }
}
