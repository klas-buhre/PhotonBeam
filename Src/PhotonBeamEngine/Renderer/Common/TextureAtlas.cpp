#include "TextureAtlas.hpp"

#include <assert.h>

#include "IImage.hpp"
#include "Optional.hpp"
#include "BitmapImage.hpp"

using namespace Pht;

namespace {
    constexpr auto maxSize = 2048;
    constexpr auto minSize = 32;
    
    struct SubTexture {
        IVec2 mPosition;
        const IImage* mImage;
    };
    
    bool ImagesAreCompatible(const SubImages& images) {
        const IImage* previousImage = nullptr;
        
        for (auto& image: images) {
            if (image == nullptr) {
                return false;
            }
            
            if (previousImage) {
                if (image->GetFormat() != previousImage->GetFormat() ||
                    image->GetBitsPerComponent() != previousImage->GetBitsPerComponent() ||
                    image->HasPremultipliedAlpha() != previousImage->HasPremultipliedAlpha()) {
                    
                    return false;
                }
            }
            
            previousImage = image.get();
        }
        
        return true;
    }
    
    bool TextureFits(const IVec2& subTexturePosition,
                     const IVec2& subTextureSize,
                     const IVec2& atlasSize) {
        return subTexturePosition.x >= 0 && subTexturePosition.x + subTextureSize.x <= atlasSize.x &&
               subTexturePosition.y >= 0 && subTexturePosition.y + subTextureSize.y <= atlasSize.y;
    }
    
    bool PackVertically(std::vector<SubTexture>& subTextures,
                        const IVec2& atlasSize,
                        const TextureAtlasConfig& textureAtlasConfig) {
        IVec2 cursorPosition {0, 0};
        auto columnWidth = 0;
        auto padding = textureAtlasConfig.mPadding;
        
        for (auto& subTexture: subTextures) {
            auto subTextureSize = subTexture.mImage->GetSize();
            if (!TextureFits(cursorPosition, subTextureSize, atlasSize)) {
                cursorPosition.x += columnWidth + padding;
                cursorPosition.y = 0;
                columnWidth = 0;
                if (!TextureFits(cursorPosition, subTextureSize, atlasSize)) {
                    return false;
                }
            }
            
            subTexture.mPosition = cursorPosition;
            cursorPosition.y += subTextureSize.y + padding;
            columnWidth = std::max(columnWidth, subTextureSize.x);
        }
        
        return true;
    }
    
    Optional<IVec2> PackTexturesVertically(std::vector<SubTexture>& subTextures,
                                           const TextureAtlasConfig& textureAtlasConfig) {
        IVec2 atlasImageSize {minSize, minSize};
        auto counter = 0;
        
        while (atlasImageSize.x <= maxSize && atlasImageSize.y <= maxSize) {
            if (PackVertically(subTextures, atlasImageSize, textureAtlasConfig)) {
                return atlasImageSize;
            }
            
            ++counter;
            if (counter % 2 == 0) {
                atlasImageSize.x *= 2;
            } else {
                atlasImageSize.y *= 2;
            }
        }

        return Optional<IVec2> {};
    }

    bool PackHorizontally(std::vector<SubTexture>& subTextures,
                          const IVec2& atlasSize,
                          const TextureAtlasConfig& textureAtlasConfig) {
        IVec2 cursorPosition {0, 0};
        auto rowHeight = 0;
        auto padding = textureAtlasConfig.mPadding;
        
        for (auto& subTexture: subTextures) {
            auto subTextureSize = subTexture.mImage->GetSize();
            if (!TextureFits(cursorPosition, subTextureSize, atlasSize)) {
                cursorPosition.x = 0;
                cursorPosition.y += rowHeight + padding;
                rowHeight = 0;
                if (!TextureFits(cursorPosition, subTextureSize, atlasSize)) {
                    return false;
                }
            }
            
            subTexture.mPosition = cursorPosition;
            cursorPosition.x += subTextureSize.x + padding;
            rowHeight = std::max(rowHeight, subTextureSize.y);
        }
        
        return true;
    }

