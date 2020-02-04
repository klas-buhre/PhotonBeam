#include "FileStorage.hpp"

#include <sstream>
#include <fstream>
#include <array>
#include <assert.h>

#include "FileSystem.hpp"

using namespace Pht;

namespace {
    const std::array<unsigned char, 4> key = {0xfe, 0x59, 0xa0, 0x4b};
    
    void Xor(const std::string& input, std::string& output) {
        output.resize(input.size());
        
        for (auto i = 0; i < input.size(); ++i) {
            output[i] = input[i] ^ key[i % key.size()];
        }
    }
}

bool FileStorage::Load(const std::string& filename, std::string& data) {
    auto fullPath = FileSystem::GetSyncedAppHomeDirectory() + "/" + filename;
    std::ifstream file {fullPath};
    if (!file.is_open()) {
        return false;
    }
    
    std::stringstream stream;
    stream << file.rdbuf();
    Xor(stream.str(), data);
    
    return true;
}

bool FileStorage::Save(const std::string& filename, const std::string& data) {
    auto fullPath = FileSystem::GetSyncedAppHomeDirectory() + "/" + filename;
    std::ofstream file {fullPath};
    if (!file.is_open()) {
        return false;
    }
    
    std::string xoredData;
    Xor(data, xoredData);
    file << xoredData;
    
    return true;
}

bool FileStorage::LoadCleartextFile(const std::string& fullPathFilename, std::string& data) {
    std::ifstream file {fullPathFilename};
    if (!file.is_open()) {
        return false;
    }
    
    std::stringstream stream;
    stream << file.rdbuf();
    data = stream.str();

    return true;
}
