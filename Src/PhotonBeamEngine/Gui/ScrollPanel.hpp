#ifndef ScrollPanel_hpp
#define ScrollPanel_hpp

#include <memory>

#include "Vector.hpp"
#include "SceneObject.hpp"

namespace Pht {
    class IEngine;
    class TouchEvent;
    
    class ScrollPanel {
    public:
        ScrollPanel(IEngine& engine,
                    const Vec2& size,
                    float dampingCoefficient,
                    float cutoffVelocity);
        virtual ~ScrollPanel() {}
        
        void SetPosition(const Vec3& position);
        void AddSceneObject(SceneObject& sceneObject);
        void Init();
        void OnTouch(const TouchEvent& event);
        void Update();
                
        Pht::SceneObject& GetRoot() {
            return *mRoot;
        }
        
    private:
        void UpdateIsTouchingState(const Pht::TouchEvent& touch);
        void SetYScrollPosition(float yPosition);
        void HideObjectsOutsideView();
        
        IEngine& mEngine;
        Vec2 mSize {0.0f, 0.0f};
        float mDampingCoefficient;
        float mCutoffVelocity;
        std::unique_ptr<Pht::SceneObject> mRoot;
        std::unique_ptr<Pht::SceneObject> mPanel;
        bool mIsTouching {false};
        Vec2 mTouchLocationAtScrollBegin {0.0f, 0.0f};
        float mPanelYPositionAtScrollBegin {0.0f};
        float mPanelYVelocity {0.0f};
        float mSceneObjectsYMin {100.0f};
    };
}

#endif
