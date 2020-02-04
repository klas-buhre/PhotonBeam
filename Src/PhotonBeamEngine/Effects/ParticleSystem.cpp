#include "ParticleSystem.hpp"

#include "ParticleEffect.hpp"
#include "SceneObject.hpp"

using namespace Pht;

void ParticleSystem::AddParticleEffect(ParticleEffect& effect) {
    if (std::find(std::begin(mParticleEffects), std::end(mParticleEffects), &effect) ==
        std::end(mParticleEffects)) {

        mParticleEffects.push_back(&effect);
    }
}

void ParticleSystem::RemoveParticleEffect(ParticleEffect& effect) {
    mParticleEffects.erase(
        std::remove(std::begin(mParticleEffects), std::end(mParticleEffects), &effect),
        std::end(mParticleEffects));
}

std::unique_ptr<SceneObject>
ParticleSystem::CreateParticleEffectSceneObject(const ParticleSettings& particleSettings,
                                                const EmitterSettings& emitterSettings,
                                                RenderMode renderMode) {
    auto sceneObject = std::make_unique<SceneObject>();
    
    auto particleEffect = std::make_unique<ParticleEffect>(*sceneObject,
                                                           *this,
                                                           particleSettings,
                                                           emitterSettings,
                                                           renderMode);
    
    sceneObject->SetComponent<ParticleEffect>(std::move(particleEffect));
    return sceneObject;
}

void ParticleSystem::Update(float dt) {
    for (auto* effect: mParticleEffects) {
        effect->Update(dt);
    }
}
