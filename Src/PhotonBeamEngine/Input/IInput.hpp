#ifndef IInput_hpp
#define IInput_hpp

#include "Vector.hpp"

namespace Pht {
    class InputEvent;

    class IInput {
    public:
        virtual ~IInput() {}
        
        virtual void SetUseGestureRecognizers(bool useGestureRecognizers) = 0;
        virtual void EnableInput() = 0;
        virtual void DisableInput() = 0;
        virtual bool HasEvents() const = 0;
        virtual const InputEvent& GetNextEvent() const = 0;
        virtual void PopNextEvent() = 0;
        virtual bool ConsumeWholeTouch() = 0;
        virtual const Vec2& GetScreenInputSize() const = 0;
    };
}

#endif
