#include "SphereMesh.hpp"

using namespace Pht;

SphereMesh::SphereMesh(float radius, const Optional<std::string>& name) :
    SphereMesh {radius, Vec2{5.0f, 8.0f}, name} {}

SphereMesh::SphereMesh(float radius, const IVec2& divisions, const Optional<std::string>& name) :
    ParametricSurface {divisions, Vec2{Pi, TwoPi}, Vec2{1.0f, 1.0f}, name},
    mRadius {radius} {}

SphereMesh::SphereMesh(float radius, const Vec2& textureCount, const Optional<std::string>& name) :
    SphereMesh {radius, textureCount, Vec3{0.0f, 0.0f, 0.0f}, name} {}

SphereMesh::SphereMesh(float radius,
                       const Vec2& textureCount,
                       const Vec3& translation,
                       const Optional<std::string>& name) :
    ParametricSurface {IVec2{20, 20}, Vec2{Pi, TwoPi}, textureCount, name},
    mRadius {radius},
    mTranslation {translation} {}

Vec3 SphereMesh::Evaluate(const Vec2& domain) const {
    auto u = domain.x;
    auto v = domain.y;
    auto x = static_cast<float>(mRadius * sin(u) * cos(v)) + mTranslation.x;
    auto y = static_cast<float>(mRadius * cos(u)) + mTranslation.y;
    auto z = static_cast<float>(mRadius * -sin(u) * sin(v)) + mTranslation.z;
    return {x, y, z};
}
