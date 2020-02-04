#include "IMusicTrack.hpp"

#include <iostream>

#import <AVFoundation/AVFoundation.h>

namespace {
    class MusicTrackIOS: public Pht::IMusicTrack {
    public:
        explicit MusicTrackIOS(const std::string& filename) {
            NSString* basePath = [NSString stringWithUTF8String:filename.c_str()];
            NSString* resourcePath = [[NSBundle mainBundle] resourcePath];
            NSString* fullPath = [resourcePath stringByAppendingPathComponent:basePath];
            NSURL* fileUrl = [NSURL fileURLWithPath:fullPath];
            NSError* error = nil;
            
            mAudioPlayer = [[AVAudioPlayer alloc] initWithContentsOfURL:fileUrl error:&error];
            
            if (error) {
                std::cout << "MusicTrackIOS: ERROR: Could not load " << filename << std::endl;
            }
            
            [mAudioPlayer setNumberOfLoops:-1];
            [mAudioPlayer prepareToPlay];
        }

        void Play() override {
            [mAudioPlayer play];
        }
        
        void Pause() override {
            [mAudioPlayer pause];
        }
        
        void Stop() override {
            Pause();
            mAudioPlayer.currentTime = 0.0f;
        }

        void SetVolume(float volume) override {
            mAudioPlayer.volume = volume;
        }

        void SetVolume(float volume, float fadeDuration) override {
            [mAudioPlayer setVolume:volume fadeDuration:fadeDuration];
        }

    private:
        AVAudioPlayer* mAudioPlayer;
    };
}

std::unique_ptr<Pht::IMusicTrack> Pht::LoadMusicTrack(const std::string& filename) {
    return std::make_unique<MusicTrackIOS>(filename);
}
