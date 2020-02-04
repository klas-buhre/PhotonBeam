#ifndef ParticleSystem_hpp
#define ParticleSystem_hpp

#include <vector>

#include "IParticleSystem.hpp"

namespace Pht {
    class ParticleSystem: public IParticleSystem {
    public:
        void AddParticleEffect(ParticleEffect& effect) override;
        void RemoveParticleEffect(ParticleEffect& effect) override;
        std::unique_ptr<SceneObject>
            CreateParticleEffectSceneObject(const ParticleSettings& particleSettings,
                                            const EmitterSettings& emitterSettings,
                                            RenderMode renderMode) override;
        
        void Update(float dt);
        
    private:
        std::vector<ParticleEffect*> mParticleEffects;
    };
}

#endif
