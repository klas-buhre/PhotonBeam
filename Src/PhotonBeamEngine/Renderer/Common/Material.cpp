#include "Material.hpp"

#include <assert.h>

#include "IImage.hpp"

using namespace Pht;

uint32_t Material::mIdCounter = 0;

Material::Material() :
    mShaderId {ShaderId::VertexColor} {

    mDepthState.mDepthWrite = false;
}

Material::Material(const std::string& textureName) :
    mShaderId {ShaderId::Textured},
    mTexture {TextureCache::GetTexture(textureName, GenerateMipmap::Yes)} {}

Material::Material(const IImage& image,
                   GenerateMipmap generateMipmap,
                   const Optional<std::string>& imageName) :
    mShaderId {ShaderId::Textured},
    mTexture {TextureCache::GetTexture(image, generateMipmap, imageName)} {}

Material::Material(const std::string& textureName,
                   float ambient,
                   float diffuse,
                   float specular,
                   float shininess) :
    mAmbient {Color{ambient, ambient, ambient}},
    mDiffuse {Color{diffuse, diffuse, diffuse}},
    mSpecular {Color{specular, specular, specular}},
    mShininess {shininess},
    mShaderId {ShaderId::TexturedLighting},
    mTexture {TextureCache::GetTexture(textureName, GenerateMipmap::Yes)} {}

Material::Material(const std::string& textureName,
                   const std::string& emissionTextureName,
                   float ambient,
                   float diffuse,
                   float specular,
                   float emissive,
                   float shininess) :
    mAmbient {Color{ambient, ambient, ambient}},
    mDiffuse {Color{diffuse, diffuse, diffuse}},
    mSpecular {Color{specular, specular, specular}},
    mEmissive {Color{emissive, emissive, emissive}},
    mShininess {shininess},
    mShaderId {ShaderId::TexturedEmissiveLighting},
    mTexture {TextureCache::GetTexture(textureName, GenerateMipmap::Yes)},
    mEmissionTexture {TextureCache::GetTexture(emissionTextureName, GenerateMipmap::Yes)} {}

Material::Material(const EnvMapTextureFilenames& envMapTextures,
                   const Color& ambient,
                   const Color& diffuse,
                   const Color& specular,
                   float shininess,
                   float reflectivity) :
    mAmbient {ambient},
    mDiffuse {diffuse},
    mSpecular {specular},
    mShininess {shininess},
    mReflectivity {reflectivity},
    mShaderId {ShaderId::EnvMap},
    mEnvMapTexture {TextureCache::GetTexture(envMapTextures)} {}

Material::Material(const std::string& textureName,
                   const EnvMapTextureFilenames& envMapTextures,
                   const Color& ambient,
                   const Color& diffuse,
                   const Color& specular,
                   float shininess,
                   float reflectivity) :
    mAmbient {ambient},
    mDiffuse {diffuse},
    mSpecular {specular},
    mShininess {shininess},
    mReflectivity {reflectivity},
    mShaderId {ShaderId::TexturedEnvMapLighting},
    mTexture {TextureCache::GetTexture(textureName, GenerateMipmap::Yes)},
    mEnvMapTexture {TextureCache::GetTexture(envMapTextures)} {}

Material::Material(const Color& ambient, 
                   const Color& diffuse, 
                   const Color& specular, 
                   float shininess) : 
    mAmbient {ambient},
    mDiffuse {diffuse},
    mSpecular {specular},
    mShininess {shininess} {}

Material::Material(const Color& color) :
    Material {color, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 1.0f} {}

Material::Material(const Material& other) {
    Copy(other);
}

Material& Material::operator=(const Material& other) {
    if (&other == this) {
        return *this;
    }
    
    Copy(other);
    return *this;
}

void Material::Copy(const Material& other) {
    mId = other.mId;
    mIsACopyOrHasBeenCopied = true;
    other.mIsACopyOrHasBeenCopied = true;
    mIsDirty = other.mIsDirty;
    mAmbient = other.mAmbient;
    mDiffuse = other.mDiffuse;
    mSpecular = other.mSpecular;
    mEmissive = other.mEmissive;
    mShininess = other.mShininess;
    mReflectivity = other.mReflectivity;
    mOpacity = other.mOpacity;
    mShaderId = other.mShaderId;
    mTexture = other.mTexture;
    mEmissionTexture = other.mEmissionTexture;
    mEnvMapTexture = other.mEnvMapTexture;
    mBlend = other.mBlend;
    mDepthState = other.mDepthState;
}

void Material::OnModifyMember() {
    if (mIsACopyOrHasBeenCopied) {
        mIsDirty = true;
    }
}

void Material::SetTextureAtlas(const std::vector<std::string>& subTextureFilenames,
                               const TextureAtlasConfig& textureAtlasConfig) {
    OnModifyMember();
    mTexture = TextureCache::GetTextureAtlas(subTextureFilenames, textureAtlasConfig);
}

void Material::SetOpacity(float opacity) {
    OnModifyMember();
    mOpacity = opacity;
    if (mOpacity != 1.0f) {
        mDepthState.mDepthWrite = false;
    }
}

void Material::SetShaderId(ShaderId shaderId) {
    OnModifyMember();
    assert(shaderId != ShaderId::Other);
    mShaderId = shaderId;
    if (mShaderId == ShaderId::VertexColor) {
        mDepthState.mDepthWrite = false;
    }
}

void Material::SetBlend(Blend blend) {
    OnModifyMember();
    mBlend = blend;
    
    switch (blend) {
        case Blend::Yes:
        case Blend::Additive:
            mDepthState.mDepthWrite = false;
            break;
        case Blend::No:
            break;
    }
}

void Material::SetAmbient(const Color& ambient) {
    OnModifyMember();
    mAmbient = ambient;
}

void Material::SetDiffuse(const Color& diffuse) {
    OnModifyMember();
    mDiffuse = diffuse;
}

void Material::SetSpecular(const Color& specular) {
    OnModifyMember();
    mSpecular = specular;
}

void Material::SetEmissive(const Color& emissive) {
    OnModifyMember();
    mEmissive = emissive;
}

void Material::SetReflectivity(float reflectivity) {
    OnModifyMember();
    mReflectivity = reflectivity;
}

void Material::SetDepthTest(bool depthTest) {
    OnModifyMember();
    mDepthState.mDepthTest = depthTest;
}

void Material::SetDepthTestAllowedOverride(bool depthTestAllowedOverride) {
    OnModifyMember();
    mDepthState.mDepthTestAllowedOverride = depthTestAllowedOverride;
}

void Material::SetDepthWrite(bool depthWrite) {
    OnModifyMember();
    mDepthState.mDepthWrite = depthWrite;
}

Blend Material::GetBlend() const {
    if (mBlend == Blend::Additive) {
        return Blend::Additive;
    }

    if (mBlend == Blend::Yes || mOpacity != 1.0f || mShaderId == ShaderId::VertexColor) {
        return Blend::Yes;
    }
    
    return Blend::No;
}
