#include <assert.h>
#include <memory>
#include <unordered_map>

#define GLES_SILENCE_DEPRECATION

#include <OpenGLES/ES3/gl.h>

#include "RenderableObject.hpp"
#include "Material.hpp"
#include "IMesh.hpp"
#include "Vector.hpp"
#include "VertexBuffer.hpp"
#include "Font.hpp"
#include "SceneObject.hpp"
#include "Scene.hpp"
#include "ISceneManager.hpp"
#include "CameraComponent.hpp"
#include "LightComponent.hpp"
#include "TextComponent.hpp"
#include "IEngine.hpp"
#include "IRendererInternal.hpp"
#include "Camera.hpp"
#include "Material.hpp"
#include "RenderQueue.hpp"
#include "VertexBufferCache.hpp"
#include "GLES3Handles.hpp"
#include "GLES3ShaderProgram.hpp"
#include "GLES3TextRenderer.hpp"
#include "GLES3RenderStateManager.hpp"

#define STRINGIFY(A)  #A
#include "../GLES3Shaders/PixelLighting.vert"
#include "../GLES3Shaders/PixelLighting.frag"
#include "../GLES3Shaders/VertexLighting.vert"
#include "../GLES3Shaders/VertexLighting.frag"
#include "../GLES3Shaders/TexturedLighting.vert"
#include "../GLES3Shaders/TexturedLighting.frag"
#include "../GLES3Shaders/TexturedLightingVertexColor.vert"
#include "../GLES3Shaders/TexturedLightingVertexColor.frag"
#include "../GLES3Shaders/TexturedPixelLighting.vert"
#include "../GLES3Shaders/TexturedPixelLighting.frag"
#include "../GLES3Shaders/TexturedEmissiveLighting.vert"
#include "../GLES3Shaders/TexturedEmissiveLighting.frag"
#include "../GLES3Shaders/TexturedEnvMapLighting.vert"
#include "../GLES3Shaders/TexturedEnvMapLighting.frag"
#include "../GLES3Shaders/Textured.vert"
#include "../GLES3Shaders/Textured.frag"
#include "../GLES3Shaders/EnvMap.vert"
#include "../GLES3Shaders/EnvMap.frag"
#include "../GLES3Shaders/VertexColor.vert"
#include "../GLES3Shaders/VertexColor.frag"
#include "../GLES3Shaders/Particle.vert"
#include "../GLES3Shaders/Particle.frag"
#include "../GLES3Shaders/ParticleTextureColor.vert"
#include "../GLES3Shaders/ParticleTextureColor.frag"
#include "../GLES3Shaders/ParticleNoAlphaTexture.vert"
#include "../GLES3Shaders/ParticleNoAlphaTexture.frag"
#include "../GLES3Shaders/PointParticle.vert"
#include "../GLES3Shaders/PointParticle.frag"

using namespace Pht;

namespace {
    class GLES3Renderer: public IRendererInternal {
    public:
        GLES3Renderer(bool createFrameBuffer);
        
        // Methods implementing IRenderer:
        void EnableShader(ShaderId shaderId) override;
        void DisableShader(ShaderId shaderId) override;
        void SetClearColorBuffer(bool clearColorBuffer) override;
        void SetHudMode(bool hudMode) override;
        void SetProjectionMode(ProjectionMode projectionMode) override;
        int GetAdjustedNumPixels(int numPixels) const override;
        const Mat4& GetViewMatrix() const override;
        const Mat4& GetProjectionMatrix() const override;
        const Vec2& GetHudFrustumSize() const override;
        const Vec2& GetOrthographicFrustumSize() const override;
        float GetFrustumHeightFactor() const override;
        const IVec2& GetRenderBufferSize() const override;
        float GetTopPaddingHeight() const override;
        float GetBottomPaddingHeight() const override;

        // Methods implementing IRendererSystem:
        void Init(bool createFrameBuffer) override;
        void InitCamera(float narrowFrustumHeightFactor) override;
        void InitRenderQueue(const Scene& scene) override;
        std::unique_ptr<RenderableObject> CreateRenderableObject(const IMesh& mesh,
                                                                 const Material& material,
                                                                 VertexBufferLocation bufferLocation) override;
        void ClearFrameBuffer() override;
        void RenderScene(const Scene& scene, float frameSeconds) override;
        
    private:
        void InitOpenGL(bool createFrameBuffer);
        void InitCamera();
        void InitHudFrustum();
        float GetAspectRatio() const;
        void InitShaders();
        void SetDepthTest(const DepthState& depthState);
        void SetScissorBox(const Vec2& lowerLeft, const Vec2& size);
        void SetLightDirection(const Vec3& lightDirection);
        void CalculateCameraSpaceLightDirection();
        const Vec3& GetCameraPosition() const;
        void RenderObject(const RenderableObject& renderableObject, const Mat4& modelTransform);
        void SetTransforms(const Mat4& modelTransform, GLES3ShaderProgram& shaderProgram);
        void SetMaterialProperties(const Material& material,
                                   ShaderId shaderId,
                                   const GLES3ShaderProgram& shaderProgram);
        void BindTextures(const Material& material,
                          ShaderId shaderId,
                          const GLES3ShaderProgram& shaderProgram);
        void SetupBlend(const Material& material, ShaderId shaderId);
        void SetVbo(const RenderableObject& renderableObject,
                    const GLES3ShaderProgram& shaderProgram);
        void CreateShader(ShaderId shaderId, const VertexFlags& vertexFlags);
        GLES3ShaderProgram& GetShader(ShaderId shaderId);
        void RenderText(const std::string& text,
                        const Vec2& position,
                        RenderQueue::TextKind textKind,
                        const TextProperties& properties);
        void RenderTextImpl(const std::string& text,
                            const Vec2& position,
                            RenderQueue::TextKind textKind,
                            const GLES3TextRenderer::ColorProperties& colorProperties,
                            const TextProperties& properties);
        void Render(const RenderPass& renderPass, DistanceFunction distanceFunction);
        Vec2 CalculateTextHudPosition(const TextComponent& textComponent);
        
