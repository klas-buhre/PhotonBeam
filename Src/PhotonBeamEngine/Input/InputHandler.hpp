#ifndef InputHandler_hpp
#define InputHandler_hpp

#include <vector>

#include "Vector.hpp"
#include "InputEvent.hpp"
#include "IInput.hpp"

namespace Pht {
    class IRenderer;

    class InputHandler: public IInput {
    public:
        InputHandler(const Vec2& nativeScreenInputSize);
        
        void SetUseGestureRecognizers(bool useGestureRecognizers) override;
        void EnableInput() override;
        void DisableInput() override;
        bool HasEvents() const override;
        const InputEvent& GetNextEvent() const override;
        void PopNextEvent() override;
        bool ConsumeWholeTouch() override;
        const Vec2& GetScreenInputSize() const override;

        void Init(const IRenderer& renderer);
        void PushToQueue(InputEvent& event);
        
        bool GetUseGestureRecognizers() const {
            return mUseGestureRecognizers;
        }
        
    private:
        void ProcessInputEvent(InputEvent& event);
        void ProcessTouchEvent(TouchEvent& event);
        Vec2 NativeToStandardCoordinates(const Vec2& nativeLocation);
    
        bool mIsInputEnabled {true};
        bool mUseGestureRecognizers {true};
        std::vector<InputEvent> mEventQueue;
        int mQueueReadIndex {0};
        int mQueueWriteIndex {0};
        Vec2 mNativeScreenInputSize;
        Vec2 mScreenInputSize;
        Vec2 mTouchBeginLocation;
        Vec2 mTouchPreviousLocation;
        Vec2 mPreviousVelocity;
    };
}

#endif
