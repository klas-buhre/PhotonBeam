#ifndef TextureCache_hpp
#define TextureCache_hpp

#include <string>
#include <memory>
#include <vector>

#include "Optional.hpp"

namespace Pht {
    enum class GenerateMipmap {
        Yes,
        No
    };
    
    struct TextureHandles;
    class TextureAtlas;
    
    class Texture {
    public:
        Texture(bool hasPremultipliedAlpha, std::unique_ptr<TextureAtlas> atlas);
        ~Texture();
        
        const TextureHandles* GetHandles() const {
            return mHandles.get();
        }

        const TextureAtlas* GetAtlas() const {
            return mAtlas.get();
        }

        bool HasPremultipliedAlpha() const {
            return mHasPremultipliedAlpha;
        }
        
    private:
        std::unique_ptr<TextureHandles> mHandles;
        std::unique_ptr<TextureAtlas> mAtlas;
        bool mHasPremultipliedAlpha {false};
    };
    
    struct EnvMapTextureFilenames {
        bool operator==(const EnvMapTextureFilenames& other) const;
        
        std::string mPositiveX;
        std::string mNegativeX;
        std::string mPositiveY;
        std::string mNegativeY;
        std::string mPositiveZ;
        std::string mNegativeZ;
    };
    
    class IImage;
    class TextureAtlasConfig;
    
    namespace TextureCache {
        std::shared_ptr<Texture> GetTexture(const std::string& textureName,
                                            GenerateMipmap generateMipmap);
        std::shared_ptr<Texture> GetTexture(const EnvMapTextureFilenames& filenames);
        std::shared_ptr<Texture> GetTexture(const IImage& image,
                                            GenerateMipmap generateMipmap,
                                            const Optional<std::string>& name);
        std::shared_ptr<Texture> GetTextureAtlas(const std::vector<std::string>& filenames,
                                                 const TextureAtlasConfig& textureAtlasConfig);
        std::shared_ptr<Texture> GetTextureAtlas(const std::string& name,
                                                 const std::vector<std::unique_ptr<const IImage>>& images,
                                                 const TextureAtlasConfig& textureAtlasConfig);
    }
}

#endif