        struct HudFrustum {
            Mat4 mProjection;
            Vec2 mSize;
        };
        
        struct GlobalLight {
            Vec3 mDirectionWorldSpace;
            Vec3 mDirectionCameraSpace;
            float mDirectionalIntensity {1.0f};
            float mAmbientIntensity {1.0f};
        };
        
        ProjectionMode mProjectionMode {ProjectionMode::Perspective};
        GLuint mColorRenderbuffer {0};
        GlobalLight mGlobalLight;
        Camera mCamera;
        Vec3 mHudCameraPosition;
        HudFrustum mHudFrustum;
        Vec2 mOrthographicFrustumSize;
        float mNarrowFrustumHeightFactor {1.0f};
        IVec2 mRenderBufferSize;
        RenderQueue mRenderQueue;
        GLES3RenderStateManager mRenderState;
        std::unordered_map<ShaderId, std::unique_ptr<GLES3ShaderProgram>> mShaders;
        std::unique_ptr<GLES3TextRenderer> mTextRenderer;
        bool mClearColorBuffer {true};
        bool mHudMode {false};
        bool mIsDepthTestAllowed {true};
    };

    constexpr auto defaultScreenHeight = 1136;
    constexpr auto topPadding = 1.05f;
    constexpr auto bottomPadding = 1.67f;
    const Mat4 identityMatrix;
    const Vec4 modelSpaceOrigin {0.0f, 0.0f, 0.0f, 1.0f};
    
    struct FrustumSettings {
        float mHeight;
        float mZNearClip;
        float mZFarClip;
    };
    
    const static FrustumSettings perspectiveFrustumSettings {
        .mHeight = 7.1f,
        .mZNearClip = 5.0f,
        .mZFarClip = 750.0f
    };

    const static FrustumSettings orthographicFrustumSettings {
        .mHeight = 24.317f,
        .mZNearClip = -1.0f,
        .mZFarClip = 75.0f
    };

    const static FrustumSettings hudFrustumSettings {
        .mHeight = 26.625f,
        .mZNearClip = -1.0f,
        .mZFarClip = 75.0f
    };

    bool IsParticleShader(ShaderId shaderId) {
        switch (shaderId) {
            case ShaderId::Particle:
            case ShaderId::ParticleTextureColor:
            case ShaderId::ParticleNoAlphaTexture:
            case ShaderId::PointParticle:
                return true;
            default:
                return false;
        }
    }

    int CalculateStride(VertexFlags vertexFlags) {
        int stride {sizeof(Vec3)};

        if (vertexFlags.mNormals) {
            stride += sizeof(Vec3);
        }
        
        if (vertexFlags.mTextureCoords) {
            stride += sizeof(Vec2);
        }
        
        if (vertexFlags.mColors) {
            stride += sizeof(Vec4);
        }
        
        if (vertexFlags.mPointSizes) {
            stride += sizeof(float);
        }
        
        return stride;
    }
    
    void EnableVertexAttributes(const GLES3ShaderProgram& shaderProgram) {
        auto vertexFlags = shaderProgram.GetVertexFlags();
        auto stride = CalculateStride(vertexFlags);
        auto& attributes = shaderProgram.GetAttributes();
        
        glDisableVertexAttribArray(attributes.mTextCoords);
        glEnableVertexAttribArray(attributes.mPosition);
        glVertexAttribPointer(attributes.mPosition, 3, GL_FLOAT, GL_FALSE, stride, 0);
        
        auto offset = sizeof(Vec3);
        
        if (vertexFlags.mNormals) {
            glEnableVertexAttribArray(attributes.mNormal);
            glVertexAttribPointer(attributes.mNormal,
                                  3,
                                  GL_FLOAT,
                                  GL_FALSE,
                                  stride,
                                  reinterpret_cast<const GLvoid*>(offset));
            offset += sizeof(Vec3);
        } else {
            glDisableVertexAttribArray(attributes.mNormal);
        }
        
        if (vertexFlags.mTextureCoords) {
            glEnableVertexAttribArray(attributes.mTextureCoord);
            glVertexAttribPointer(attributes.mTextureCoord,
                                  2,
                                  GL_FLOAT,
                                  GL_FALSE,
                                  stride,
                                  reinterpret_cast<const GLvoid*>(offset));
            offset += sizeof(Vec2);
        } else {
            glDisableVertexAttribArray(attributes.mTextureCoord);
        }

        if (vertexFlags.mColors) {
            glEnableVertexAttribArray(attributes.mColor);
            glVertexAttribPointer(attributes.mColor,
                                  4,
                                  GL_FLOAT,
                                  GL_FALSE,
                                  stride,
                                  reinterpret_cast<const GLvoid*>(offset));
            offset += sizeof(Vec4);
        } else {
            glDisableVertexAttribArray(attributes.mColor);
        }

        if (vertexFlags.mPointSizes) {
            glEnableVertexAttribArray(attributes.mPointSize);
            glVertexAttribPointer(attributes.mPointSize,
                                  1,
                                  GL_FLOAT,
                                  GL_FALSE,
                                  stride,
                                  reinterpret_cast<const GLvoid*>(offset));
            offset += sizeof(float);
        } else {
            glDisableVertexAttribArray(attributes.mPointSize);
        }
    }
}

