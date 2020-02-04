#include "GLES3TextRenderer.hpp"

#include "Font.hpp"
#include "TextureAtlas.hpp"
#include "GLES3RenderStateManager.hpp"
#include "GLES3Handles.hpp"

#define STRINGIFY(A)  #A
#include "../GLES3Shaders/Text.vert"
#include "../GLES3Shaders/Text.frag"
#include "../GLES3Shaders/TextDoubleGradient.vert"
#include "../GLES3Shaders/TextDoubleGradient.frag"
#include "../GLES3Shaders/TextTopGradient.vert"
#include "../GLES3Shaders/TextTopGradient.frag"
#include "../GLES3Shaders/TextMidGradient.vert"
#include "../GLES3Shaders/TextMidGradient.frag"

using namespace Pht;

namespace {
    void DisableVertexAttributes(const GLES3ShaderProgram& shaderProgram) {
        auto& attributes = shaderProgram.GetAttributes();
    
        glDisableVertexAttribArray(attributes.mPosition);
        glDisableVertexAttribArray(attributes.mNormal);
        glDisableVertexAttribArray(attributes.mTextureCoord);
        glDisableVertexAttribArray(attributes.mColor);
        glDisableVertexAttribArray(attributes.mPointSize);
    }
}

GLES3TextRenderer::GLES3TextRenderer(GLES3RenderStateManager& renderState,
                                     const IVec2& screenSize) :
    mRenderState {renderState},
    mProjection {Mat4::OrthographicProjection(0.0f, screenSize.x, 0.0f, screenSize.y, -1.0f, 1.0f)},
    mTextShader {{}},
    mTextDoubleGradientShader {{}},
    mTextTopGradientShader {{}},
    mTextMidGradientShader {{}} {
    
    glGenBuffers(1, &mVbo);
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(GLfloat) * mVertexBuffer.GetCapacity(),
                 nullptr,
                 GL_DYNAMIC_DRAW);
    
    BuildShader(mTextShader, TextVertexShader, TextFragmentShader);
    BuildShader(mTextDoubleGradientShader, TextDoubleGradientVertexShader, TextDoubleGradientFragmentShader);
    BuildShader(mTextTopGradientShader, TextTopGradientVertexShader, TextTopGradientFragmentShader);
    BuildShader(mTextMidGradientShader, TextMidGradientVertexShader, TextMidGradientFragmentShader);
}

GLES3TextRenderer::~GLES3TextRenderer() {
    glDeleteBuffers(1, &mVbo);
}

void GLES3TextRenderer::BuildShader(GLES3ShaderProgram& shader,
                                    const char* vertexShaderSource,
                                    const char* fragmentShaderSource) {
    shader.Build(vertexShaderSource, fragmentShaderSource);
    shader.Use();
    shader.SetProjection(mProjection);
}

