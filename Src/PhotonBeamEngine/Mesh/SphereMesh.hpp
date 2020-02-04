#ifndef SphereMesh_hpp
#define SphereMesh_hpp

#include "ParametricSurface.hpp"

namespace Pht {
    class SphereMesh: public ParametricSurface {
    public:
        explicit SphereMesh(float radius, const Optional<std::string>& name = {});
        SphereMesh(float radius, const IVec2& divisions, const Optional<std::string>& name = {});
        SphereMesh(float radius, const Vec2& textureCount, const Optional<std::string>& name = {});
        SphereMesh(float radius,
                   const Vec2& textureCount,
                   const Vec3& translation,
                   const Optional<std::string>& name = {});
        
        Vec3 Evaluate(const Vec2& domain) const override;
        
    private:
        float mRadius;
        Vec3 mTranslation {0.0f, 0.0f, 0.0f};
    };
}

#endif