std::unique_ptr<IRendererInternal> Pht::CreateRenderer(bool createFrameBuffer) {
    return std::make_unique<GLES3Renderer>(createFrameBuffer);
}

GLES3Renderer::GLES3Renderer(bool createFrameBuffer) {
    CreateShader(ShaderId::PixelLighting,               {.mNormals = true});
    CreateShader(ShaderId::VertexLighting,              {.mNormals = true});
    CreateShader(ShaderId::TexturedLighting,            {.mNormals = true, .mTextureCoords = true});
    CreateShader(ShaderId::TexturedLightingVertexColor, {.mNormals = true, .mTextureCoords = true, .mColors = true});
    CreateShader(ShaderId::TexturedPixelLighting,       {.mNormals = true, .mTextureCoords = true});
    CreateShader(ShaderId::TexturedEmissiveLighting,    {.mNormals = true, .mTextureCoords = true});
    CreateShader(ShaderId::TexturedEnvMapLighting,      {.mNormals = true, .mTextureCoords = true});
    CreateShader(ShaderId::Textured,                    {.mTextureCoords = true});
    CreateShader(ShaderId::EnvMap,                      {.mNormals = true});
    CreateShader(ShaderId::VertexColor,                 {.mColors = true});
    CreateShader(ShaderId::Particle,                    {.mTextureCoords = true, .mColors = true});
    CreateShader(ShaderId::ParticleTextureColor,        {.mTextureCoords = true, .mColors = true});
    CreateShader(ShaderId::ParticleNoAlphaTexture,      {.mTextureCoords = true, .mColors = true});
    CreateShader(ShaderId::PointParticle,               {.mColors = true, .mPointSizes = true});
    
    if (createFrameBuffer) {
        glGenRenderbuffers(1, &mColorRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, mColorRenderbuffer);
    }
}

void GLES3Renderer::Init(bool createFrameBuffer) {
    std::cout << "Pht::Renderer: Initializing..." << std::endl;
    
    InitOpenGL(createFrameBuffer);
    InitShaders();
    InitCamera(ISceneManager::defaultNarrowFrustumHeightFactor);

    mRenderState.Init();
    mRenderState.SetCullFace(true);
    mTextRenderer = std::make_unique<GLES3TextRenderer>(mRenderState, mRenderBufferSize);

    std::cout << "Pht::Renderer: Using " << mRenderBufferSize.x << "x" << mRenderBufferSize.y
              << " resolution." << std::endl;
}

void GLES3Renderer::InitOpenGL(bool createFrameBuffer) {
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &mRenderBufferSize.x);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &mRenderBufferSize.y);

    if (createFrameBuffer) {        
        // Create a depth buffer.
        GLuint depthRenderbuffer {0};
        glGenRenderbuffers(1, &depthRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER,
                              GL_DEPTH_COMPONENT16,
                              mRenderBufferSize.x,
                              mRenderBufferSize.y);
        
        // Create the framebuffer object.
        GLuint framebuffer;
        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                                  GL_COLOR_ATTACHMENT0,
                                  GL_RENDERBUFFER,
                                  mColorRenderbuffer);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                                  GL_DEPTH_ATTACHMENT,
                                  GL_RENDERBUFFER,
                                  depthRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, mColorRenderbuffer);
        
        glViewport(0, 0, mRenderBufferSize.x, mRenderBufferSize.y);
    }
}

void GLES3Renderer::InitCamera() {
    mCamera = Camera {
        mRenderBufferSize,
        perspectiveFrustumSettings.mHeight * GetFrustumHeightFactor(),
        perspectiveFrustumSettings.mZNearClip,
        perspectiveFrustumSettings.mZFarClip
    };
    
    Vec3 cameraPosition {0.0f, 0.0f, 0.0f};
    Vec3 target {0.0f, 0.0f, -10.0f};
    Vec3 up {0.0f, 1.0f, 0.0f};
    mCamera.LookAt(cameraPosition, target, up);
    
    mOrthographicFrustumSize.y = orthographicFrustumSettings.mHeight * GetFrustumHeightFactor();
    mOrthographicFrustumSize.x = mOrthographicFrustumSize.y * mRenderBufferSize.x / mRenderBufferSize.y;
    mCamera.SetOrthographicProjection(mRenderBufferSize,
                                      mOrthographicFrustumSize.y,
                                      orthographicFrustumSettings.mZNearClip,
                                      orthographicFrustumSettings.mZFarClip);
}

