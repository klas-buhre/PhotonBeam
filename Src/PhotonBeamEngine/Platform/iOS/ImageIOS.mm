#include "IImage.hpp"

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>

#import <string>

namespace {
    bool StringEndsWith(const std::string& str, const std::string& with) {
        auto pos = str.find(with);
        
        if (pos == std::string::npos) {
            return false;
        }
        
        return pos == str.size() - with.size();
    }

    class ImageIOS: public Pht::IImage {
    public:
        ImageIOS(const std::string& filename) {
            LoadGenericImage(filename);
            
            if (StringEndsWith(filename, ".png")) {
                mHasPremultipliedAlpha = true;
            }
        }
        
        Pht::ImageFormat GetFormat() const override {
            return mFormat;
        }
        
        int GetBitsPerComponent() const override {
            return mBitsPerComponent;
        }
        
        Pht::IVec2 GetSize() const override {
            return mSize;
        }

        const void* GetImageData() const override {
            return (void*) [mImageData bytes];
        }
        
        bool HasPremultipliedAlpha() const override {
            return mHasPremultipliedAlpha;
        }
        
    private:
        void LoadGenericImage(const std::string& filename) {
            NSString* basePath = [NSString stringWithUTF8String:filename.c_str()];
            NSString* resourcePath = [[NSBundle mainBundle] resourcePath];
            NSString* fullPath = [resourcePath stringByAppendingPathComponent:basePath];
            UIImage* uiImage = [UIImage imageWithContentsOfFile:fullPath];
            
            mSize.x = static_cast<int>(CGImageGetWidth(uiImage.CGImage));
            mSize.y = static_cast<int>(CGImageGetHeight(uiImage.CGImage));
            mBitsPerComponent = 8;
            mFormat = Pht::ImageFormat::Rgba;

            int bpp = mBitsPerComponent / 2;
            int byteCount = mSize.x * mSize.y * bpp;
            unsigned char* data = (unsigned char*) calloc(byteCount, 1);
            
            CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
            CGBitmapInfo bitmapInfo =  kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big;
            CGContextRef context = CGBitmapContextCreate(data,
                                                         mSize.x,
                                                         mSize.y,
                                                         mBitsPerComponent,
                                                         bpp * mSize.x,
                                                         colorSpace,
                                                         bitmapInfo);
            CGColorSpaceRelease(colorSpace);
            CGRect rect = CGRectMake(0, 0, mSize.x, mSize.y);
            CGContextDrawImage(context, rect, uiImage.CGImage);
            CGContextRelease(context);
            
            mImageData = [NSData dataWithBytesNoCopy:data length:byteCount freeWhenDone:YES];
        }

        void LoadPng(const std::string& filename) {
            NSString* basePath = [NSString stringWithUTF8String:filename.c_str()];
            NSString* resourcePath = [[NSBundle mainBundle] resourcePath];
            NSString* fullPath = [resourcePath stringByAppendingPathComponent:basePath];
            
            UIImage* uiImage = [UIImage imageWithContentsOfFile:fullPath];
            CGImageRef cgImage = uiImage.CGImage;
            
            CFDataRef dataRef = CGDataProviderCopyData(CGImageGetDataProvider(cgImage));
            mImageData = [NSData dataWithData:(__bridge NSData*) dataRef];
            CFRelease(dataRef);
            
            mSize.x = static_cast<int>(CGImageGetWidth(cgImage));
            mSize.y = static_cast<int>(CGImageGetHeight(cgImage));
            
            bool hasAlpha = CGImageGetAlphaInfo(cgImage) != kCGImageAlphaNone;
            CGColorSpaceRef colorSpace = CGImageGetColorSpace(cgImage);
            
            CGColorSpaceModel colorSpaceModel = CGColorSpaceGetModel(colorSpace);
            switch (colorSpaceModel) {
                case kCGColorSpaceModelMonochrome:
                    mFormat = hasAlpha ? Pht::ImageFormat::GrayAlpha : Pht::ImageFormat::Gray;
                    break;
                case kCGColorSpaceModelRGB:
                    mFormat = hasAlpha ? Pht::ImageFormat::Rgba : Pht::ImageFormat::Rgb;
                    break;
                default:
                    assert(!"Unsupported color space.");
                    break;
            }
            
            mBitsPerComponent = static_cast<int>(CGImageGetBitsPerComponent(cgImage));
        }
        
        Pht::ImageFormat mFormat;
        int mBitsPerComponent;
        Pht::IVec2 mSize;
        NSData* mImageData;
        bool mHasPremultipliedAlpha {false};
    };
}

std::unique_ptr<Pht::IImage> Pht::LoadImage(const std::string& filename) {
    return std::make_unique<ImageIOS>(filename);
}