void GLES3TextRenderer::RenderText(const std::string& text,
                                   Vec2 position,
                                   float slant,
                                   RenderQueue::TextKind textKind,
                                   const ColorProperties& colorProperties,
                                   const TextProperties& properties) {
    auto& shaderProgram = GetShaderProgram(textKind);
    auto& uniforms = shaderProgram.GetUniforms();
    auto& attributes = shaderProgram.GetAttributes();
    
    if (!mRenderState.IsShaderInUse(shaderProgram)) {
        mRenderState.UseShader(shaderProgram);
        mRenderState.SetDepthTest(false);
        mRenderState.SetBlend(true);
        mRenderState.SetBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        DisableVertexAttributes(shaderProgram);
        
        auto stride = static_cast<int>(sizeof(Vec4) + sizeof(float));
        glBindBuffer(GL_ARRAY_BUFFER, mVbo);
        glEnableVertexAttribArray(attributes.mTextCoords);
        glVertexAttribPointer(attributes.mTextCoords, 4, GL_FLOAT, GL_FALSE, stride, 0);
        
        auto offset = sizeof(Vec4);
        glEnableVertexAttribArray(attributes.mTextGradientFunction);
        glVertexAttribPointer(attributes.mTextGradientFunction,
                              1,
                              GL_FLOAT,
                              GL_FALSE,
                              stride,
                              reinterpret_cast<const GLvoid*>(offset));
    }
    
    glUniform4fv(uniforms.mTextColor, 1, colorProperties.mColor.Pointer());
    
    if (colorProperties.mTopGradientColorSubtraction.HasValue()) {
        glUniform3fv(uniforms.mTextTopColorSubtraction,
                     1,
                     colorProperties.mTopGradientColorSubtraction.GetValue().Pointer());
    }

    if (colorProperties.mMidGradientColorSubtraction.HasValue()) {
        glUniform3fv(uniforms.mTextMidColorSubtraction,
                     1,
                     colorProperties.mMidGradientColorSubtraction.GetValue().Pointer());
    }
    
    switch (properties.mAlignment) {
        case TextAlignment::Left:
            break;
        case TextAlignment::CenterX:
            position = AdjustPositionCenterXAlignment(text, position, slant, properties);
            break;
    }
    
    auto& font = properties.mFont;
    auto* texture = font.GetTexture();
    if (texture == nullptr) {
        assert(texture);
        return;
    }
    
    auto* textureAtlas = texture->GetAtlas();
    if (textureAtlas == nullptr) {
        assert(texture);
        return;
    }
    
    mRenderState.BindTexture(GL_TEXTURE0, GL_TEXTURE_2D, texture->GetHandles()->mGLHandle);
    mVertexBuffer.Clear();
    mNumVertices = 0;
    
    auto numCharacters = 0;
    
    for (auto c: text) {
        if (numCharacters > maxNumCharacters) {
            break;
        }
        
        auto* glyph = font.GetGlyph(c);
        if (glyph == nullptr) {
            continue;
        }
        
        GLfloat xPos {position.x + glyph->mBearing.x * properties.mScale};
        GLfloat yPos {position.y - (glyph->mSize.y - glyph->mBearing.y) * properties.mScale};
        GLfloat w {glyph->mSize.x * properties.mScale};
        GLfloat h {glyph->mSize.y * properties.mScale};
        
        position.x += (glyph->mAdvance >> 6) * properties.mScale;
        
        auto textureIndex = glyph->mSubTextureIndex;
        if (!textureIndex.HasValue()) {
            continue;
        }
        
        auto* uv = textureAtlas->GetSubTextureUV(textureIndex.GetValue());
        if (uv == nullptr) {
            continue;
        }

        WriteVertex({xPos + slant,     yPos + h}, {uv->mTopLeft.x,     uv->mTopLeft.y},     0.0f);
        WriteVertex({xPos,             yPos},     {uv->mBottomLeft.x,  uv->mBottomLeft.y},  1.0f);
        WriteVertex({xPos + w,         yPos},     {uv->mBottomRight.x, uv->mBottomRight.y}, 1.0f);
        
        WriteVertex({xPos + slant,     yPos + h}, {uv->mTopLeft.x,     uv->mTopLeft.y},     0.0f);
        WriteVertex({xPos + w,         yPos},     {uv->mBottomRight.x, uv->mBottomRight.y}, 1.0f);
        WriteVertex({xPos + w + slant, yPos + h}, {uv->mTopRight.x,    uv->mTopRight.y},    0.0f);

        ++numCharacters;
    }
    
    // Should use glBufferSubData here but it leads to very poor performance for some reason.
    glBufferData(GL_ARRAY_BUFFER,
                 mVertexBuffer.Size() * sizeof(float),
                 mVertexBuffer.GetData(),
                 GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, mNumVertices);
    IF_USING_FRAME_STATS(mRenderState.ReportDrawCall());
}

void GLES3TextRenderer::WriteVertex(const Vec2& position,
                                    const Vec2& textureCoords,
                                    float gradientFunction) {
    mVertexBuffer.PushBack(position.x);
    mVertexBuffer.PushBack(position.y);
    mVertexBuffer.PushBack(textureCoords.x);
    mVertexBuffer.PushBack(textureCoords.y);
    mVertexBuffer.PushBack(gradientFunction);

    ++mNumVertices;
}

GLES3ShaderProgram& GLES3TextRenderer::GetShaderProgram(RenderQueue::TextKind textKind) {
    switch (textKind) {
        case RenderQueue::TextKind::DoubleGradientShader:
            return mTextDoubleGradientShader;
        case RenderQueue::TextKind::TopGradientShader:
            return mTextTopGradientShader;
        case RenderQueue::TextKind::MidGradientShader:
            return mTextMidGradientShader;
        case RenderQueue::TextKind::PlainShader:
        case RenderQueue::TextKind::Specular:
        case RenderQueue::TextKind::Shadow:
        case RenderQueue::TextKind::SecondShadow:
            return mTextShader;
        case RenderQueue::TextKind::None:
            assert(false);
            break;
    }

    return mTextShader;
}

Vec2 GLES3TextRenderer::AdjustPositionCenterXAlignment(const std::string& text,
                                                       Vec2 position,
                                                       float slant,
                                                       const TextProperties& properties) {
    if (text.empty()) {
        return position;
    }
    
    auto textWidth = CalculateTextWidth(text, slant, properties);
    auto* firstGlyph = properties.mFont.GetGlyph(text.front());
    if (firstGlyph) {
        position.x = position.x - firstGlyph->mBearing.x * properties.mScale - textWidth / 2.0f;
    }
    
    return position;
}

float GLES3TextRenderer::CalculateTextWidth(const std::string& text,
                                            float slant,
                                            const TextProperties& properties) {
    if (text.empty()) {
        return 0.0f;
    }
    
    auto x = 0.0f;
    auto textStartX = x;
    for (auto i = 0; i < text.size(); ++i) {
        auto* glyph = properties.mFont.GetGlyph(text[i]);
        if (glyph == nullptr) {
            return 0.0f;
        }
        
        if (i == 0) {
            textStartX = glyph->mBearing.x;
        }
        
        if (i == text.size() - 1) {
            x += glyph->mBearing.x + glyph->mSize.x + slant;
            break;
        }
        
        x += glyph->mAdvance >> 6;
    }
    
    return (x - textStartX) * properties.mScale;
}
