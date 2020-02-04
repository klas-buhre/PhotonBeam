#include "QuadMesh.hpp"

#include <assert.h>

#include "MeshUtils.hpp"

using namespace Pht;

QuadMesh::QuadMesh(float xSize, float ySize, const Optional<std::string>& name) :
    QuadMesh {xSize, ySize, Vec2{1.0f, 1.0f}, 0.0f, name} {}

QuadMesh::QuadMesh(float xSize, float ySize, float tilt, const Optional<std::string>& name) :
    QuadMesh {xSize, ySize, Vec2{1.0f, 1.0f}, tilt, name} {}

QuadMesh::QuadMesh(float xSize,
                   float ySize,
                   const Vec2& textureCount,
                   float tilt,
                   const Optional<std::string>& name) :
    mName {name},
    mXSize {xSize},
    mYSize {ySize},
    mTilt {tilt},
    mTextureCount {textureCount} {}

QuadMesh::QuadMesh(const Vertices& vertices, const Optional<std::string>& name) :
    mName {name},
    mVertices {vertices} {}

Optional<std::string> QuadMesh::GetName() const {
    return {};
}

std::unique_ptr<VertexBuffer> QuadMesh::CreateVertexBuffer(VertexFlags attributeFlags) const {
    auto vertexBuffer = std::make_unique<VertexBuffer>(4, 6, attributeFlags);
    
    if (mXSize || mYSize) {
        auto halfX = mXSize / 2.0f;
        auto halfY = mYSize / 2.0f;
        
        MeshUtils::QuadVertices vertices {
            {-halfX, -halfY, 0.0f},
            {halfX, -halfY, 0.0f},
            {halfX + mTilt, halfY, 0.0f},
            {-halfX + mTilt, halfY, 0.0f}
        };

        auto textureCoords = MeshUtils::CreateQuadTextureCoords(mTextureCount);
        MeshUtils::CreateQuad(*vertexBuffer, vertices, textureCoords);
    } else {
        auto u = mVertices.mV1.mPosition - mVertices.mV0.mPosition;
        auto v = mVertices.mV3.mPosition - mVertices.mV0.mPosition;
        auto normal = u.Cross(v).Normalized();

        vertexBuffer->BeginSurface();

        vertexBuffer->Write(mVertices.mV0.mPosition,
                            normal,
                            mVertices.mV0.mTextureCoord,
                            mVertices.mV0.mColor);
        
        vertexBuffer->Write(mVertices.mV1.mPosition,
                            normal,
                            mVertices.mV1.mTextureCoord,
                            mVertices.mV1.mColor);
        
        vertexBuffer->Write(mVertices.mV2.mPosition,
                            normal,
                            mVertices.mV2.mTextureCoord,
                            mVertices.mV2.mColor);
        
        vertexBuffer->Write(mVertices.mV3.mPosition,
                            normal,
                            mVertices.mV3.mTextureCoord,
                            mVertices.mV3.mColor);

        vertexBuffer->AddIndex(0);
        vertexBuffer->AddIndex(1);
        vertexBuffer->AddIndex(2);
        vertexBuffer->AddIndex(2);
        vertexBuffer->AddIndex(3);
        vertexBuffer->AddIndex(0);
    }
    
    return vertexBuffer;
}
