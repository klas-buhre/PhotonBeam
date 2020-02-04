#ifndef GLES3RenderStateManager_hpp
#define GLES3RenderStateManager_hpp

#define GLES_SILENCE_DEPRECATION

#include <OpenGLES/ES3/gl.h>

#include "Optional.hpp"
#include "Noncopyable.hpp"
#include "Material.hpp"

#define USE_FRAME_STATS 0
#if USE_FRAME_STATS
    #define IF_USING_FRAME_STATS(x) x;
#else
    #define IF_USING_FRAME_STATS(x)
#endif

namespace Pht {
    class GLES3ShaderProgram;
    class Material;
    class GpuVertexBuffer;
    
    class GLES3RenderStateManager: public Noncopyable {
    public:
        void Init();
        void SetBlend(bool enabled);
        void SetBlendFunc(GLenum sFactor, GLenum dFactor);
        void SetDepthTest(bool enabled);
        void SetDepthWrite(bool enabled);
        void SetCullFace(bool enabled);
        void SetScissorTest(bool enabled);
        void BindTexture(GLenum textureUnitIndex, GLenum target, GLuint texture);
        void UseShader(GLES3ShaderProgram& shaderProgram);
        void OnBeginRenderPass();

        void UseMaterial(const Material& material) {
            mMaterial = &material;
            IF_USING_FRAME_STATS(++mFrameStats.mNumMaterialUses);
        }

        void UseVbo(const GpuVertexBuffer& vbo) {
            mVbo = &vbo;
            IF_USING_FRAME_STATS(ReportVboUse());
        }

        bool IsShaderInUse(const GLES3ShaderProgram& shaderProgram) const {
            return &shaderProgram == mShaderProgram;
        }
        
        bool IsMaterialInUse(const Material& material) const {
            return mMaterial->Equals(material);
        }

        bool IsVboInUse(const GpuVertexBuffer& vbo) const {
            return &vbo == mVbo;
        }
        
#if USE_FRAME_STATS
        void LogFrameStats(float frameSeconds);
        
        void ReportDrawCall() {
            ++mFrameStats.mNumDrawCalls;
        }
        
        void ReportTextureBind() {
            ++mFrameStats.mNumTextureBinds;
        }

        void ReportVboUse() {
            ++mFrameStats.mNumVboUses;
        }

        void ResetFrameStats() {
            mFrameStats = FrameStats {};
        }
#endif

    private:
        struct TextureUnit {
            GLenum mTarget;
            GLuint mTexture;
        };

        void UpdateGLBlend();
        void UpdateGLBlendFunc();
        void UpdateGLDepthTest();
        void UpdateGLDepthWrite();
        void UpdateGLCullFace();
        void UpdateGLScissorTest();
        Optional<TextureUnit>* GetTextureUnit(GLenum unitIndex);
        void InvalidateShader();
        
        struct FrameStats {
            int mNumShaderUses {0};
            int mNumMaterialUses {0};
            int mNumTextureBinds {0};
            int mNumVboUses {0};
            int mNumDrawCalls {0};
        };
        
        FrameStats mFrameStats;
        bool mBlendEnabled {false};
        GLenum mBlendSFactor {GL_ONE};
        GLenum mBlendDFactor {GL_ZERO};
        bool mDepthTestEnabled {false};
        bool mDepthWriteEnabled {true};
        bool mCullFaceEnabled {false};
        bool mScissorTestEnabled {false};
        Optional<TextureUnit> mTextureUnit0;
        Optional<TextureUnit> mTextureUnit1;
        const GLES3ShaderProgram* mShaderProgram {nullptr};
        const Material* mMaterial {nullptr};
        const GpuVertexBuffer* mVbo {nullptr};
    };
}

#endif