void GLES3Renderer::InitHudFrustum() {
    mHudCameraPosition = Vec3 {0.0f, 0.0f, 0.0f};
    auto frustumHeight = hudFrustumSettings.mHeight * GetFrustumHeightFactor();
    float frustumWidth {frustumHeight * mRenderBufferSize.x / mRenderBufferSize.y};
    mHudFrustum.mProjection = Mat4::OrthographicProjection(-frustumWidth / 2.0f,
                                                           frustumWidth / 2.0f,
                                                           -frustumHeight / 2.0f,
                                                           frustumHeight / 2.0f,
                                                           hudFrustumSettings.mZNearClip,
                                                           hudFrustumSettings.mZFarClip);
    mHudFrustum.mSize = {frustumWidth, frustumHeight};
}

float GLES3Renderer::GetAspectRatio() const {
    return static_cast<float>(mRenderBufferSize.y) / static_cast<float>(mRenderBufferSize.x);
}

float GLES3Renderer::GetFrustumHeightFactor() const {
    if (GetAspectRatio() >= 2.0f) {
        return mNarrowFrustumHeightFactor;
    }
    
    return 1.0f;
}

void GLES3Renderer::InitShaders() {
    
    // Build shaders.
    GetShader(ShaderId::PixelLighting).Build(PixelLightingVertexShader, PixelLightingFragmentShader);
    GetShader(ShaderId::VertexLighting).Build(VertexLightingVertexShader, VertexLightingFragmentShader);
    GetShader(ShaderId::TexturedLighting).Build(TexturedLightingVertexShader, TexturedLightingFragmentShader);
    GetShader(ShaderId::TexturedLightingVertexColor).Build(TexturedLightingVertexColorVertexShader, TexturedLightingVertexColorFragmentShader);
    GetShader(ShaderId::TexturedPixelLighting).Build(TexturedPixelLightingVertexShader, TexturedPixelLightingFragmentShader);
    GetShader(ShaderId::TexturedEmissiveLighting).Build(TexturedEmissiveLightingVertexShader, TexturedEmissiveLightingFragmentShader);
    GetShader(ShaderId::TexturedEnvMapLighting).Build(TexturedEnvMapLightingVertexShader, TexturedEnvMapLightingFragmentShader);
    GetShader(ShaderId::Textured).Build(TexturedVertexShader, TexturedFragmentShader);
    GetShader(ShaderId::EnvMap).Build(EnvMapVertexShader, EnvMapFragmentShader);
    GetShader(ShaderId::VertexColor).Build(VertexColorVertexShader, VertexColorFragmentShader);
    GetShader(ShaderId::Particle).Build(ParticleVertexShader, ParticleFragmentShader);
    GetShader(ShaderId::ParticleTextureColor).Build(ParticleTextureColorVertexShader, ParticleTextureColorFragmentShader);
    GetShader(ShaderId::ParticleNoAlphaTexture).Build(ParticleNoAlphaTextureVertexShader, ParticleNoAlphaTextureFragmentShader);
    GetShader(ShaderId::PointParticle).Build(PointParticleVertexShader, PointParticleFragmentShader);
}

void GLES3Renderer::InitRenderQueue(const Scene& scene) {
    mRenderQueue.Init(scene.GetRoot());
}

void GLES3Renderer::CreateShader(ShaderId shaderId, const VertexFlags& vertexFlags) {
    auto shaderProgram = std::make_unique<GLES3ShaderProgram>(vertexFlags);
    mShaders.insert(std::make_pair(shaderId, std::move(shaderProgram)));
}

GLES3ShaderProgram& GLES3Renderer::GetShader(ShaderId shaderId) {
    auto shader = mShaders.find(shaderId);
    assert(shader != std::end(mShaders));
    return *shader->second;
}

const Vec3& GLES3Renderer::GetCameraPosition() const {
    if (mHudMode) {
        return mHudCameraPosition;
    }
    
    return mCamera.GetPosition();
}

int GLES3Renderer::GetAdjustedNumPixels(int numPixels) const {
    return numPixels * mRenderBufferSize.y / (defaultScreenHeight * GetFrustumHeightFactor());
}

void GLES3Renderer::InitCamera(float narrowFrustumHeightFactor) {
    mNarrowFrustumHeightFactor = narrowFrustumHeightFactor;

    InitCamera();
    InitHudFrustum();
}

const Mat4& GLES3Renderer::GetViewMatrix() const {
    if (mHudMode) {
        return identityMatrix;
    }
    
    return mCamera.GetViewMatrix();
}

const Mat4& GLES3Renderer::GetProjectionMatrix() const {
    if (mHudMode) {
        return mHudFrustum.mProjection;
    }
    
    switch (mProjectionMode) {
        case ProjectionMode::Perspective:
            return mCamera.GetProjectionMatrix();
        case ProjectionMode::Orthographic:
            return mCamera.GetOrthographicProjectionMatrix();
    }
}

const Vec2& GLES3Renderer::GetHudFrustumSize() const {
    return mHudFrustum.mSize;
}

const Vec2& GLES3Renderer::GetOrthographicFrustumSize() const {
    return mOrthographicFrustumSize;
}

const IVec2& GLES3Renderer::GetRenderBufferSize() const {
    return mRenderBufferSize;
}

float GLES3Renderer::GetTopPaddingHeight() const {
    if (GetAspectRatio() >= 2.0f) {
        return topPadding * mNarrowFrustumHeightFactor;
    }
    
    return 0.0f;
}

