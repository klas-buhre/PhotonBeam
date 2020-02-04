#ifndef PrismMesh_hpp
#define PrismMesh_hpp

#include "IMesh.hpp"

namespace Pht {
    struct PrismTextureCounts {
        Vec2 mZSides {1.0f, 1.0f};
        Vec2 mXSide {1.0f, 1.0f};
        Vec2 mYSide {1.0f, 1.0f};
        Vec2 mTiltedSide {1.0f, 1.0f};
    };
    
    class PrismMesh: public IMesh {
    public:
        PrismMesh(float size, float depth);
        PrismMesh(float size, float depth, const Pht::Vec3& offset);
        PrismMesh(float size, float depth, const PrismTextureCounts& textureCounts);
        
        Optional<std::string> GetName() const override;
        std::unique_ptr<VertexBuffer> CreateVertexBuffer(VertexFlags attributeFlags) const override;
        
    private:
        float mSize;
        float mDepth;
        Pht::Vec3 mOffset {0.0f, 0.0f, 0.0f};
        PrismTextureCounts mTextureCounts;
    };
}

#endif
