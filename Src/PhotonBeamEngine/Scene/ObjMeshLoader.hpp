#ifndef ObjMeshLoader_hpp
#define ObjMeshLoader_hpp

#include <memory>

#include "VertexBuffer.hpp"

namespace Pht {
    enum class MoveMeshToOrigin {
        Yes,
        No
    };
    
    namespace ObjMeshLoader {
        std::unique_ptr<VertexBuffer> Load(const std::string& filename,
                                           VertexFlags attributeFlags,
                                           float scale,
                                           MoveMeshToOrigin moveMeshToOrigin);
    }
}

#endif
