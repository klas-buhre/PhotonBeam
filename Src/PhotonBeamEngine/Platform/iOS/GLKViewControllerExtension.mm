#import "GLKViewControllerExtension.h"
#import "GLKViewExtension.h"

@implementation GLKViewControllerExtension

- (void) createGLContext {
    EAGLRenderingAPI api = kEAGLRenderingAPIOpenGLES3;
    mContext = [[EAGLContext alloc] initWithAPI:api];
    
    if (!mContext || ![EAGLContext setCurrentContext:mContext]) {
        std::cout << "Could not create EAGLContext." << std::endl;
    }
}

- (void) configureView {
    GLKView* view = (GLKView*) self.view;
 
    // Configure renderbuffers created by the view.
    view.drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat16;
    view.enableSetNeedsDisplay = YES;
 
    self.preferredFramesPerSecond = 60;
}

- (void) initEngine {
    CGRect screenBounds = [[UIScreen mainScreen] bounds];
    
    Pht::Vec2 screenInputSize {
        static_cast<float>(screenBounds.size.width),
        static_cast<float>(screenBounds.size.height)
    };
    
    mEngine = new Pht::Engine(false, screenInputSize);
    mEngine->Init(false);
    
    GLKViewExtension* glkViewExtension = (GLKViewExtension*)self.view;
    [glkViewExtension setEngine: mEngine];
    
#if 0
    if (mEngine->GetInputHandler().GetUseGestureRecognizers()) {
        UITapGestureRecognizer* tapGestureRecognizer =
            [[UITapGestureRecognizer alloc] initWithTarget:glkViewExtension
                                            action:@selector(handleTap:)];
        [glkViewExtension addGestureRecognizer:tapGestureRecognizer];
        
        UIPanGestureRecognizer* panGestureRecognizer =
            [[UIPanGestureRecognizer alloc] initWithTarget:glkViewExtension
                                            action:@selector(handlePan:)];
        [glkViewExtension addGestureRecognizer:panGestureRecognizer];
    }
#endif

    mTimestamp = CACurrentMediaTime();
}

- (BOOL) prefersStatusBarHidden {
    return YES;
}

- (UIRectEdge) preferredScreenEdgesDeferringSystemGestures {
    return UIRectEdgeAll;
}

- (void) glkView:(GLKView*)view drawInRect:(CGRect)rect {
    if (!mEngine) {
        [self initEngine];
    }
    
    double timeStamp = CACurrentMediaTime();
    mEngine->Update(timeStamp - mTimestamp);
    mTimestamp = timeStamp;
    
    // Trick iOS to keep the touch input rate at 60 Hz. See:
    // https://forums.developer.apple.com/thread/62315
    dispatch_async(dispatch_get_main_queue(), ^{
        [(GLKView*)self.view setNeedsDisplay];
    });
}

- (EAGLContext*) getContext {
    return mContext;
}

- (Pht::Engine*) getEngine {
    return mEngine;
}

@end
