#ifndef MathUtils_hpp
#define MathUtils_hpp

#include "Vector.hpp"
#include "StaticVector.hpp"

namespace Pht {
    float NormalizedRand();
    int Sign(float value);
    float ToDegrees(float rad);
    float ToRadians(float degrees);
    
    template<int N>
    float Lerp(float x, const StaticVector<Vec2, N>& points) {
        auto& back = points.Back();
        if (x == back.x) {
            return back.y;
        }
        
        auto& front = points.Front();
        if (x == front.x) {
            return front.y;
        }
        
        Vec2 leftPoint;
        Vec2 rightPoint;
        auto numPoints = points.Size();
        auto* previousPoint = &points.Front();
        
        for (auto i {1}; i < numPoints; ++i) {
            auto& point = points.At(i);
            if (x >= previousPoint->x && x <= point.x) {
                leftPoint = *previousPoint;
                rightPoint = point;
                break;
            }
            
            previousPoint = &point;
        }
        
        auto diff = rightPoint - leftPoint;
        return diff.y * (x - leftPoint.x) / (rightPoint.x - leftPoint.x) + leftPoint.y;
    }
}

#endif