float GLES3Renderer::GetBottomPaddingHeight() const {
    if (GetAspectRatio() >= 2.0f) {
        return bottomPadding * mNarrowFrustumHeightFactor;
    }
    
    return 0.0f;
}

std::unique_ptr<RenderableObject>
GLES3Renderer::CreateRenderableObject(const IMesh& mesh,
                                      const Material& material,
                                      VertexBufferLocation bufferLocation) {
    auto& shaderProgram = GetShader(material.GetShaderId());
    return std::make_unique<RenderableObject>(material,
                                              mesh,
                                              shaderProgram.GetVertexFlags(),
                                              bufferLocation);
}

void GLES3Renderer::SetLightDirection(const Vec3& lightDirection) {
    mGlobalLight.mDirectionWorldSpace = lightDirection;
    CalculateCameraSpaceLightDirection();
}

void GLES3Renderer::CalculateCameraSpaceLightDirection() {
    // Since the matrix is row-major it has to be transposed in order to multiply with the vector.
    auto transposedViewMatrix = GetViewMatrix().Transposed();
    auto lightPosCamSpace = transposedViewMatrix * Vec4{mGlobalLight.mDirectionWorldSpace, 0.0f};
    Vec3 lightPosCamSpaceVec3 {lightPosCamSpace.x, lightPosCamSpace.y, lightPosCamSpace.z};
    mGlobalLight.mDirectionCameraSpace = lightPosCamSpaceVec3.Normalized();
}

void GLES3Renderer::EnableShader(ShaderId shaderId) {
    GetShader(shaderId).SetIsEnabled(true);
}

void GLES3Renderer::DisableShader(ShaderId shaderId) {
    GetShader(shaderId).SetIsEnabled(false);
}

void GLES3Renderer::SetClearColorBuffer(bool clearColorBuffer) {
    mClearColorBuffer = clearColorBuffer;
}

void GLES3Renderer::SetDepthTest(const DepthState& depthState) {
    if (!mIsDepthTestAllowed && !depthState.mDepthTestAllowedOverride) {
        mRenderState.SetDepthTest(false);
    } else {
        if (depthState.mDepthTest) {
            mRenderState.SetDepthTest(true);
        } else {
            mRenderState.SetDepthTest(false);
        }
    }
}

void GLES3Renderer::SetHudMode(bool hudMode) {
    mHudMode = hudMode;
}

void GLES3Renderer::SetProjectionMode(ProjectionMode projectionMode) {
    mProjectionMode = projectionMode;
}

void GLES3Renderer::SetScissorBox(const Vec2& lowerLeft, const Vec2& size) {
    assert(mHudMode || mProjectionMode == ProjectionMode::Orthographic);
    
    Vec4 localOrigin {0.0f, 0.0f, 0.0f, 1.0f};
    auto modelView = Mat4::Translate(lowerLeft.x, lowerLeft.y, 0.0f) * GetViewMatrix();
    auto modelViewProjection = modelView * GetProjectionMatrix();
    
    // Since the matrix is row-major it has to be transposed in order to multiply with the vector.
    auto clipSpacePos = modelViewProjection.Transposed() * localOrigin;
    auto normProjPos = clipSpacePos / clipSpacePos.w;
    
    auto& frustumSize = mHudMode ? GetHudFrustumSize() : GetOrthographicFrustumSize();
    
    Vec2 lowerLeftProjected {
        normProjPos.x * frustumSize.x / 2.0f,
        normProjPos.y * frustumSize.y / 2.0f
    };

    auto pixelX =
        mRenderBufferSize.x / 2.0f + mRenderBufferSize.x * lowerLeftProjected.x / frustumSize.x;
    
    auto pixelY =
        mRenderBufferSize.y / 2.0f + mRenderBufferSize.y * lowerLeftProjected.y / frustumSize.y;
    
    IVec2 lowerLeftInPixels {static_cast<int>(pixelX), static_cast<int>(pixelY)};
    
    IVec2 sizeInPixels {
        static_cast<int>(mRenderBufferSize.x * size.x / frustumSize.x),
        static_cast<int>(mRenderBufferSize.y * size.y / frustumSize.y)
    };
    
    glScissor(lowerLeftInPixels.x, lowerLeftInPixels.y, sizeInPixels.x, sizeInPixels.y);
}

void GLES3Renderer::ClearFrameBuffer() {
    // Depth writing has to be enabled in order to clear the depth buffer.
    mRenderState.SetDepthWrite(true);

    if (mClearColorBuffer) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    } else {
        glClear(GL_DEPTH_BUFFER_BIT);
    }
}

