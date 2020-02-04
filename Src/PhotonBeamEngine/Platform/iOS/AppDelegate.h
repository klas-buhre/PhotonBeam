#import "GLKViewControllerExtension.h"
#import "GLKViewExtension.h"

@interface AppDelegate : NSObject <UIApplicationDelegate> {
@private
    UIWindow* mWindow;
    GLKViewControllerExtension* mViewController;
    GLKViewExtension* mView;
}

@end
