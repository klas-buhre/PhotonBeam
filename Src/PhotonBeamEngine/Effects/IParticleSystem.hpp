#ifndef IParticleSystem_hpp
#define IParticleSystem_hpp

#include <memory>

#include "RenderableObject.hpp"

namespace Pht {
    class ParticleEffect;
    class ParticleSettings;
    class EmitterSettings;
    class SceneObject;
    
    class IParticleSystem {
    public:
        virtual ~IParticleSystem() {}

        virtual void AddParticleEffect(ParticleEffect& effect) = 0;
        virtual void RemoveParticleEffect(ParticleEffect& effect) = 0;
        virtual std::unique_ptr<SceneObject>
            CreateParticleEffectSceneObject(const ParticleSettings& particleSettings,
                                            const EmitterSettings& emitterSettings,
                                            RenderMode renderMode) = 0;
    };
}

#endif
