#include "PrismMesh.hpp"

#include "MeshUtils.hpp"

using namespace Pht;

PrismMesh::PrismMesh(float size, float depth) :
    PrismMesh {size, depth, PrismTextureCounts{}} {}

PrismMesh::PrismMesh(float size, float depth, const Pht::Vec3& offset) :
    mSize {size},
    mDepth {depth},
    mOffset {offset} {}

PrismMesh::PrismMesh(float size, float depth, const PrismTextureCounts& textureCounts) :
    mSize {size},
    mDepth {depth},
    mTextureCounts {textureCounts} {}

Optional<std::string> PrismMesh::GetName() const {
    return {};
}

std::unique_ptr<VertexBuffer> PrismMesh::CreateVertexBuffer(VertexFlags attributeFlags) const {
    auto vertexBuffer = std::make_unique<VertexBuffer>(18, 24, attributeFlags);
    auto halfSize = mSize / 2.0f;
    auto halfDepth = mDepth / 2.0f;
    
    MeshUtils::TriangleTextureCoords triangleTextureCoords {
        {0.0f, mTextureCounts.mZSides.y},
        {mTextureCounts.mZSides.x, mTextureCounts.mZSides.y},
        {mTextureCounts.mZSides.x, 0.0f}
    };
    
    MeshUtils::TriangleVertices triFrontVertices {
        {-halfSize + mOffset.x, -halfSize + mOffset.y, halfDepth + mOffset.z},
        {halfSize + mOffset.x, -halfSize + mOffset.y, halfDepth + mOffset.z},
        {halfSize + mOffset.x, halfSize + mOffset.y, halfDepth + mOffset.z}
    };
    MeshUtils::CreateTriangle(*vertexBuffer, triFrontVertices, triangleTextureCoords);

    if (mDepth > 0.0f) {
        MeshUtils::TriangleVertices triBackVertices {
            {halfSize + mOffset.x, -halfSize + mOffset.y, -halfDepth + mOffset.z},
            {-halfSize + mOffset.x, -halfSize + mOffset.y, -halfDepth + mOffset.z},
            {halfSize + mOffset.x, halfSize + mOffset.y, -halfDepth + mOffset.z}
        };
        MeshUtils::CreateTriangle(*vertexBuffer, triBackVertices, triangleTextureCoords);

        MeshUtils::QuadVertices ySideVertices {
            {halfSize + mOffset.x, -halfSize + mOffset.y, -halfDepth + mOffset.z},
            {halfSize + mOffset.x, -halfSize + mOffset.y, halfDepth + mOffset.z},
            {-halfSize + mOffset.x, -halfSize + mOffset.y, halfDepth + mOffset.z},
            {-halfSize + mOffset.x, -halfSize + mOffset.y, -halfDepth + mOffset.z}
        };
        auto ySideTextureCoords = MeshUtils::CreateQuadTextureCoords(mTextureCounts.mYSide);
        MeshUtils::CreateQuad(*vertexBuffer, ySideVertices, ySideTextureCoords);
        
        MeshUtils::QuadVertices xSideVertices {
            {halfSize + mOffset.x, halfSize + mOffset.y, -halfDepth + mOffset.z},
            {halfSize + mOffset.x, halfSize + mOffset.y, halfDepth + mOffset.z},
            {halfSize + mOffset.x, -halfSize + mOffset.y, halfDepth + mOffset.z},
            {halfSize + mOffset.x, -halfSize + mOffset.y, -halfDepth + mOffset.z}
        };
        auto xSideTextureCoords = MeshUtils::CreateQuadTextureCoords(mTextureCounts.mXSide);
        MeshUtils::CreateQuad(*vertexBuffer, xSideVertices, xSideTextureCoords);
        
        MeshUtils::QuadVertices tiltedSideVertices {
            {-halfSize + mOffset.x, -halfSize + mOffset.y, -halfDepth + mOffset.z},
            {-halfSize + mOffset.x, -halfSize + mOffset.y, halfDepth + mOffset.z},
            {halfSize + mOffset.x, halfSize + mOffset.y, halfDepth + mOffset.z},
            {halfSize + mOffset.x, halfSize + mOffset.y, -halfDepth + mOffset.z}
        };
        auto tiltedSideTextureCoords = MeshUtils::CreateQuadTextureCoords(mTextureCounts.mTiltedSide);
        MeshUtils::CreateQuad(*vertexBuffer, tiltedSideVertices, tiltedSideTextureCoords);
    }
    
    return vertexBuffer;
}
