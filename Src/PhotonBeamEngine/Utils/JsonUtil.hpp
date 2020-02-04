#ifndef JsonUtil_hpp
#define JsonUtil_hpp

#include <string>

// RapidJson include.
#include "document.h"

#include "Vector.hpp"

namespace Pht {
    namespace Json {
        void ParseFile(rapidjson::Document& document, const std::string& filename);
        void ParseDocument(rapidjson::Document& document, const std::string& jsonString);
        void EncodeDocument(rapidjson::Document& document, std::string& jsonString);
        std::string ReadString(const rapidjson::Value& object, const std::string& name);
        bool ReadBool(const rapidjson::Value& object, const std::string& name);
        int ReadInt(const rapidjson::Value& object, const std::string& name);
        uint64_t ReadUInt64(const rapidjson::Value& object, const std::string& name);
        float ReadFloat(const rapidjson::Value& object, const std::string& name);
        IVec2 ReadIVec2(const rapidjson::Value& object, const std::string& name);
        void AddString(rapidjson::Value& object,
                       const std::string& name,
                       const std::string& value,
                       rapidjson::Document::AllocatorType& allocator);
        void AddBool(rapidjson::Value& object,
                     const std::string& name,
                     bool value,
                     rapidjson::Document::AllocatorType& allocator);
        void AddInt(rapidjson::Value& object,
                    const std::string& name,
                    int value,
                    rapidjson::Document::AllocatorType& allocator);
        void AddUInt64(rapidjson::Value& object,
                       const std::string& name,
                       uint64_t value,
                       rapidjson::Document::AllocatorType& allocator);
        void AddValue(rapidjson::Value& object,
                      const std::string& name,
                      rapidjson::Value& value,
                      rapidjson::Document::AllocatorType& allocator);
    }
}

#endif
