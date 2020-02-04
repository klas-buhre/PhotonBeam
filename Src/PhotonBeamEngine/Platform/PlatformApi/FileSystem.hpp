#ifndef FileSystem_hpp
#define FileSystem_hpp

#include <string>

namespace Pht {
    namespace FileSystem {
        std::string GetResourceDirectory();
        std::string GetSyncedAppHomeDirectory();
    }
}

#endif
