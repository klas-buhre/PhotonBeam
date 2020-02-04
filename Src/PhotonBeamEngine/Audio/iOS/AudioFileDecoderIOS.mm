#include "AudioFileDecoder.hpp"

#include <iostream>

#include <AudioToolBox/AudioToolBox.h>

using namespace Pht;

namespace {
    class AudioFileGuard {
    public:
        explicit AudioFileGuard(AudioFileID fileId) : mFileId {fileId} {}
        
        ~AudioFileGuard() {
            AudioFileClose(mFileId);
        }
        
    private:
        AudioFileID mFileId;
    };

    bool IsFormatOk(const AudioStreamBasicDescription& format, const std::string& filename) {
        if (format.mFormatID != kAudioFormatLinearPCM) {
            std::cout << "AudioFileDecoderIOS: ERROR: Audio format is not linear PCM. File: "
                      << filename << std::endl;
            return false;
        }
        
        if (!TestAudioFormatNativeEndian(format)) {
            std::cout << "AudioFileDecoderIOS: ERROR: Invalid sample endianess. File: "
                      << filename << std::endl;
            return false;
        }

        if (format.mChannelsPerFrame != 1 && format.mChannelsPerFrame != 2) {
            std::cout << "AudioFileDecoderIOS: ERROR: Invalid number of sound channels. File: "
                      << filename << std::endl;
            return false;
        }

        if (format.mBitsPerChannel != 8 && format.mBitsPerChannel != 16) {
            std::cout << "AudioFileDecoderIOS: ERROR: Invalid sample resolution. File: "
                      << filename << std::endl;
            return false;
        }
        
        return true;
    }
}

std::unique_ptr<DecodedAudioData> Pht::DecodeAudioFile(const std::string& filename) {
    NSString* basePath = [NSString stringWithUTF8String:filename.c_str()];
    NSString* resourcePath = [[NSBundle mainBundle] resourcePath];
    NSString* fullPath = [resourcePath stringByAppendingPathComponent:basePath];
    NSURL* fileUrl = [NSURL fileURLWithPath:fullPath];
    
    OSStatus error = noErr;
    UInt32 propertySize;
    AudioFileID fileId = 0;

    error = AudioFileOpenURL((__bridge CFURLRef) fileUrl, kAudioFileReadPermission, 0, &fileId);
    if (error) {
        std::cout << "AudioFileDecoderIOS: ERROR: Could not read file " << filename << std::endl;
        return nullptr;
    }
    
    AudioFileGuard guard {fileId};

    AudioStreamBasicDescription format;
    propertySize = sizeof(format);
    error = AudioFileGetProperty(fileId, kAudioFilePropertyDataFormat, &propertySize, &format);
    if (error) {
        std::cout << "AudioFileDecoderIOS: ERROR: Could not read format. File: "
                  << filename << std::endl;
        return nullptr;
    }
    
    if (!IsFormatOk(format, filename)) {
        return nullptr;
    }
    
    auto decodedAudioData = std::make_unique<DecodedAudioData>();
    decodedAudioData->mSampleRate = format.mSampleRate;
    decodedAudioData->mNumberOfChannels = format.mChannelsPerFrame;
    decodedAudioData->mBitsPerChannel = format.mBitsPerChannel;

    UInt64 fileSize = 0;
    propertySize = sizeof(fileSize);
    error = AudioFileGetProperty(fileId,
                                 kAudioFilePropertyAudioDataByteCount,
                                 &propertySize,
                                 &fileSize);
    if (error) {
        std::cout << "AudioFileDecoderIOS: ERROR: Could not read byte count. File: "
                  << filename << std::endl;
        return nullptr;
    }

    auto dataSize = static_cast<UInt32>(fileSize);
    decodedAudioData->mSampleData.resize(dataSize);

    error = AudioFileReadBytes(fileId, false, 0, &dataSize, &decodedAudioData->mSampleData[0]);
    if (error) {
        std::cout << "AudioFileDecoderIOS: ERROR: Could not read audio data. File: "
                  << filename << std::endl;
        return nullptr;

    }

    return decodedAudioData;
}
