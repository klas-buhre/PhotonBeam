#ifndef TextureAtlas_hpp
#define TextureAtlas_hpp

#include <vector>
#include <string>

#include "Vector.hpp"
#include "TextureCache.hpp"

namespace Pht {
    class IImage;
    
    enum class TexturePackingDirection {
        Horizontal,
        Vertical
    };
    
    struct TextureAtlasConfig {
        TexturePackingDirection mPackingDirection {TexturePackingDirection::Horizontal};
        GenerateMipmap mGenerateMipmap {GenerateMipmap::No};
        int mPadding {0};
        
        bool operator==(const TextureAtlasConfig& other) const {
            return mPackingDirection == other.mPackingDirection &&
                   mGenerateMipmap == other.mGenerateMipmap &&
                   mPadding == other.mPadding;
        }
    };
    
    using SubImages = std::vector<std::unique_ptr<const IImage>>;

    struct SubTextureUV {
        Vec2 mBottomLeft;
        Vec2 mBottomRight;
        Vec2 mTopRight;
        Vec2 mTopLeft;
    };

    class TextureAtlas {
    public:
        std::unique_ptr<IImage> CreateAtlasImage(const SubImages& images,
                                                 const TextureAtlasConfig& textureAtlasConfig);
        
        const SubTextureUV* GetSubTextureUV(int subTextureIndex) const;
        
        int GetNumSubTextures() const {
            return static_cast<int>(mSubTextureUVs.size());
        }
        
    private:
        std::vector<SubTextureUV> mSubTextureUVs;
    };
}

#endif
