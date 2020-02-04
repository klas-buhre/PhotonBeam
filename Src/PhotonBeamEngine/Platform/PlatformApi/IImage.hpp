#ifndef IImage_hpp
#define IImage_hpp

#include <memory>

#include "Vector.hpp"

namespace Pht {
    enum class ImageFormat {
        Rgb,
        Rgba,
        GrayAlpha,
        Gray,
        Alpha
    };
    
    class IImage {
    public:
        virtual ~IImage() {}
        
        virtual ImageFormat GetFormat() const = 0;
        virtual int GetBitsPerComponent() const = 0;
        virtual IVec2 GetSize() const = 0;
        virtual const void* GetImageData() const = 0;
        virtual bool HasPremultipliedAlpha() const = 0;
    };
    
    std::unique_ptr<Pht::IImage> LoadImage(const std::string& filename);
}

#endif