    Optional<IVec2> PackTexturesHorizontally(std::vector<SubTexture>& subTextures,
                                             const TextureAtlasConfig& textureAtlasConfig) {
        IVec2 atlasImageSize {minSize, minSize};
        auto counter = 0;
        
        while (atlasImageSize.x <= maxSize && atlasImageSize.y <= maxSize) {
            if (PackHorizontally(subTextures, atlasImageSize, textureAtlasConfig)) {
                return atlasImageSize;
            }
            
            ++counter;
            if (counter % 2 == 0) {
                atlasImageSize.y *= 2;
            } else {
                atlasImageSize.x *= 2;
            }
        }

        return Optional<IVec2> {};
    }

    Optional<IVec2> PackTextures(std::vector<SubTexture>& subTextures,
                                 const TextureAtlasConfig& textureAtlasConfig) {
        switch (textureAtlasConfig.mPackingDirection) {
            case TexturePackingDirection::Horizontal: {
                auto atlasImageSize = PackTexturesHorizontally(subTextures, textureAtlasConfig);
                if (atlasImageSize.HasValue()) {
                    return atlasImageSize;
                }
                return PackTexturesVertically(subTextures, textureAtlasConfig);
            }
            case TexturePackingDirection::Vertical: {
                auto atlasImageSize = PackTexturesVertically(subTextures, textureAtlasConfig);
                if (atlasImageSize.HasValue()) {
                    return atlasImageSize;
                }
                return PackTexturesHorizontally(subTextures, textureAtlasConfig);
            }
        }
    }
    
    Vec2 CalcUV(const IVec2& pixelPosition, const IVec2& textureSize) {
        return Vec2{
            static_cast<float>(pixelPosition.x) / static_cast<float>(textureSize.x),
            static_cast<float>(textureSize.y - pixelPosition.y) / static_cast<float>(textureSize.y)
        };
    }

    std::unique_ptr<IImage>
    CreateAtlasImageAndCalcUVs(std::vector<SubTextureUV>& subTextureUVs,
                               const std::vector<SubTexture>& subTextures,
                               const IVec2& atlasSize) {
        auto* firstSubImage = subTextures.front().mImage;
        auto atlasImage = std::make_unique<BitmapImage>(firstSubImage->GetFormat(), atlasSize);
        atlasImage->SetHasPremultipliedAlpha(firstSubImage->HasPremultipliedAlpha());
        
        subTextureUVs.clear();
        subTextureUVs.reserve(subTextures.size());
        
        for (auto& subTexture: subTextures) {
            atlasImage->Insert(*subTexture.mImage, subTexture.mPosition);
            
            auto subTextureSize = subTexture.mImage->GetSize();
            auto bottomRight = subTexture.mPosition + IVec2{subTextureSize.x, 0};
            auto topRight = subTexture.mPosition + IVec2{subTextureSize.x, subTextureSize.y};
            auto topLeft = subTexture.mPosition + IVec2{0, subTextureSize.y};

            SubTextureUV subTextureUV {
                .mBottomLeft = CalcUV(subTexture.mPosition, atlasSize),
                .mBottomRight = CalcUV(bottomRight, atlasSize),
                .mTopRight = CalcUV(topRight, atlasSize),
                .mTopLeft = CalcUV(topLeft, atlasSize)
            };
            
            subTextureUVs.push_back(subTextureUV);
        }
        
        return atlasImage;
    }
}

std::unique_ptr<IImage>
TextureAtlas::CreateAtlasImage(const SubImages& images,
                               const TextureAtlasConfig& textureAtlasConfig) {
    if (!ImagesAreCompatible(images)) {
        assert(false);
        return nullptr;
    }
    
    std::vector<SubTexture> subTextures;
    subTextures.reserve(images.size());
    for (auto& image: images) {
        subTextures.push_back({.mImage = image.get()});
    }
    
    auto atlasImageSize = PackTextures(subTextures, textureAtlasConfig);
    if (atlasImageSize.HasValue()) {
        return CreateAtlasImageAndCalcUVs(mSubTextureUVs, subTextures, atlasImageSize.GetValue());
    }
    
    return nullptr;
}

const SubTextureUV* TextureAtlas::GetSubTextureUV(int subTextureIndex) const {
    if (subTextureIndex < mSubTextureUVs.size()) {
        return &mSubTextureUVs[subTextureIndex];
    }
    
    return nullptr;
}
