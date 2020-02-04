#ifndef BitmapImage_hpp
#define BitmapImage_hpp

#include <vector>
#include <memory>

#include "IImage.hpp"
#include "Noncopyable.hpp"

namespace Pht {
    class BitmapImage: public IImage, Noncopyable {
    public:
        BitmapImage(const std::vector<Vec4>& data, const IVec2& size);
        BitmapImage(const unsigned char* data, ImageFormat format, const IVec2& size);
        BitmapImage(ImageFormat format, const IVec2& size);
        
        ImageFormat GetFormat() const override;
        int GetBitsPerComponent() const override;
        IVec2 GetSize() const override;
        const void* GetImageData() const override;
        bool HasPremultipliedAlpha() const override;
        
        void Insert(const IImage& image, const IVec2& position);
        
        void SetHasPremultipliedAlpha(bool hasPremultipliedAlpha) {
            mHasPremultipliedAlpha = hasPremultipliedAlpha;
        }
    
    private:
        void CopyData(const unsigned char* sourceData, int bufferSize);
        unsigned char* CalcDataPtr(int x, int y, int numBytesPerPixel);

        ImageFormat mFormat {ImageFormat::Rgba};
        int mBitsPerComponent {8};
        IVec2 mSize {0, 0};
        std::unique_ptr<unsigned char[]> mData;
        bool mHasPremultipliedAlpha {false};
    };
}

#endif
