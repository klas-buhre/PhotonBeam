#include "GLES3RenderStateManager.hpp"

#include <assert.h>

#include "GLES3ShaderProgram.hpp"

using namespace Pht;

void GLES3RenderStateManager::Init() {
    UpdateGLBlend();
    UpdateGLBlendFunc();
    UpdateGLDepthTest();
    UpdateGLDepthWrite();
    UpdateGLCullFace();
    UpdateGLScissorTest();
}

void GLES3RenderStateManager::SetBlend(bool blendEnabled) {
    if (mBlendEnabled != blendEnabled) {
        mBlendEnabled = blendEnabled;
        UpdateGLBlend();
    }
}

void GLES3RenderStateManager::SetBlendFunc(GLenum sFactor, GLenum dFactor) {
    if (mBlendSFactor != sFactor || mBlendDFactor != dFactor) {
        mBlendSFactor = sFactor;
        mBlendDFactor = dFactor;
        UpdateGLBlendFunc();
    }
}

void GLES3RenderStateManager::SetDepthTest(bool depthTestEnabled) {
    if (mDepthTestEnabled != depthTestEnabled) {
        mDepthTestEnabled = depthTestEnabled;
        UpdateGLDepthTest();
    }
}

void GLES3RenderStateManager::SetDepthWrite(bool depthWriteEnabled) {
    if (mDepthWriteEnabled != depthWriteEnabled) {
        mDepthWriteEnabled = depthWriteEnabled;
        UpdateGLDepthWrite();
    }
}

void GLES3RenderStateManager::SetCullFace(bool cullFaceEnabled) {
    if (mCullFaceEnabled != cullFaceEnabled) {
        mCullFaceEnabled = cullFaceEnabled;
        UpdateGLCullFace();
    }
}

void GLES3RenderStateManager::SetScissorTest(bool scissorTestEnabled) {
    if (mScissorTestEnabled != scissorTestEnabled) {
        mScissorTestEnabled = scissorTestEnabled;
        UpdateGLScissorTest();
    }
}

void GLES3RenderStateManager::BindTexture(GLenum textureUnitIndex, GLenum target, GLuint texture) {
    auto* unit = GetTextureUnit(textureUnitIndex);
    if (unit == nullptr) {
        assert(false);
        return;
    }
    
    if (!unit->HasValue() ||
        unit->GetValue().mTarget != target || unit->GetValue().mTexture != texture) {
        
        *unit = TextureUnit {.mTarget = target, .mTexture = texture};
        
        glActiveTexture(textureUnitIndex);
        glBindTexture(target, texture);
        IF_USING_FRAME_STATS(ReportTextureBind());
    }
}

Optional<GLES3RenderStateManager::TextureUnit>*
GLES3RenderStateManager::GetTextureUnit(GLenum unitIndex) {
    switch (unitIndex) {
        case GL_TEXTURE0:
            return &mTextureUnit0;
        case GL_TEXTURE1:
            return &mTextureUnit1;
        default:
            return nullptr;
    }
}

void GLES3RenderStateManager::UpdateGLBlend() {
    if (mBlendEnabled) {
        glEnable(GL_BLEND);
    } else {
        glDisable(GL_BLEND);
    }
}

void GLES3RenderStateManager::UpdateGLBlendFunc() {
    glBlendFunc(mBlendSFactor, mBlendDFactor);
}

void GLES3RenderStateManager::UpdateGLDepthTest() {
    if (mDepthTestEnabled) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
}

void GLES3RenderStateManager::UpdateGLDepthWrite() {
    if (mDepthWriteEnabled) {
        glDepthMask(GL_TRUE);
    } else {
        glDepthMask(GL_FALSE);
    }
}

void GLES3RenderStateManager::UpdateGLCullFace() {
    if (mCullFaceEnabled) {
        glEnable(GL_CULL_FACE);
    } else {
        glDisable(GL_CULL_FACE);
    }
}

void GLES3RenderStateManager::UpdateGLScissorTest() {
    if (mScissorTestEnabled) {
        glEnable(GL_SCISSOR_TEST);
    } else {
        glDisable(GL_SCISSOR_TEST);
    }
}

void GLES3RenderStateManager::UseShader(GLES3ShaderProgram& shaderProgram) {
    InvalidateShader();
    assert(shaderProgram.IsEnabled());
    shaderProgram.Use();
    mShaderProgram = &shaderProgram;
    IF_USING_FRAME_STATS(++mFrameStats.mNumShaderUses);
}

void GLES3RenderStateManager::OnBeginRenderPass() {
    InvalidateShader();
}

void GLES3RenderStateManager::InvalidateShader() {
    mShaderProgram = nullptr;
    mMaterial = nullptr;
    mVbo = nullptr;    
    mTextureUnit0.Reset();
    mTextureUnit1.Reset();
}

#if USE_FRAME_STATS
void GLES3RenderStateManager::LogFrameStats(float frameSeconds) {
    std::cout << "=======================================" << std::endl
              << " FPS: " << 1.0f / frameSeconds << std::endl
              << " NumShaderUses: " << mFrameStats.mNumShaderUses << std::endl
              << " NumMaterialUses: " << mFrameStats.mNumMaterialUses << std::endl
              << " NumTextureBinds: " << mFrameStats.mNumTextureBinds << std::endl
              << " NumVboUses: " << mFrameStats.mNumVboUses << std::endl
              << " NumDrawCalls: " << mFrameStats.mNumDrawCalls << std::endl;
}
#endif
