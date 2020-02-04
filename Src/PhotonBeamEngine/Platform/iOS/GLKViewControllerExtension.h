#define GLES_SILENCE_DEPRECATION

#import <GLKit/GLKit.h>

#import "Engine.hpp"

@interface GLKViewControllerExtension : GLKViewController {
    EAGLContext* mContext;
    Pht::Engine* mEngine;
    double mTimestamp;
}

- (void) createGLContext;
- (void) configureView;
- (EAGLContext*) getContext;
- (Pht::Engine*) getEngine;

@end