void GLES3Renderer::RenderScene(const Scene& scene, float frameSeconds) {
    const CameraComponent* previousCamera = nullptr;
    const LightComponent* previousLight = nullptr;
    
    IF_USING_FRAME_STATS(mRenderState.ResetFrameStats());
    
    for (auto& renderPass: scene.GetRenderPasses()) {
        if (!renderPass.IsEnabled()) {
            continue;
        }
        
        mRenderState.OnBeginRenderPass();

        // Set up camera.
        auto* cameraOverride = renderPass.GetCamera();
        auto* camera = cameraOverride ? cameraOverride : scene.GetCamera();
        assert(camera);
        if (camera != previousCamera) {
            auto cameraPositionWorldSpace = camera->GetSceneObject().GetWorldSpacePosition();
            if (renderPass.IsHudMode()) {
                mHudCameraPosition = cameraPositionWorldSpace;
            } else {
                mCamera.LookAt(cameraPositionWorldSpace, camera->GetTarget(), camera->GetUp());
            }
            
            CalculateCameraSpaceLightDirection();
            previousCamera = camera;
        }

        // Set up the lighting.
        auto* lightOverride = renderPass.GetLight();
        auto* light = lightOverride ? lightOverride : scene.GetGlobalLight();
        assert(light);
        if (light != previousLight) {
            SetLightDirection(light->GetDirection());
            mGlobalLight.mDirectionalIntensity = light->GetDirectionalIntensity();
            mGlobalLight.mAmbientIntensity = light->GetAmbientIntensity();
            previousLight = light;
        }
        
        // Render the pass.
        Render(renderPass, scene.GetDistanceFunction());
    }
    
    IF_USING_FRAME_STATS(mRenderState.LogFrameStats(frameSeconds));
}

void GLES3Renderer::Render(const RenderPass& renderPass, DistanceFunction distanceFunction) {
    auto isHudMode = renderPass.IsHudMode();
    auto projectionMode = renderPass.GetProjectionMode();
    if (isHudMode != mHudMode || projectionMode != mProjectionMode) {
        SetHudMode(isHudMode);
        SetProjectionMode(projectionMode);
    }
    
    auto& scissorBox = renderPass.GetScissorBox();
    if (scissorBox.HasValue()) {
        auto& scissorBoxValue = scissorBox.GetValue();
        SetScissorBox(scissorBoxValue.mLowerLeft, scissorBoxValue.mSize);
        mRenderState.SetScissorTest(true);
    } else {
        mRenderState.SetScissorTest(false);
    }
    
    mIsDepthTestAllowed = renderPass.IsDepthTestAllowed();
    
    // Build the render queue.
    mRenderQueue.Build(GetViewMatrix(),
                       renderPass.GetRenderOrder(),
                       distanceFunction,
                       renderPass.GetLayerMask());
    
    if (renderPass.MustRenderDepthWritingObjectsFirst()) {
        // Start by rendering the opaque objects (depth writing) and enable depth write for those.
        mRenderState.SetDepthWrite(true);
    }
    
    RenderQueue::Entry* previousEntry {nullptr};

    for (mRenderQueue.BeginIteration(); mRenderQueue.HasMoreEntries();) {
        auto& renderEntry = mRenderQueue.GetNextEntry();

        if (renderPass.MustRenderDepthWritingObjectsFirst()) {
            if (!renderEntry.IsDepthWriting()) {
                if (previousEntry == nullptr || previousEntry->IsDepthWriting()) {
                    // Transition into rendering the transparent objects (non-depth writing).
                    mRenderState.SetDepthWrite(false);
                }
            }
        } else {
            mRenderState.SetDepthWrite(renderEntry.IsDepthWriting());
        }
        
        auto* sceneObject = renderEntry.mSceneObject;
        if (auto* renderable = sceneObject->GetRenderable()) {
            RenderObject(*renderable, sceneObject->GetMatrix());
        }
        
        auto textKind = renderEntry.GetTextKind();
        if (textKind != RenderQueue::TextKind::None) {
            if (auto* textComponent = sceneObject->GetComponent<TextComponent>()) {
                auto textPosition = CalculateTextHudPosition(*textComponent);
                RenderText(textComponent->GetText(),
                           textPosition,
                           textKind,
                           textComponent->GetProperties());
            }
        }
        
        previousEntry = &renderEntry;
    }
}

void GLES3Renderer::RenderObject(const RenderableObject& renderableObject,
                                 const Mat4& modelTransform) {
    auto& material = renderableObject.GetMaterial();
    auto shaderId = material.GetShaderId();
    auto& shaderProgram = GetShader(shaderId);
    
    auto isShaderSameAsLastDraw = mRenderState.IsShaderInUse(shaderProgram);
    if (!isShaderSameAsLastDraw) {
        mRenderState.UseShader(shaderProgram);
    }
    
    SetTransforms(modelTransform, shaderProgram);
    
    if (!isShaderSameAsLastDraw || !mRenderState.IsMaterialInUse(material)) {
        mRenderState.UseMaterial(material);
        SetMaterialProperties(material, shaderId, shaderProgram);
    }
    
    auto& vbo = renderableObject.GetGpuVertexBuffer();
    if (!isShaderSameAsLastDraw || !mRenderState.IsVboInUse(vbo)) {
        mRenderState.UseVbo(vbo);
        SetVbo(renderableObject, shaderProgram);
    }

    switch (renderableObject.GetRenderMode()) {
        case RenderMode::Triangles:
            glDrawElements(GL_TRIANGLES, vbo.GetIndexCount(), GL_UNSIGNED_SHORT, 0);
            break;
        case RenderMode::Points:
            glDrawArrays(GL_POINTS, 0, vbo.GetPointCount());
            break;
    }
    
    IF_USING_FRAME_STATS(mRenderState.ReportDrawCall());
}

