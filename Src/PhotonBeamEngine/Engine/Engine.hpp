#ifndef Engine_hpp
#define Engine_hpp

#include <memory>

#include "IEngine.hpp"
#include "IRendererInternal.hpp"
#include "InputHandler.hpp"
#include "Audio.hpp"
#include "SceneManager.hpp"
#include "AnimationSystem.hpp"
#include "ParticleSystem.hpp"
#include "IAnalytics.hpp"

namespace Pht {
    class IApplication;
    
    class Engine: public IEngine {
    public:
        Engine(bool createFrameBuffer, const Vec2& screenInputSize);
        
        IRenderer& GetRenderer() override;
        IInput& GetInput() override;
        IAudio& GetAudio() override;
        ISceneManager& GetSceneManager() override;
        IAnimationSystem& GetAnimationSystem() override;
        IParticleSystem& GetParticleSystem() override;
        IAnalytics& GetAnalytics() override;
        float GetLastFrameSeconds() const override;
        
        void Init(bool createFrameBuffer);
        void Update(float frameSeconds);
        
        Audio& GetAudioSystem() {
            return mAudio;
        }
        
        InputHandler& GetInputHandler() {
            return mInputHandler;
        }
        
    private:
        void HandleSceneTransition(Scene& newScene);
        
        std::unique_ptr<IRendererInternal> mRenderer;
        InputHandler mInputHandler;
        Audio mAudio;
        SceneManager mSceneManager;
        AnimationSystem mAnimationSystem;
        ParticleSystem mParticleSystem;
        std::unique_ptr<IAnalytics> mAnalytics;
        std::unique_ptr<IApplication> mApplication;
        float mLastFrameSeconds {0.0f};
        Scene* mScene {nullptr};
    };
}

#endif
