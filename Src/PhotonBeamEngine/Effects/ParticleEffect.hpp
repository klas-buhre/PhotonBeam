#ifndef ParticleEffect_hpp
#define ParticleEffect_hpp

#include <vector>
#include <memory>

#include "VertexBuffer.hpp"
#include "ParticleEmitter.hpp"
#include "RenderableObject.hpp"
#include "ISceneObjectComponent.hpp"

namespace Pht {
    class SceneObject;
    class IParticleSystem;
    
    class ParticleEffect: public ISceneObjectComponent {
    public:
        static const ComponentId id;
        
        ParticleEffect(SceneObject& sceneObject,
                       IParticleSystem& particleSystem,
                       const ParticleSettings& particleSettings,
                       const EmitterSettings& emitterSettings,
                       RenderMode renderMode);
        ~ParticleEffect();
        
        void Update(float dt);
        void Start();
        void Stop();
        
        bool IsActive() const {
            return mIsActive;
        }
        
        ParticleEmitter& GetEmitter() {
            return mEmitter;
        }
        
        SceneObject& GetSceneObject() {
            return mSceneObject;
        }

        const SceneObject& GetSceneObject() const {
            return mSceneObject;
        }

    private:
        bool UpdateParticle(Particle& particle, float dt);
        void ResetParticles();
        void WriteVertexBuffer();
        void WritePoints();
        void WriteTriangles();
        void WriteParticleTriangles(const Particle& particle);
        
        SceneObject& mSceneObject;
        IParticleSystem& mParticleSystem;
        ParticleEmitter mEmitter;
        RenderMode mRenderMode {RenderMode::Triangles};
        std::vector<Particle> mParticles;
        VertexBuffer* mVertexBuffer {nullptr};
        std::unique_ptr<RenderableObject> mRenderableObject;
        bool mIsActive {false};
    };
}

#endif