void GLES3Renderer::SetTransforms(const Mat4& modelTransform, GLES3ShaderProgram& shaderProgram) {
    // Note: the matrix in the matrix lib is row-major while OpenGL expects column-major. However,
    // it works since all transforms are created in row-major order while OpenGL reads the matrix in
    // column-major order which transposes it. Transposing the matrix is required in order to
    // multiply with a vector: M * v.
    auto modelview = modelTransform * GetViewMatrix();
    auto modelViewProjection = modelview * GetProjectionMatrix();
    auto& uniforms = shaderProgram.GetUniforms();
    glUniformMatrix4fv(uniforms.mModelViewProjection, 1, 0, modelViewProjection.Pointer());

    // Set the normal matrix in camera space. Modelview is orthogonal, so its Inverse-Transpose is
    // itself.
    auto normalMatrix = modelview.ToMat3();
    glUniformMatrix3fv(uniforms.mNormalMatrix, 1, 0, normalMatrix.Pointer());
    
    // Set the model matrix.
    glUniformMatrix4fv(uniforms.mModel, 1, 0, modelTransform.Pointer());
    glUniformMatrix3fv(uniforms.mModel3x3, 1, 0, modelTransform.ToMat3().Pointer());
    
    // Set the camera position in world space.
    shaderProgram.SetCameraPosition(GetCameraPosition());
    
    // Set light direction in camera space.
    shaderProgram.SetLightPosition(mGlobalLight.mDirectionCameraSpace);
}

void GLES3Renderer::SetMaterialProperties(const Material& material,
                                          ShaderId shaderId,
                                          const GLES3ShaderProgram& shaderProgram) {
    auto& uniforms = shaderProgram.GetUniforms();
    auto ambientlIntensity = mGlobalLight.mAmbientIntensity;
    auto& ambient = material.GetAmbient();
    glUniform3f(uniforms.mAmbientMaterial,
                ambient.mRed * ambientlIntensity,
                ambient.mGreen * ambientlIntensity,
                ambient.mBlue * ambientlIntensity);
    
    auto directionalIntensity = mGlobalLight.mDirectionalIntensity;
    auto& diffuse = material.GetDiffuse();
    glUniform3f(uniforms.mDiffuseMaterial,
                diffuse.mRed * directionalIntensity,
                diffuse.mGreen * directionalIntensity,
                diffuse.mBlue * directionalIntensity);
    
    auto& specular = material.GetSpecular();
    glUniform3f(uniforms.mSpecularMaterial,
                specular.mRed * directionalIntensity,
                specular.mGreen * directionalIntensity,
                specular.mBlue * directionalIntensity);

    auto& emissive = material.GetEmissive();
    glUniform3f(uniforms.mEmissiveMaterial, emissive.mRed, emissive.mGreen, emissive.mBlue);

    glUniform1f(uniforms.mShininess, material.GetShininess());
    glUniform1f(uniforms.mReflectivity, material.GetReflectivity());
    glUniform1f(uniforms.mOpacity, material.GetOpacity());
    
    glUniform1i(uniforms.mSampler, 0);
    glUniform1i(uniforms.mSecondSampler, 1);
    
    BindTextures(material, shaderId, shaderProgram);
    SetupBlend(material, shaderId);
    SetDepthTest(material.GetDepthState());
}

void GLES3Renderer::BindTextures(const Material& material,
                                 ShaderId shaderId,
                                 const GLES3ShaderProgram& shaderProgram) {
    switch (shaderId) {
        case ShaderId::EnvMap:
            mRenderState.BindTexture(GL_TEXTURE0,
                                     GL_TEXTURE_CUBE_MAP,
                                     material.GetEnvMapTexture()->GetHandles()->mGLHandle);
            break;
        case ShaderId::TexturedEmissiveLighting:
            mRenderState.BindTexture(GL_TEXTURE1,
                                     GL_TEXTURE_2D,
                                     material.GetEmissionTexture()->GetHandles()->mGLHandle);
            break;
        case ShaderId::TexturedEnvMapLighting:
            mRenderState.BindTexture(GL_TEXTURE1,
                                     GL_TEXTURE_CUBE_MAP,
                                     material.GetEnvMapTexture()->GetHandles()->mGLHandle);
            break;
        default:
            break;
    }
    
    if (shaderProgram.GetVertexFlags().mTextureCoords) {
        mRenderState.BindTexture(GL_TEXTURE0,
                                 GL_TEXTURE_2D,
                                 material.GetTexture()->GetHandles()->mGLHandle);
    }
}

void GLES3Renderer::SetupBlend(const Material& material, ShaderId shaderId) {
    switch (material.GetBlend()) {
        case Blend::Yes:
            mRenderState.SetBlend(true);
            if (IsParticleShader(shaderId)) {
                mRenderState.SetBlendFunc(GL_SRC_ALPHA, GL_ONE);
            } else {
                auto* texture = material.GetTexture();
                if (texture && texture->HasPremultipliedAlpha()) {
                    mRenderState.SetBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
                } else {
                    mRenderState.SetBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                }
            }
            break;
        case Blend::Additive:
            mRenderState.SetBlend(true);
            mRenderState.SetBlendFunc(GL_SRC_ALPHA, GL_ONE);
            break;
        case Blend::No:
            mRenderState.SetBlend(false);
            break;
    }
}

