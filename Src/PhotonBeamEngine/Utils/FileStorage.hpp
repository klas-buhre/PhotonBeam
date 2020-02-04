#ifndef FileStorage_hpp
#define FileStorage_hpp

#include <string>

namespace Pht {
    namespace FileStorage {
        bool Load(const std::string& filename, std::string& data);
        bool Save(const std::string& filename, const std::string& data);
        bool LoadCleartextFile(const std::string& fullPathFilename, std::string& data);
    }
}

#endif
