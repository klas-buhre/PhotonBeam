#include "ParametricSurface.hpp"

using namespace Pht;

ParametricSurface::ParametricSurface(const IVec2& divisions,
                                     const Vec2& upperBound,
                                     const Vec2& textureCount,
                                     const Optional<std::string>& mName) :
    mName {mName},
    mDivisions {divisions},
    mUpperBound {upperBound},
    mSlices {mDivisions - IVec2{1, 1}},
    mTextureCount {textureCount} {}

Vec2 ParametricSurface::ComputeDomain(float x, float y) const {
    return {x * mUpperBound.x / mSlices.x, y * mUpperBound.y / mSlices.y};
}

Optional<std::string> ParametricSurface::GetName() const {
    return mName;
}

std::unique_ptr<VertexBuffer>
ParametricSurface::CreateVertexBuffer(VertexFlags attributeFlags) const {
    auto vertexCount = mDivisions.x * mDivisions.y;
    auto triangleIndexCount = 6 * mSlices.x * mSlices.y;
    auto vertexBuffer = std::make_unique<VertexBuffer>(vertexCount,
                                                       triangleIndexCount,
                                                       attributeFlags);
    for (auto j = 0; j < mDivisions.y; j++) {
        for (auto i = 0; i < mDivisions.x; i++) {

            // Compute Position.
            Vec2 domain {ComputeDomain(i, j)};
            Vec3 vertex {Evaluate(domain)};

            auto k = static_cast<float>(i);
            auto l = static_cast<float>(j);

            // Nudge the point if the normal is indeterminate.
            if (i == 0) {
                k += 0.01f;
            }
            if (i == mDivisions.x - 1) {
                k -= 0.01f;
            }
            if (j == 0) {
                l += 0.01f;
            }
            if (j == mDivisions.y - 1) {
                l -= 0.01f;
            }
            
            // Compute the tangents.
            Vec3 p {Evaluate(ComputeDomain(k, l))};
            Vec3 u {Evaluate(ComputeDomain(k + 0.01f, l)) - p};
            Vec3 v {Evaluate(ComputeDomain(k, l + 0.01f)) - p};
            
            // Compute Normal.
            Vec3 normal {u.Cross(v).Normalized()};
            
            // Compute Texture coord.
            auto s = mTextureCount.x * i / mSlices.x;
            auto t = mTextureCount.y * j / mSlices.y;
            Vec2 textureCoord {s, t};
            vertexBuffer->Write(vertex, normal, textureCoord);
        }
    }
    
    GenerateTriangleIndices(*vertexBuffer);
    return vertexBuffer;
}

void ParametricSurface::GenerateTriangleIndices(VertexBuffer& vertexBuffer) const {
    for (auto j = 0, vertex = 0; j < mSlices.y; j++) {
        for (auto i = 0; i < mSlices.x; i++) {
            auto next = (i + 1) % mDivisions.x;
            vertexBuffer.AddIndex(vertex + i);
            vertexBuffer.AddIndex(vertex + next);
            vertexBuffer.AddIndex(vertex + i + mDivisions.x);
            vertexBuffer.AddIndex(vertex + next);
            vertexBuffer.AddIndex(vertex + next + mDivisions.x);
            vertexBuffer.AddIndex(vertex + i + mDivisions.x);
        }
        vertex += mDivisions.x;
    }
}
