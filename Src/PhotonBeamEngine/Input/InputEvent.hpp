#ifndef InputEvent_hpp
#define InputEvent_hpp

#include "Vector.hpp"

namespace Pht {
    enum class TouchState {
        Begin,
        Ongoing,
        End,
        Cancelled,
        Other
    };
    
    struct TouchEvent {
        TouchState mState;
        Vec2 mLocation;
        Vec2 mTranslation;
        Vec2 mVelocity;        
    };

    struct TapGestureEvent {
        Vec2 mLocation;
    };
    
    struct PanGestureEvent {
        TouchState mState;
        Vec2 mTranslation;
        Vec2 mVelocity;
    };
    
    enum class InputKind {
        Touch,
        TapGesture,
        PanGesture
    };
    
    class InputEvent {
    public:
        InputEvent();
        explicit InputEvent(const TouchEvent& event);
        explicit InputEvent(const TapGestureEvent& event);
        explicit InputEvent(const PanGestureEvent& event);
        
        const TouchEvent& GetTouchEvent() const;
        const TapGestureEvent& GetTapGestureEvent() const;
        const PanGestureEvent& GetPanGestureEvent() const;
        
        InputKind GetKind() const {
            return mKind;
        }
    
    private:
        friend class InputHandler;
    
        InputKind mKind;
        union {
            TouchEvent mTouch;
            TapGestureEvent mTap;
            PanGestureEvent mPan;
        };
    };
}

#endif
