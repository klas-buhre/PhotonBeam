#ifndef AudioFileDecoder_hpp
#define AudioFileDecoder_hpp

#include <vector>
#include <memory>
#include <string>

namespace Pht {
    struct DecodedAudioData {
        std::vector<unsigned char> mSampleData;
        int mSampleRate {0};
        int mNumberOfChannels {0};
        int mBitsPerChannel {0};
    };
    
    std::unique_ptr<DecodedAudioData> DecodeAudioFile(const std::string& filename);
}

#endif
