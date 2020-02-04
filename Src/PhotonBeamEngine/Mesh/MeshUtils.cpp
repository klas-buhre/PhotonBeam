#include "MeshUtils.hpp"

#include <assert.h>

using namespace Pht;

void MeshUtils::CreateTriangle(VertexBuffer& vertexBuffer,
                                const TriangleVertices& vertices,
                                const TriangleTextureCoords& textureCoords) {
    Vec3 u {vertices.mV1 - vertices.mV0};
    Vec3 v {vertices.mV2 - vertices.mV0};
    Vec3 normal {u.Cross(v).Normalized()};

    vertexBuffer.BeginSurface();
    
    vertexBuffer.Write(vertices.mV0, normal, textureCoords.mV0);
    vertexBuffer.Write(vertices.mV1, normal, textureCoords.mV1);
    vertexBuffer.Write(vertices.mV2, normal, textureCoords.mV2);

    vertexBuffer.AddIndex(0);
    vertexBuffer.AddIndex(1);
    vertexBuffer.AddIndex(2);
}

void MeshUtils::CreateQuad(VertexBuffer& vertexBuffer,
                            const QuadVertices& vertices,
                            const QuadTextureCoords& textureCoords) {
    Vec3 u {vertices.mV1 - vertices.mV0};
    Vec3 v {vertices.mV3 - vertices.mV0};
    Vec3 normal {u.Cross(v).Normalized()};
    
    vertexBuffer.BeginSurface();

    vertexBuffer.Write(vertices.mV0, normal, textureCoords.mV0);
    vertexBuffer.Write(vertices.mV1, normal, textureCoords.mV1);
    vertexBuffer.Write(vertices.mV2, normal, textureCoords.mV2);
    vertexBuffer.Write(vertices.mV3, normal, textureCoords.mV3);

    vertexBuffer.AddIndex(0);
    vertexBuffer.AddIndex(1);
    vertexBuffer.AddIndex(2);
    vertexBuffer.AddIndex(2);
    vertexBuffer.AddIndex(3);
    vertexBuffer.AddIndex(0);
}

MeshUtils::QuadTextureCoords MeshUtils::CreateQuadTextureCoords(const Vec2& textureCounts) {
    return {
        {0.0f, textureCounts.y},
        {textureCounts.x, textureCounts.y},
        {textureCounts.x, 0.0f},
        {0.0f, 0.0f}
    };
}
