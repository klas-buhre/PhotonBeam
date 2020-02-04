#include "BoxMesh.hpp"

#include "MeshUtils.hpp"

using namespace Pht;

BoxMesh::BoxMesh(float xSize, float ySize, float zSize, const Optional<std::string>& name) :
    BoxMesh {
        xSize,
        ySize,
        zSize,
        BoxTextureCounts{{1.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 1.0f}},
        name
    } {}

BoxMesh::BoxMesh(float xSize,
                 float ySize,
                 float zSize,
                 const BoxTextureCounts& textureCounts,
                 const Optional<std::string>& name) :
    mName {name},
    mXSize {xSize},
    mYSize {ySize},
    mZSize {zSize},
    mTextureCounts {textureCounts} {}

Optional<std::string> BoxMesh::GetName() const {
    return {};
}

std::unique_ptr<VertexBuffer> BoxMesh::CreateVertexBuffer(VertexFlags attributeFlags) const {
    auto vertexBuffer = std::make_unique<VertexBuffer>(24, 36, attributeFlags);
    
    auto halfX = mXSize / 2.0f;
    auto halfY = mYSize / 2.0f;
    auto halfZ = mZSize / 2.0f;
    
    MeshUtils::QuadVertices frontVertices {
        {-halfX, -halfY, halfZ},
        {halfX, -halfY, halfZ},
        {halfX, halfY, halfZ},
        {-halfX, halfY, halfZ}
    };
    auto frontTextureCoords = MeshUtils::CreateQuadTextureCoords(mTextureCounts.mZSides);
    MeshUtils::CreateQuad(*vertexBuffer, frontVertices, frontTextureCoords);
    
    MeshUtils::QuadVertices backVertices {
        {halfX, -halfY, -halfZ},
        {-halfX, -halfY, -halfZ},
        {-halfX, halfY, -halfZ},
        {halfX, halfY, -halfZ}
    };
    auto backTextureCoords = MeshUtils::CreateQuadTextureCoords(mTextureCounts.mZSides);
    MeshUtils::CreateQuad(*vertexBuffer, backVertices, backTextureCoords);
    
    MeshUtils::QuadVertices posXSideVertices {
        {halfX, -halfY, halfZ},
        {halfX, -halfY, -halfZ},
        {halfX, halfY, -halfZ},
        {halfX, halfY, halfZ}
    };
    auto xSideTextureCoords = MeshUtils::CreateQuadTextureCoords(mTextureCounts.mXSides);
    MeshUtils::CreateQuad(*vertexBuffer, posXSideVertices, xSideTextureCoords);
    
    MeshUtils::QuadVertices posYSideVertices {
        {-halfX, halfY, halfZ},
        {halfX, halfY, halfZ},
        {halfX, halfY, -halfZ},
        {-halfX, halfY, -halfZ}
    };
    auto ySideTextureCoords = MeshUtils::CreateQuadTextureCoords(mTextureCounts.mYSides);
    MeshUtils::CreateQuad(*vertexBuffer, posYSideVertices, ySideTextureCoords);
  
    MeshUtils::QuadVertices negXSideVertices {
        {-halfX, -halfY, -halfZ},
        {-halfX, -halfY, halfZ},
        {-halfX, halfY, halfZ},
        {-halfX, halfY, -halfZ}
    };
    MeshUtils::CreateQuad(*vertexBuffer, negXSideVertices, xSideTextureCoords);
    
    MeshUtils::QuadVertices negYSideVertices {
        {halfX, -halfY, -halfZ},
        {halfX, -halfY, halfZ},
        {-halfX, -halfY, halfZ},
        {-halfX, -halfY, -halfZ}
    };
    MeshUtils::CreateQuad(*vertexBuffer, negYSideVertices, ySideTextureCoords);
    
    return vertexBuffer;
}
