#ifndef ObjMesh_hpp
#define ObjMesh_hpp

#include "IMesh.hpp"
#include "ObjMeshLoader.hpp"

namespace Pht {
    class ObjMesh: public IMesh {
    public:
        ObjMesh(const std::string& filename,
                float scale = 1.0f,
                MoveMeshToOrigin moveToOrigin = MoveMeshToOrigin::No);
        
        Optional<std::string> GetName() const override;
        std::unique_ptr<VertexBuffer> CreateVertexBuffer(VertexFlags attributeFlags) const override;
        
    private:
        std::string mFilename;
        float mScale {1.0f};
        MoveMeshToOrigin mMoveToOrigin {MoveMeshToOrigin::No};
    };
}

#endif
