#import "AppDelegate.h"

#import <AVFoundation/AVFoundation.h>

@implementation AppDelegate

- (BOOL) application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    CGRect screenBounds = [[UIScreen mainScreen] bounds];
    mWindow = [[UIWindow alloc] initWithFrame:screenBounds];
    
    mViewController = [[GLKViewControllerExtension alloc] initWithNibName:nil bundle:nil];
    [mViewController createGLContext];
    [mWindow setRootViewController:mViewController];
    
    mView = [[GLKViewExtension alloc] initWithFrame:screenBounds
                                      context:[mViewController getContext]];
    mView.userInteractionEnabled = YES;
    
    mViewController.view = mView;
    [mViewController configureView];
    
    [mWindow makeKeyAndVisible];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                          selector:@selector(handleAudioSessionInterruption:)
                                          name:AVAudioSessionInterruptionNotification
                                          object:[AVAudioSession sharedInstance]];    
    return YES;
}

- (void) handleAudioSessionInterruption:(NSNotification*)event {
    Pht::Engine* engine = [mViewController getEngine];
    NSUInteger type = [[[event userInfo] objectForKey:AVAudioSessionInterruptionTypeKey] unsignedIntegerValue];
    
    switch (type) {
        case AVAudioSessionInterruptionTypeBegan:
            if (engine) {
                engine->GetAudioSystem().OnAudioSessionInterrupted();
            }
            break;
        case AVAudioSessionInterruptionTypeEnded:
            if ([[UIApplication sharedApplication] applicationState] == UIApplicationStateActive) {
                if (engine) {
                    engine->GetAudioSystem().OnAudioSessionInterruptionEnded();
                }
            } else {
                // Have to wait for the app to become active, otherwise the audio session won’t
                // resume correctly.
            }
            break;
    }
}

- (void) applicationDidBecomeActive:(UIApplication*)application {
    Pht::Engine* engine = [mViewController getEngine];
    
    if (engine) {
        engine->GetAudioSystem().OnApplicationBecameActive();
    }
}

@end
