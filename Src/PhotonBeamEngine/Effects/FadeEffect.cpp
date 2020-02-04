#include "FadeEffect.hpp"

#include "ISceneManager.hpp"
#include "Material.hpp"
#include "QuadMesh.hpp"
#include "IRenderer.hpp"
#include "Fnv1Hash.hpp"

using namespace Pht;

FadeEffect::FadeEffect(ISceneManager& sceneManager,
                       IRenderer& renderer,
                       float duration,
                       float midFade,
                       float zPosition,
                       const Vec4& color) :
    mRenderer {renderer},
    mMidFade {midFade},
    mFadeSpeed {midFade / duration} {
    
    Pht::Material quadMaterial;
    quadMaterial.SetDepthTest(false);
    
    auto& hudFrustumSize = mRenderer.GetHudFrustumSize();
    auto width = hudFrustumSize.x + 0.1f;
    auto height = hudFrustumSize.y + 0.1f;

    Pht::QuadMesh::Vertices vertices {
        {{-width / 2.0f, -height / 2.0f, 0.0f}, color},
        {{width / 2.0f, -height / 2.0f, 0.0f}, color},
        {{width / 2.0f, height / 2.0f, 0.0f}, color},
        {{-width / 2.0f, height / 2.0f, 0.0f}, color},
    };
    
    mQuad = sceneManager.CreateRenderableObject(Pht::QuadMesh {vertices}, quadMaterial);
    mSceneObject = std::make_unique<SceneObject>(mQuad.get());
    mSceneObject->SetName(Hash::Fnv1a("fadeEffect"));
    mSceneObject->GetTransform().SetPosition({0.0f, 0.0f, zPosition});
    mQuad->GetMaterial().SetOpacity(mFade);
    mSceneObject->SetIsVisible(false);
}

void FadeEffect::Reset() {
    mFade = 0.0f;
    mState = State::Idle;
    mQuad->GetMaterial().SetOpacity(mFade);
    mSceneObject->SetIsVisible(false);
}

void FadeEffect::Start() {
    mState = State::FadingOut;
    mSceneObject->SetIsVisible(true);
}

void FadeEffect::StartInMidFade() {
    mFade = mMidFade;
    mState = State::FadingIn;
    mSceneObject->SetIsVisible(true);
}

FadeEffect::State FadeEffect::Update(float dt) {
    switch (mState) {
        case State::FadingOut:
            mFade += mFadeSpeed * dt;
            if (mFade >= mMidFade) {
                mFade = mMidFade;
                mState = State::Transition;
            }
            mQuad->GetMaterial().SetOpacity(mFade);
            break;
        case State::Transition:
            mState = State::FadingIn;
            break;
        case State::FadingIn:
            mFade -= mFadeSpeed * dt;
            if (mFade <= 0.0f) {
                mFade = 0.0f;
                mState = State::Idle;
                mSceneObject->SetIsVisible(false);
            }
            mQuad->GetMaterial().SetOpacity(mFade);
            break;
        case State::Idle:
            break;
    }
    
    return mState;
}

void FadeEffect::UpdateFadeOut(float dt) {
    if (mState == State::FadingOut) {
        Update(dt);
    }
}

bool FadeEffect::IsFadingOut() const {
    return mState == State::FadingOut;
}

void FadeEffect::SetDuration(float duration) {
    mFadeSpeed = mMidFade / duration;
}
