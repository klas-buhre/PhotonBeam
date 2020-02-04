#include "InputHandler.hpp"

#include <assert.h>

#include "IRenderer.hpp"

using namespace Pht;

namespace {
    constexpr auto eventQueueMaxSize {1000};
    const Vec2 defaultScreenInputSize {320.0f, 568.0f};
}

InputHandler::InputHandler(const Vec2& nativeScreenInputSize) :
    mNativeScreenInputSize {nativeScreenInputSize} {
    
    mEventQueue.resize(eventQueueMaxSize);
}

void InputHandler::Init(const IRenderer& renderer) {
    auto nativeWhRatio = mNativeScreenInputSize.x / mNativeScreenInputSize.y;
    auto defaultWhRatio = defaultScreenInputSize.x / defaultScreenInputSize.y;
    
    mScreenInputSize.x = nativeWhRatio * defaultScreenInputSize.x / defaultWhRatio;
    mScreenInputSize.y = defaultScreenInputSize.y;
    mScreenInputSize *= renderer.GetFrustumHeightFactor();
}

void InputHandler::SetUseGestureRecognizers(bool useGestureRecognizers) {
    mUseGestureRecognizers = useGestureRecognizers;
}

void InputHandler::EnableInput() {
    mQueueReadIndex = 0;
    mQueueWriteIndex = 0;
    mIsInputEnabled = true;
}

void InputHandler::DisableInput() {
    mQueueReadIndex = 0;
    mQueueWriteIndex = 0;
    mIsInputEnabled = false;
}

void InputHandler::PushToQueue(InputEvent& event) {
    if (!mIsInputEnabled) {
        return;
    }
    
    ProcessInputEvent(event);
    mEventQueue[mQueueWriteIndex % eventQueueMaxSize] = event;
    
    ++mQueueWriteIndex;
    assert(mQueueWriteIndex - mQueueReadIndex <= eventQueueMaxSize);
}

bool InputHandler::HasEvents() const {
    return mIsInputEnabled && (mQueueReadIndex != mQueueWriteIndex);
}

const InputEvent& InputHandler::GetNextEvent() const {
    assert(mIsInputEnabled);
    return mEventQueue[mQueueReadIndex % eventQueueMaxSize];
}

void InputHandler::PopNextEvent() {
    if (mIsInputEnabled) {
        ++mQueueReadIndex;
    }
}

bool InputHandler::ConsumeWholeTouch() {
    auto gotTouch = false;

    if (!mIsInputEnabled) {
        return gotTouch;
    }
    
    while (HasEvents()) {
        auto& event = GetNextEvent();
        switch (event.GetKind()) {
            case Pht::InputKind::Touch:
                if (event.GetTouchEvent().mState == Pht::TouchState::End) {
                    gotTouch = true;
                }
                break;
            default:
                assert(false);
                break;
        }
        
        PopNextEvent();
        
        if (gotTouch) {
            break;
        }
    }
    
    return gotTouch;
}

const Vec2& InputHandler::GetScreenInputSize() const {
    return mScreenInputSize;
}

void InputHandler::ProcessInputEvent(InputEvent& event) {
    switch (event.GetKind()) {
        case InputKind::Touch: {
            auto& touchEvent = event.mTouch;
            touchEvent.mLocation = NativeToStandardCoordinates(touchEvent.mLocation);
            ProcessTouchEvent(touchEvent);
            break;
        }
        case InputKind::TapGesture: {
            auto& tapEvent = event.mTap;
            tapEvent.mLocation = NativeToStandardCoordinates(tapEvent.mLocation);
            break;
        }
        case InputKind::PanGesture: {
            auto& panEvent = event.mPan;
            panEvent.mTranslation = NativeToStandardCoordinates(panEvent.mTranslation);
            break;
        }
    }
}

void InputHandler::ProcessTouchEvent(TouchEvent& event) {
    switch (event.mState) {
        case TouchState::Begin:
            mTouchBeginLocation = event.mLocation;
            event.mTranslation = {0.0f, 0.0f};
            event.mVelocity = {0.0f, 0.0f};
            break;
        case TouchState::Ongoing:
            event.mTranslation = event.mLocation - mTouchBeginLocation;
            event.mVelocity = event.mLocation - mTouchPreviousLocation;
            break;
        case TouchState::End:
            event.mTranslation = event.mLocation - mTouchBeginLocation;
            if (event.mLocation == mTouchPreviousLocation) {
                event.mVelocity = mPreviousVelocity;
            } else {
                event.mVelocity = event.mLocation - mTouchPreviousLocation;
            }
            break;
        default:
            break;
    }
    
    mTouchPreviousLocation = event.mLocation;
    mPreviousVelocity = event.mVelocity;
}

Vec2 InputHandler::NativeToStandardCoordinates(const Vec2& nativeLocation) {
    return {
        nativeLocation.x * mScreenInputSize.x / mNativeScreenInputSize.x,
        nativeLocation.y * mScreenInputSize.y / mNativeScreenInputSize.y
    };
}
