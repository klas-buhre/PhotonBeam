#include "Font.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "FileSystem.hpp"
#include "TextureCache.hpp"
#include "TextureAtlas.hpp"
#include "BitmapImage.hpp"

using namespace Pht;

Font::Font(const std::string& filename, int size) :
    mSize {size} {

    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cout << "Font: ERROR: Could not init FreeType Library" << std::endl;
        assert(false);
    }

    std::string fullPath {FileSystem::GetResourceDirectory() + "/" + filename};
    FT_Face face;
    if (auto errorCode {FT_New_Face(ft, fullPath.c_str(), 0, &face)}) {
        std::cout << "Font: ERROR: Failed to load font. Error code: " << errorCode << std::endl;
        assert(false);
    }

    FT_Set_Pixel_Sizes(face, 0, size);
    
    auto subTextureIndex = 0;
    std::vector<std::unique_ptr<const IImage>> glyphImages;

    // Load the first printable set of the ASCII table.
    for (char c {firstPrintableCharacter}; c < lastPrintableCharacter; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cout << "Font: ERROR: Failed to load Glyph" << std::endl;
            continue;
        }
        
        Optional<int> glyphSubTextureIndex;
        IVec2 glyphImageSize {
            static_cast<int>(face->glyph->bitmap.width),
            static_cast<int>(face->glyph->bitmap.rows)
        };

        if (glyphImageSize.x > 0 && glyphImageSize.y > 0) {
            auto glyphImage = std::make_unique<BitmapImage>(face->glyph->bitmap.buffer,
                                                            ImageFormat::Alpha,
                                                            glyphImageSize);
            glyphImages.push_back(std::move(glyphImage));
            glyphSubTextureIndex = subTextureIndex;
            ++subTextureIndex;
        }
        
        Glyph glyph {
            glyphSubTextureIndex,
            IVec2 {
                static_cast<int>(face->glyph->bitmap.width),
                static_cast<int>(face->glyph->bitmap.rows)
            },
            IVec2 {face->glyph->bitmap_left, face->glyph->bitmap_top},
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        mGlyphs.push_back(glyph);
    }
    
    TextureAtlasConfig textureAtlasConfig {
        .mPackingDirection = TexturePackingDirection::Horizontal,
        .mGenerateMipmap = GenerateMipmap::No,
        .mPadding = 1
    };
    auto textureAtlasName = filename + std::to_string(size);
    mTexture = TextureCache::GetTextureAtlas(textureAtlasName, glyphImages, textureAtlasConfig);
    
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

Font::~Font() {}
