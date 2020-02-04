#ifndef OpenALBuffer_hpp
#define OpenALBuffer_hpp

#include <memory>

#include <OpenAl/al.h>

namespace Pht {
    class OpenALBuffer {
    public:
        static std::unique_ptr<OpenALBuffer> Create(const std::string& filename);

        ~OpenALBuffer();
        
        ALuint GetHandle() const {
            return mHandle;
        }
        
        explicit OpenALBuffer(ALuint handle);
        
    private:
        ALuint mHandle {0};
    };
}

#endif
