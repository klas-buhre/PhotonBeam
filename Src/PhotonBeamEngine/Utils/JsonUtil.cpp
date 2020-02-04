#include "JsonUtil.hpp"

#include <sstream>
#include <fstream>
#include <assert.h>

// RapidJson includes.
#include "stringbuffer.h"
#include "writer.h"

#include "FileSystem.hpp"

using namespace Pht;

void Json::ParseFile(rapidjson::Document& document, const std::string& filename) {
    auto fullPath = FileSystem::GetResourceDirectory() + "/" + filename;
    std::ifstream file {fullPath};
    assert(file.is_open());
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    
    document.Parse(buffer.str().c_str());
}

void Json::ParseDocument(rapidjson::Document& document, const std::string& jsonString) {
    document.Parse(jsonString.c_str());
}

void Json::EncodeDocument(rapidjson::Document& document, std::string& jsonString) {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer {buffer};
    document.Accept(writer);
    jsonString = buffer.GetString();
}

std::string Json::ReadString(const rapidjson::Value& object, const std::string& name) {
    assert(object.HasMember(name.c_str()));
    
    const auto& value = object[name.c_str()];
    assert(value.IsString());
    
    return value.GetString();
}

bool Json::ReadBool(const rapidjson::Value& object, const std::string& name) {
    assert(object.HasMember(name.c_str()));
    
    const auto& value = object[name.c_str()];
    assert(value.IsBool());
    
    return value.GetBool();
}

int Json::ReadInt(const rapidjson::Value& object, const std::string& name) {
    assert(object.HasMember(name.c_str()));
    
    const auto& value = object[name.c_str()];
    assert(value.IsInt());
    
    return value.GetInt();
}

uint64_t Json::ReadUInt64(const rapidjson::Value& object, const std::string& name) {
    assert(object.HasMember(name.c_str()));
    
    const auto& value = object[name.c_str()];
    assert(value.IsUint64());
    
    return value.GetUint64();
}

float Json::ReadFloat(const rapidjson::Value& object, const std::string& name) {
    assert(object.HasMember(name.c_str()));
    
    const auto& value = object[name.c_str()];
    assert(value.IsFloat());
    
    return value.GetFloat();
}

IVec2 Json::ReadIVec2(const rapidjson::Value& object, const std::string& name) {
    assert(object.HasMember(name.c_str()));

    const auto& value = object[name.c_str()];
    assert(value.IsArray());
    
    const auto& array = value.GetArray();
    assert(array.Size() == 2);
    assert(array[0].IsInt());
    assert(array[1].IsInt());
    
    return {array[0].GetInt(), array[1].GetInt()};
}

void Json::AddString(rapidjson::Value& object,
                     const std::string& name,
                     const std::string& value,
                     rapidjson::Document::AllocatorType& allocator) {
    rapidjson::Value v;
    v.SetString(value.c_str(), static_cast<unsigned int>(value.size()), allocator);

    rapidjson::Value n;
    n.SetString(name.c_str(), static_cast<unsigned int>(name.size()), allocator);
    
    object.AddMember(n, v, allocator);
}

void Json::AddBool(rapidjson::Value& object,
                   const std::string& name,
                   bool value,
                   rapidjson::Document::AllocatorType& allocator) {
    rapidjson::Value v;
    v.SetBool(value);

    rapidjson::Value n;
    n.SetString(name.c_str(), static_cast<unsigned int>(name.size()), allocator);
    
    object.AddMember(n, v, allocator);
}

void Json::AddInt(rapidjson::Value& object,
                  const std::string& name,
                  int value,
                  rapidjson::Document::AllocatorType& allocator) {
    rapidjson::Value v;
    v.SetInt(value);

    rapidjson::Value n;
    n.SetString(name.c_str(), static_cast<unsigned int>(name.size()), allocator);
    
    object.AddMember(n, v, allocator);
}

void Json::AddUInt64(rapidjson::Value& object,
                     const std::string& name,
                     uint64_t value,
                     rapidjson::Document::AllocatorType& allocator) {
    rapidjson::Value v;
    v.SetUint64(value);

    rapidjson::Value n;
    n.SetString(name.c_str(), static_cast<unsigned int>(name.size()), allocator);
    
    object.AddMember(n, v, allocator);
}

void Json::AddValue(rapidjson::Value& object,
                    const std::string& name,
                    rapidjson::Value& value,
                    rapidjson::Document::AllocatorType& allocator) {
    rapidjson::Value n;
    n.SetString(name.c_str(), static_cast<unsigned int>(name.size()), allocator);

    object.AddMember(n, value, allocator);
}
