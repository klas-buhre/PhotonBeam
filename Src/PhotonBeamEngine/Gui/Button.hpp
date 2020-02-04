#ifndef Button_hpp
#define Button_hpp

#include <functional>

#include "Vector.hpp"

namespace Pht {
    class TouchEvent;
    class IEngine;
    class SceneObject;

    class Button {
    public:
        enum class Result {
            Down,
            UpInside,
            UpOutside,
            MoveOutside,
            MoveInside,
            None
        };

        Button(SceneObject& sceneObject, const Vec2& size, IEngine& engine);
        virtual ~Button() {}

        bool IsClicked(const TouchEvent& event);
        Result OnTouch(const TouchEvent& event);
        void Reset();
        bool StateIsDownOrMovedOutside() const;
        bool IsDown() const;
        
        void SetSize(const Vec2& size) {
            mSize = size;
        }
        
        void SetOnDown(const std::function<void()>& onDown) {
            mOnDown = onDown;
        }

        void SetOnUpInside(const std::function<void()>& onUpInside) {
            mOnUpInside = onUpInside;
        }
        
        void SetOnUpOutside(const std::function<void()>& onUpOutside) {
            mOnUpOutside = onUpOutside;
        }

        void SetOnMoveOutside(const std::function<void()>& onMoveOutside) {
            mOnMoveOutside = onMoveOutside;
        }
        
    private:
        Result OnTouchBegin(const Vec2& touchLocation);
        Result OnTouchMove(const Vec2& touchLocation);
        Result OnTouchEnd(const Vec2& touchLocation);
        bool Hit(const Vec2& touchLocation);

        enum class State {
            Up,
            Down,
            MovedOutside
        };

        IEngine& mEngine;
        State mState {State::Up};
        SceneObject& mSceneObject;
        Vec2 mSize;
        std::function<void()> mOnDown;
        std::function<void()> mOnUpInside;
        std::function<void()> mOnUpOutside;
        std::function<void()> mOnMoveOutside;
    };
}

#endif
