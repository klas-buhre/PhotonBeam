#define GLES_SILENCE_DEPRECATION

#import <GLKit/GLKit.h>

#import "GLKViewControllerExtension.h"
#import "Engine.hpp"

@interface GLKViewExtension : GLKView {
    Pht::Engine* mEngine;
}

- (void) setEngine: (Pht::Engine*) engine;

@end
