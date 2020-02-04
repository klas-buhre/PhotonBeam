#ifndef Material_hpp
#define Material_hpp

#include "TextureCache.hpp"

namespace Pht {
    class TextureAtlasConfig;

    struct Color {
        float mRed {0.0f};
        float mGreen {0.0f};
        float mBlue {0.0f};
        
        Color operator+(const Color& other) const {
            return Color {mRed + other.mRed, mGreen + other.mGreen, mBlue + other.mBlue};
        }
        
        Color operator*(float brightness) const {
            return Color {mRed * brightness, mGreen * brightness, mBlue * brightness};
        }
    };
    
    enum class ShaderId: uint8_t {
        PixelLighting = 0,
        VertexLighting,
        TexturedLighting,
        TexturedLightingVertexColor,
        TexturedPixelLighting,
        TexturedEmissiveLighting,
        TexturedEnvMapLighting,
        Textured,
        EnvMap,
        VertexColor,
        Particle,
        ParticleTextureColor,
        ParticleNoAlphaTexture,
        PointParticle,
        Other
    };
    
    enum class Blend {
        Yes,
        Additive,
        No
    };
    
    struct DepthState {
        bool mDepthTest {true};
        bool mDepthTestAllowedOverride {false};
        bool mDepthWrite {true};
    };

    class Material {
    public:
        Material();
        explicit Material(const std::string& textureName);
        Material(const IImage& image,
                 GenerateMipmap generateMipmap,
                 const Optional<std::string>& imageName = {});
        Material(const std::string& textureName,
                 float ambient,
                 float diffuse,
                 float specular,
                 float shininess);
        Material(const std::string& textureName,
                 const std::string& emissionTextureName,
                 float ambient,
                 float diffuse,
                 float specular,
                 float emissive,
                 float shininess);
        Material(const EnvMapTextureFilenames& envMapTextureFilenames,
                 const Color& ambient,
                 const Color& diffuse, 
                 const Color& specular, 
                 float shininess,
                 float reflectivity);
        Material(const std::string& textureName,
                 const EnvMapTextureFilenames& envMapTextureFilenames,
                 const Color& ambient,
                 const Color& diffuse,
                 const Color& specular,
                 float shininess,
                 float reflectivity);
        Material(const Color& ambient, 
                 const Color& diffuse, 
                 const Color& specular,
                 float shininess);
        explicit Material(const Color& color);
        Material(const Material& other);
        Material& operator=(const Material& other);
        
        void SetTextureAtlas(const std::vector<std::string>& subTextureFilenames,
                             const TextureAtlasConfig& textureAtlasConfig);
        void SetOpacity(float opacity);
        void SetShaderId(ShaderId shaderId);
        void SetBlend(Blend blend);
        void SetAmbient(const Color& ambient);
        void SetDiffuse(const Color& diffuse);
        void SetSpecular(const Color& specular);
        void SetEmissive(const Color& emissive);
        void SetReflectivity(float reflectivity);
        void SetDepthTest(bool depthTest);
        void SetDepthTestAllowedOverride(bool depthTestAllowedOverride);
        void SetDepthWrite(bool depthWrite);
        Blend GetBlend() const;

        const Color& GetAmbient() const {
            return mAmbient;
        }
        
        const Color& GetDiffuse() const {
            return mDiffuse;
        }
        
        const Color& GetSpecular() const {
            return mSpecular;
        }
        
        const Color& GetEmissive() const {
            return mEmissive;
        }
        
        float GetShininess() const {
            return mShininess;
        }
        
        float GetReflectivity() const {
            return mReflectivity;
        }
        
        float GetOpacity() const {
            return mOpacity;
        }
        
        ShaderId GetShaderId() const {
            return mShaderId;
        }

        const DepthState& GetDepthState() const {
            return mDepthState;
        }
        
        const Texture* GetTexture() const {
            return mTexture.get();
        }

        const Texture* GetEmissionTexture() const {
            return mEmissionTexture.get();
        }

        const Texture* GetEnvMapTexture() const {
            return mEnvMapTexture.get();
        }
        
        uint32_t GetId() const {
            return mId;
        }
        
        bool Equals(const Material& other) const {
            if (this == &other) {
                return true;
            }
            
            return mId == other.mId && !mIsDirty && !other.mIsDirty;
        }

    private:
        void Copy(const Material& other);
        void OnModifyMember();
        
        static uint32_t mIdCounter;
        
        uint32_t mId {mIdCounter++};
        mutable bool mIsACopyOrHasBeenCopied {false};
        bool mIsDirty {false};
        Color mAmbient;
        Color mDiffuse;
        Color mSpecular;
        Color mEmissive;
        float mShininess {0.0f};
        float mReflectivity {0.0f};
        float mOpacity {1.0f};
        ShaderId mShaderId {ShaderId::VertexLighting};
        std::shared_ptr<Texture> mTexture;
        std::shared_ptr<Texture> mEmissionTexture;
        std::shared_ptr<Texture> mEnvMapTexture;
        Blend mBlend {Blend::No};
        DepthState mDepthState;
    };
}

#endif
