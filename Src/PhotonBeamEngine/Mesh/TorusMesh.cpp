#include "TorusMesh.hpp"

using namespace Pht;

TorusMesh::TorusMesh(float majorRadius, float minorRadius) :
    TorusMesh {majorRadius, minorRadius, Vec2{10, 3}} {}

TorusMesh::TorusMesh(float majorRadius, float minorRadius, const Vec2& textureCount) :
    ParametricSurface {IVec2{20, 20}, Vec2{TwoPi, TwoPi}, textureCount},
    mMajorRadius {majorRadius},
    mMinorRadius {minorRadius} {}

Vec3 TorusMesh::Evaluate(const Vec2& domain) const {
    auto u = domain.x;
    auto v = domain.y;
    auto x = static_cast<float>((mMajorRadius + mMinorRadius * cos(v)) * cos(u));
    auto y = static_cast<float>((mMajorRadius + mMinorRadius * cos(v)) * sin(u));
    auto z = static_cast<float>(mMinorRadius * sin(v));
    return {x, y, z};
}

