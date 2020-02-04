#include "InputEvent.hpp"

#include <assert.h>

using namespace Pht;

InputEvent::InputEvent() :
    mKind {InputKind::Touch} {}

InputEvent::InputEvent(const TouchEvent& event) :
    mKind {InputKind::Touch},
    mTouch {event} {}

InputEvent::InputEvent(const TapGestureEvent& event) :
    mKind {InputKind::TapGesture},
    mTap {event} {}

InputEvent::InputEvent(const PanGestureEvent& event) :
    mKind {InputKind::PanGesture},
    mPan {event} {}

const TouchEvent& InputEvent::GetTouchEvent() const {
    assert(mKind == InputKind::Touch);
    return mTouch;
}

const TapGestureEvent& InputEvent::GetTapGestureEvent() const {
    assert(mKind == InputKind::TapGesture);
    return mTap;
}

const PanGestureEvent& InputEvent::GetPanGestureEvent() const {
    assert(mKind == InputKind::PanGesture);
    return mPan;
}
