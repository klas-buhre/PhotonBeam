#import <Foundation/Foundation.h>
#import <string>

namespace Pht {
    namespace FileSystem {
        std::string GetResourceDirectory() {
            NSString* bundlePath = [[NSBundle mainBundle] resourcePath];
            return [bundlePath UTF8String];
        }
        
        std::string GetSyncedAppHomeDirectory() {
            NSArray* paths = NSSearchPathForDirectoriesInDomains(NSLibraryDirectory,
                                                                 NSUserDomainMask,
                                                                 YES);
            NSString* libraryDirectory = [paths firstObject];
            return [libraryDirectory UTF8String];
        }
    }
}
