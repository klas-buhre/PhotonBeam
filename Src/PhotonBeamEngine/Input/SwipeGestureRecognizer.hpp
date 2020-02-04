#ifndef SwipeGestureRecognizer_hpp
#define SwipeGestureRecognizer_hpp

namespace Pht {
    class TouchEvent;
    
    enum class IsSwipe {
        Yes,
        Maybe,
        No
    };
    
    enum class SwipeDirection {
        Up,
        Down,
        Left,
        Right
    };
    
    class SwipeGestureRecognizer {
    public:
        SwipeGestureRecognizer(SwipeDirection direction, float swipeDistanceThreshold);
        
        void TouchBegin();
        IsSwipe IsTouchContainingSwipe(const Pht::TouchEvent& touchEvent);
        
    private:
        IsSwipe IsTouchContainingSwipeUp(const Pht::TouchEvent& touchEvent);
        IsSwipe IsTouchContainingSwipeDown(const Pht::TouchEvent& touchEvent);
        IsSwipe IsTouchContainingSwipeLeft(const Pht::TouchEvent& touchEvent);
        IsSwipe IsTouchContainingSwipeRight(const Pht::TouchEvent& touchEvent);
        
        SwipeDirection mDirection;
        const float mSwipeDistanceThreshold;
        float mSwipeDistance {0.0f};
        float mPreviousPointY {0.0f};
        float mPreviousPointX {0.0f};
    };
}

#endif
