#include "ObjMesh.hpp"

#include "ObjMeshLoader.hpp"

using namespace Pht;

ObjMesh::ObjMesh(const std::string& filename, float scale, MoveMeshToOrigin moveToOrigin) :
    mFilename {filename},
    mScale {scale},
    mMoveToOrigin {moveToOrigin} {}

Optional<std::string> ObjMesh::GetName() const {
    return Optional<std::string> {mFilename + std::to_string(mScale)};
}

std::unique_ptr<VertexBuffer> ObjMesh::CreateVertexBuffer(VertexFlags attributeFlags) const {
    return ObjMeshLoader::Load(mFilename, attributeFlags, mScale, mMoveToOrigin);
}
