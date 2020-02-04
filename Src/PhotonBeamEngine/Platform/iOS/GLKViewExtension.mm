#import "GLKViewExtension.h"

namespace {
    Pht::TouchState toTouchState(UIGestureRecognizerState state) {
        switch (state) {
            case UIGestureRecognizerStateBegan:
                return Pht::TouchState::Begin;
            case UIGestureRecognizerStateChanged:
                return Pht::TouchState::Ongoing;
            case UIGestureRecognizerStateEnded:
                return Pht::TouchState::End;
            default:
                return Pht::TouchState::Other;
        }
    }
}

@implementation GLKViewExtension

- (void) touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event {
    UITouch* touch = [touches anyObject];
    CGPoint location = [touch locationInView:self];
    
    Pht::TouchEvent touchEvent {
        Pht::TouchState::Begin,
        Pht::Vec2{static_cast<float>(location.x), static_cast<float>(location.y)},
        {},
        {},
    };
    
    Pht::InputEvent inputEvent {touchEvent};
    mEngine->GetInputHandler().PushToQueue(inputEvent);
}

- (void) touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event {
    UITouch* touch = [touches anyObject];
    CGPoint location = [touch locationInView:self];
    
    Pht::TouchEvent touchEvent {
        Pht::TouchState::End,
        Pht::Vec2{static_cast<float>(location.x), static_cast<float>(location.y)},
        {},
        {},
    };
    
    Pht::InputEvent inputEvent {touchEvent};
    mEngine->GetInputHandler().PushToQueue(inputEvent);
}

- (void) touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event {
    UITouch* touch = [touches anyObject];
    CGPoint current = [touch locationInView:self];
    
    Pht::TouchEvent touchEvent {
        Pht::TouchState::Ongoing,
        Pht::Vec2{static_cast<float>(current.x), static_cast<float>(current.y)},
        {},
        {},
    };
    
    Pht::InputEvent inputEvent {touchEvent};
    mEngine->GetInputHandler().PushToQueue(inputEvent);
}

- (void) touchesCancelled:(NSSet<UITouch*>*)touches withEvent:(UIEvent*)event {
    [super touchesCancelled:touches withEvent:event];

    Pht::TouchEvent touchEvent {Pht::TouchState::Cancelled, {}, {}, {}};
    Pht::InputEvent inputEvent {touchEvent};
    mEngine->GetInputHandler().PushToQueue(inputEvent);
}

- (void) handleTap:(UITapGestureRecognizer*)recognizer {
    CGPoint location = [recognizer locationInView:self];
    
    Pht::TapGestureEvent tap {
        Pht::Vec2{static_cast<float>(location.x), static_cast<float>(location.y)}
    };
    
    Pht::InputEvent event {tap};
    mEngine->GetInputHandler().PushToQueue(event);
}

- (void) handlePan:(UIPanGestureRecognizer*)recognizer {
    CGPoint translation = [recognizer translationInView:self];
    CGPoint velocity = [recognizer velocityInView:self];
    Pht::TouchState state = toTouchState(recognizer.state);
    
    if (state != Pht::TouchState::Other) {
        Pht::PanGestureEvent pan {
            state,
            Pht::Vec2{static_cast<float>(translation.x), static_cast<float>(translation.y)},
            Pht::Vec2{static_cast<float>(velocity.x), static_cast<float>(velocity.y)}
        };
        
        Pht::InputEvent event {pan};
        mEngine->GetInputHandler().PushToQueue(event);
    }
}

- (void) setEngine:(Pht::Engine*)engine {
    mEngine = engine;
}

@end
