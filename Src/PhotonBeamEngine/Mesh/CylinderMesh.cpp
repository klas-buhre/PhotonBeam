#include "CylinderMesh.hpp"

using namespace Pht;

namespace {
    const auto slices = 20;
    const auto dTheta = TwoPi / slices;
    const auto bodyVertexCount = slices * 2;
    const auto bodyIndexCount = slices * 6;
    const auto discVertexCount = slices + 1;
    const auto discIndexCount = slices * 3;
}

CylinderMesh::CylinderMesh(float radius,
                           float height,
                           bool createDiscs,
                           const Optional<std::string>& name) :
    CylinderMesh {
        radius,
        height,
        createDiscs,
        CylinderTextureCounts{{1.0f, 1.0f}, {1.0f, 1.0f}},
        name
    } {}

CylinderMesh::CylinderMesh(float radius,
                           float height,
                           bool createDiscs,
                           const CylinderTextureCounts& textureCounts,
                           const Optional<std::string>& name) :
    mName {name},
    mRadius {radius},
    mHeight {height},
    mCreateDiscs {createDiscs},
    mTextureCounts {textureCounts} {}

Optional<std::string> CylinderMesh::GetName() const {
    return mName;
}

std::unique_ptr<VertexBuffer> CylinderMesh::CreateVertexBuffer(VertexFlags attributeFlags) const {
    auto numVertices = mCreateDiscs ? discVertexCount * 2 + bodyVertexCount : bodyVertexCount;
    auto numIndices = mCreateDiscs ? discIndexCount * 2 + bodyIndexCount : bodyIndexCount;
    auto vertexBuffer = std::make_unique<VertexBuffer>(numVertices, numIndices, attributeFlags);
    
    GenerateBody(*vertexBuffer);
    
    if (mCreateDiscs) {
        GenerateDisc(*vertexBuffer, DiscType::Upper);
        GenerateDisc(*vertexBuffer, DiscType::Lower);
    }
    
    return vertexBuffer;
}

void CylinderMesh::GenerateBody(VertexBuffer& vertexBuffer) const {
    auto theta = 0.0f;
    for (auto slice = 0; slice < slices; slice++) {
        auto x = static_cast<float>(mRadius * cos(theta));
        auto z = static_cast<float>(mRadius * sin(theta));

        Vec3 lowerRimVertex {x, -mHeight / 2.0f, z};
        Vec3 upperRimVertex {x, mHeight / 2.0f, z};
        Vec3 normal {x, 0, z};
        normal.Normalize();
        auto s = (TwoPi - theta) * mTextureCounts.mBody.x / TwoPi;
        Vec2 upperTextureCoord {s, 0.0f};
        Vec2 lowerTextureCoord {s, mTextureCounts.mBody.y};
        vertexBuffer.Write(lowerRimVertex, normal, lowerTextureCoord);
        vertexBuffer.Write(upperRimVertex, normal, upperTextureCoord);
        
        auto quadBegin = slice * 2;
        vertexBuffer.AddIndex((quadBegin + 3) % bodyVertexCount);
        vertexBuffer.AddIndex((quadBegin + 2) % bodyVertexCount);
        vertexBuffer.AddIndex(quadBegin);
        vertexBuffer.AddIndex(quadBegin + 1);
        vertexBuffer.AddIndex((quadBegin + 3) % bodyVertexCount);
        vertexBuffer.AddIndex(quadBegin);

        theta += dTheta;
    }
}

void CylinderMesh::GenerateDisc(VertexBuffer& vertexBuffer, DiscType discType) const {
    auto y = 0.0f;
    Vec3 normal;
    
    switch (discType) {
        case DiscType::Upper:
            y = mHeight / 2.0f;
            normal = Vec3{0.0f, 1.0f, 0.0f};
            break;
        case DiscType::Lower:
            y = -mHeight / 2.0f;
            normal = Vec3{0.0f, -1.0f, 0.0f};
            break;
    }

    auto centerVertexIndex = vertexBuffer.GetNumVertices();
    Vec3 centerVertex {0.0f, y, 0.0f};
    Vec2 centerTextureCoord {mTextureCounts.mDiscs.x / 2.0f, mTextureCounts.mDiscs.y / 2.0f};
    vertexBuffer.Write(centerVertex, normal, centerTextureCoord);
    auto rimBegin = vertexBuffer.GetNumVertices();
    
    auto theta = 0.0f;
    for (auto slice = 0; slice < slices; slice++) {
        auto x = static_cast<float>(mRadius * cos(theta));
        auto z = static_cast<float>(mRadius * sin(theta));
        Vec3 rimVertex {x, y, z};
        Vec2 textureCoord {
            centerTextureCoord.x * cosf(theta) + centerTextureCoord.x,
            centerTextureCoord.y * sinf(theta) + centerTextureCoord.y
        };
        vertexBuffer.Write(rimVertex, normal, textureCoord);
        
        switch (discType) {
            case DiscType::Lower:
                vertexBuffer.AddIndex(slice + rimBegin);
                vertexBuffer.AddIndex((slice + 1) % slices + rimBegin);
                vertexBuffer.AddIndex(centerVertexIndex);
                break;
            case DiscType::Upper:
                vertexBuffer.AddIndex(centerVertexIndex);
                vertexBuffer.AddIndex((slice + 1) % slices + rimBegin);
                vertexBuffer.AddIndex(slice + rimBegin);
                break;
        }
        
        theta += dTheta;
    }
}
