#include "SwipeGestureRecognizer.hpp"

#include <cmath>

#include "InputEvent.hpp"

using namespace Pht;

SwipeGestureRecognizer::SwipeGestureRecognizer(SwipeDirection direction,
                                               float swipeDistanceThreshold) :
    mDirection {direction},
    mSwipeDistanceThreshold {swipeDistanceThreshold} {}

void SwipeGestureRecognizer::TouchBegin() {
    mSwipeDistance = 0.0f;
    mPreviousPointY = 0.0f;
    mPreviousPointX = 0.0f;
}

IsSwipe SwipeGestureRecognizer::IsTouchContainingSwipe(const Pht::TouchEvent& touchEvent) {
    switch (mDirection) {
        case SwipeDirection::Up:
            return IsTouchContainingSwipeUp(touchEvent);
        case SwipeDirection::Down:
            return IsTouchContainingSwipeDown(touchEvent);
        case SwipeDirection::Left:
            return IsTouchContainingSwipeLeft(touchEvent);
        case SwipeDirection::Right:
            return IsTouchContainingSwipeRight(touchEvent);
    }
}

IsSwipe SwipeGestureRecognizer::IsTouchContainingSwipeUp(const Pht::TouchEvent& touchEvent) {
    auto result = IsSwipe::No;
    
    if (-touchEvent.mVelocity.y > std::abs(touchEvent.mVelocity.x)) {
        auto dist = mPreviousPointY - touchEvent.mTranslation.y;
        mSwipeDistance += dist;
        
        if (mSwipeDistance >= mSwipeDistanceThreshold) {
            result = IsSwipe::Yes;
        } else {
            result = IsSwipe::Maybe;
        }
    } else {
        mSwipeDistance = 0.0f;
    }

    mPreviousPointY = touchEvent.mTranslation.y;
    return result;
}

IsSwipe SwipeGestureRecognizer::IsTouchContainingSwipeDown(const Pht::TouchEvent& touchEvent) {
    auto result = IsSwipe::No;
    
    if (touchEvent.mVelocity.y > std::abs(touchEvent.mVelocity.x)) {
        auto dist = touchEvent.mTranslation.y - mPreviousPointY;
        mSwipeDistance += dist;
        
        if (mSwipeDistance >= mSwipeDistanceThreshold) {
            result = IsSwipe::Yes;
        } else {
            result = IsSwipe::Maybe;
        }
    } else {
        mSwipeDistance = 0.0f;
    }

    mPreviousPointY = touchEvent.mTranslation.y;
    return result;
}

IsSwipe SwipeGestureRecognizer::IsTouchContainingSwipeLeft(const Pht::TouchEvent& touchEvent) {
    auto result = IsSwipe::No;
    
    if (-touchEvent.mVelocity.x > std::abs(touchEvent.mVelocity.y)) {
        auto dist = mPreviousPointX - touchEvent.mTranslation.x;
        mSwipeDistance += dist;
        
        if (mSwipeDistance >= mSwipeDistanceThreshold) {
            result = IsSwipe::Yes;
        } else {
            result = IsSwipe::Maybe;
        }
    } else {
        mSwipeDistance = 0.0f;
    }

    mPreviousPointX = touchEvent.mTranslation.x;
    return result;
}

IsSwipe SwipeGestureRecognizer::IsTouchContainingSwipeRight(const Pht::TouchEvent& touchEvent) {
    auto result = IsSwipe::No;
    
    if (touchEvent.mVelocity.x > std::abs(touchEvent.mVelocity.y)) {
        auto dist = touchEvent.mTranslation.x - mPreviousPointX;
        mSwipeDistance += dist;
        
        if (mSwipeDistance >= mSwipeDistanceThreshold) {
            result = IsSwipe::Yes;
        } else {
            result = IsSwipe::Maybe;
        }
    } else {
        mSwipeDistance = 0.0f;
    }

    mPreviousPointX = touchEvent.mTranslation.x;
    return result;
}
