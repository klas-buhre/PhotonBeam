#include "BitmapImage.hpp"

using namespace Pht;

namespace {
    unsigned char MapColorToUChar(float color) {
        if (color < 0.0f) {
            return 0;
        }
        
        if (color > 1.0f) {
            return 255;
        }
        
        return static_cast<unsigned char>(color * 255.0f);
    }
    
    int GetNumBytesPerPixel(ImageFormat format) {
        switch (format) {
            case ImageFormat::Rgb:
                return 3;
            case ImageFormat::Rgba:
                return 4;
            case ImageFormat::GrayAlpha:
                return 2;
            case ImageFormat::Gray:
            case ImageFormat::Alpha:
                return 1;
        }
    }

    int CalcBufferSize(ImageFormat format, const IVec2& size) {
        return GetNumBytesPerPixel(format) * size.x * size.y;
    }
    
    bool SubImageFits(const IVec2& subImagePosition,
                      const IVec2& subImageSize,
                      const IVec2& atlasSize) {
        return subImagePosition.x >= 0 && subImagePosition.x + subImageSize.x <= atlasSize.x &&
               subImagePosition.y >= 0 && subImagePosition.y + subImageSize.y <= atlasSize.y;
    }
}

BitmapImage::BitmapImage(const std::vector<Vec4>& buffer, const IVec2& size) :
    mSize {size} {
    
    auto numPixels = size.x * size.y;
    assert(numPixels == buffer.size());

    mData = std::make_unique<unsigned char[]>(numPixels * 4);
    auto* data = &mData[0];

    for (const auto& pixel: buffer) {
        *data++ = MapColorToUChar(pixel.x);
        *data++ = MapColorToUChar(pixel.y);
        *data++ = MapColorToUChar(pixel.z);
        *data++ = MapColorToUChar(pixel.w);
    }
}

BitmapImage::BitmapImage(const unsigned char* data, ImageFormat format, const IVec2& size) :
    mFormat {format},
    mSize {size} {
    
    auto bufferSize = CalcBufferSize(format, size);
    mData = std::make_unique<unsigned char[]>(bufferSize);
    CopyData(data, bufferSize);
}

BitmapImage::BitmapImage(ImageFormat format, const IVec2& size) :
    mFormat {format},
    mSize {size} {

    auto bufferSize = CalcBufferSize(format, size);
    mData = std::make_unique<unsigned char[]>(bufferSize);
}

void BitmapImage::CopyData(const unsigned char* sourceData, int bufferSize) {
    auto* writePtr = &mData[0];
    auto* readPtr = sourceData;
    auto* readEnd = sourceData + bufferSize;
    while (readPtr < readEnd) {
        *writePtr++ = *readPtr++;
    }
}

ImageFormat BitmapImage::GetFormat() const {
    return mFormat;
}

int BitmapImage::GetBitsPerComponent() const {
    return mBitsPerComponent;
}

IVec2 BitmapImage::GetSize() const {
    return mSize;
}

const void* BitmapImage::GetImageData() const {
    return mData.get();
}

bool BitmapImage::HasPremultipliedAlpha() const {
    return mHasPremultipliedAlpha;
}

void BitmapImage::Insert(const IImage& subImage, const IVec2& subImagePosition) {
    if (GetFormat() != subImage.GetFormat() ||
        GetBitsPerComponent() != subImage.GetBitsPerComponent() ||
        HasPremultipliedAlpha() != subImage.HasPremultipliedAlpha()) {
        
        assert(false);
        return;
    }

    auto subImageSize = subImage.GetSize();
    if (!SubImageFits(subImagePosition, subImageSize, mSize)) {
        assert(false);
        return;
    }

    auto subImageTopLeft = subImagePosition + IVec2{0, subImageSize.y - 1};
    auto subImageBottomRight = subImagePosition + IVec2{subImageSize.x - 1, 0};
    auto numBytesPerPixel = GetNumBytesPerPixel(mFormat);
    const unsigned char* readPtr = static_cast<const unsigned char*>(subImage.GetImageData());
    for (auto y = subImageTopLeft.y; y >= subImageBottomRight.y; --y) {
        auto* writePtr = CalcDataPtr(subImageTopLeft.x, y, numBytesPerPixel);
        auto* writeEnd = CalcDataPtr(subImageBottomRight.x, y, numBytesPerPixel) + numBytesPerPixel;
        while (writePtr < writeEnd) {
            *writePtr++ = *readPtr++;
        }
    }
}

unsigned char* BitmapImage::CalcDataPtr(int x, int y, int numBytesPerPixel) {
    if (x < 0 || x >= mSize.x || y < 0 || y >= mSize.y) {
        assert(false);
        return nullptr;
    }

    auto offset = numBytesPerPixel * ((mSize.y - y - 1) * mSize.x + x);
    auto* data = &mData[0];
    return data + offset;
}

