#ifndef ParametricSurface_hpp
#define ParametricSurface_hpp

#include "IMesh.hpp"
#include "Vector.hpp"

namespace Pht {
    class ParametricSurface: public IMesh {
    public:
        Optional<std::string> GetName() const override;
        std::unique_ptr<VertexBuffer> CreateVertexBuffer(VertexFlags attributeFlags) const override;
        
        void SetUpperBound(const Vec2& upperBound) {
            mUpperBound = upperBound;
        }
        
    protected:
        ParametricSurface(const IVec2& divisions,
                          const Vec2& upperBound,
                          const Vec2& textureCount,
                          const Optional<std::string>& name = {});
        
        virtual Vec3 Evaluate(const Vec2& domain) const = 0;
        
    private:
        Vec2 ComputeDomain(float i, float j) const;
        void GenerateTriangleIndices(VertexBuffer& vertexBuffer) const;
        
        Optional<std::string> mName;
        IVec2 mDivisions;
        Vec2 mUpperBound;
        IVec2 mSlices;
        Vec2 mTextureCount;
    };
}

#endif
