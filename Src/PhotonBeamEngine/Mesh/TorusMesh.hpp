#ifndef TorusMesh_hpp
#define TorusMesh_hpp

#include "ParametricSurface.hpp"

namespace Pht {
    class TorusMesh: public ParametricSurface {
    public:
        TorusMesh(float majorRadius, float minorRadius);
        TorusMesh(float majorRadius, float minorRadius, const Vec2& textureCount);
        
        Vec3 Evaluate(const Vec2& domain) const override;
        
    private:
        float mMajorRadius; 
        float mMinorRadius;
    };
}

#endif
