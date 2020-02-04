#ifndef IEngine_hpp
#define IEngine_hpp

namespace Pht {
    class IRenderer;
    class IInput;
    class IAudio;
    class ISceneManager;
    class IAnimationSystem;
    class IParticleSystem;
    class IAnalytics;

    class IEngine {
    public:
        virtual ~IEngine() {}
        
        virtual IRenderer& GetRenderer() = 0;
        virtual IInput& GetInput() = 0;
        virtual IAudio& GetAudio() = 0;
        virtual ISceneManager& GetSceneManager() = 0;
        virtual IAnimationSystem& GetAnimationSystem() = 0;
        virtual IParticleSystem& GetParticleSystem() = 0;
        virtual IAnalytics& GetAnalytics() = 0;
        virtual float GetLastFrameSeconds() const = 0;
    };
}

#endif
