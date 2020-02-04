#ifndef GLES3Handles_hpp
#define GLES3Handles_hpp

#define GLES_SILENCE_DEPRECATION

#include <OpenGLES/ES3/gl.h>

namespace Pht {
    struct GpuVertexBufferHandles {
        GLuint mGLVertexBufferHandle {0};
        GLuint mGLIndexBufferHandle {0};
    };
    
    struct TextureHandles {
        GLuint mGLHandle {0};
    };
}

#endif