void GLES3Renderer::SetVbo(const RenderableObject& renderableObject,
                           const GLES3ShaderProgram& shaderProgram) {
    auto& vbo = renderableObject.GetGpuVertexBuffer();
    
    // Bind the vertex buffer.
    glBindBuffer(GL_ARRAY_BUFFER, vbo.GetHandles()->mGLVertexBufferHandle);
    
    // Enable vertex attribute arrays.
    EnableVertexAttributes(shaderProgram);
    
    if (renderableObject.GetRenderMode() == RenderMode::Triangles) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo.GetHandles()->mGLIndexBufferHandle);
    }
}

void GLES3Renderer::RenderText(const std::string& text,
                               const Vec2& position,
                               RenderQueue::TextKind textKind,
                               const TextProperties& properties) {
    auto textPosition = properties.mShadow == TextShadow::Yes ?
                        position + properties.mShadowOffset * properties.mScale :
                        position;

    switch (textKind) {
        case RenderQueue::TextKind::DoubleGradientShader: {
            GLES3TextRenderer::ColorProperties colorProperties {
                .mColor = properties.mColor,
                .mTopGradientColorSubtraction = properties.mTopGradientColorSubtraction,
                .mMidGradientColorSubtraction = properties.mMidGradientColorSubtraction
            };
            RenderTextImpl(text, textPosition, textKind, colorProperties, properties);
            break;
        }
        case RenderQueue::TextKind::TopGradientShader: {
            GLES3TextRenderer::ColorProperties colorProperties {
                .mColor = properties.mColor,
                .mTopGradientColorSubtraction = properties.mTopGradientColorSubtraction
            };
            RenderTextImpl(text, textPosition, textKind, colorProperties, properties);
            break;
        }
        case RenderQueue::TextKind::MidGradientShader: {
            GLES3TextRenderer::ColorProperties colorProperties {
                .mColor = properties.mColor,
                .mMidGradientColorSubtraction = properties.mMidGradientColorSubtraction
            };
            RenderTextImpl(text, textPosition, textKind, colorProperties, properties);
            break;
        }
        case RenderQueue::TextKind::PlainShader: {
            GLES3TextRenderer::ColorProperties colorProperties {
                .mColor = properties.mColor
            };
            RenderTextImpl(text, textPosition, textKind, colorProperties, properties);
            break;
        }
        case RenderQueue::TextKind::Specular: {
            GLES3TextRenderer::ColorProperties colorProperties {
                .mColor = properties.mSpecularColor
            };
            RenderTextImpl(text,
                           textPosition + properties.mSpecularOffset * properties.mScale,
                           textKind,
                           colorProperties,
                           properties);
            break;
        }
        case RenderQueue::TextKind::Shadow: {
            GLES3TextRenderer::ColorProperties colorProperties {
                .mColor = properties.mShadowColor
            };
            RenderTextImpl(text, position, textKind, colorProperties, properties);
            break;
        }
        case RenderQueue::TextKind::SecondShadow: {
            GLES3TextRenderer::ColorProperties colorProperties {
                .mColor = properties.mSecondShadowColor
            };
            RenderTextImpl(text,
                           position - properties.mSecondShadowOffset * properties.mScale,
                           textKind,
                           colorProperties,
                           properties);
            break;
        }
        case RenderQueue::TextKind::None:
            assert(false);
            break;
    }
}

void GLES3Renderer::RenderTextImpl(const std::string& text,
                                   const Vec2& position,
                                   RenderQueue::TextKind textKind,
                                   const GLES3TextRenderer::ColorProperties& colorProperties,
                                   const TextProperties& properties) {
    auto pixelX =
        mRenderBufferSize.x / 2.0f + mRenderBufferSize.x * position.x / mHudFrustum.mSize.x;
    
    auto pixelY =
        mRenderBufferSize.y / 2.0f + mRenderBufferSize.y * position.y / mHudFrustum.mSize.y;
    
    Vec2 pixelPosition {pixelX, pixelY};
    
    if (properties.mSnapToPixel == SnapToPixel::Yes) {
        pixelPosition.x = std::floor(pixelPosition.x);
        pixelPosition.y = std::floor(pixelPosition.y);
    }
    
    auto italicSlant =
        mRenderBufferSize.x * properties.mItalicSlant * properties.mScale / mHudFrustum.mSize.x;
    
    mTextRenderer->RenderText(text,
                              pixelPosition,
                              italicSlant,
                              textKind,
                              colorProperties,
                              properties);
}
                        
Vec2 GLES3Renderer::CalculateTextHudPosition(const TextComponent& textComponent) {
    auto& sceneObject = textComponent.GetSceneObject();
    
    if (mHudMode) {
        auto sceneObjectPosition = sceneObject.GetWorldSpacePosition();
        return Vec2 {sceneObjectPosition.x, sceneObjectPosition.y};
    }

    auto modelView = sceneObject.GetMatrix() * GetViewMatrix();
    auto modelViewProjection = modelView * GetProjectionMatrix();
    
    // Since the matrix is row-major it has to be transposed in order to multiply with the vector.
    auto clipSpacePos = modelViewProjection.Transposed() * modelSpaceOrigin;
    auto normProjPos = clipSpacePos / clipSpacePos.w;
    auto& hudSize = GetHudFrustumSize();

    return Pht::Vec2 {normProjPos.x * hudSize.x / 2.0f, normProjPos.y * hudSize.y / 2.0f};
}
