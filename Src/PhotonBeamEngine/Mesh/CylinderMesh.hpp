#ifndef CylinderMesh_hpp
#define CylinderMesh_hpp

#include "ParametricSurface.hpp"

namespace Pht {
    struct CylinderTextureCounts {
        Vec2 mBody;
        Vec2 mDiscs;
    };
    
    class CylinderMesh: public IMesh {
    public:
        CylinderMesh(float radius,
                     float height,
                     bool createDiscs,
                     const Optional<std::string>& name = {});
        CylinderMesh(float radius,
                     float height,
                     bool createDiscs,
                     const CylinderTextureCounts& textureCounts,
                     const Optional<std::string>& name = {});
        
        Optional<std::string> GetName() const override;
        std::unique_ptr<VertexBuffer> CreateVertexBuffer(VertexFlags attributeFlags) const override;
        
    private:
        enum class DiscType {
            Upper,
            Lower
        };
        
        void GenerateBody(VertexBuffer& vertexBuffer) const;
        void GenerateDisc(VertexBuffer& vertexBuffer, DiscType discType) const;
        
        Optional<std::string> mName;
        float mRadius;
        float mHeight;
        bool mCreateDiscs {true};
        CylinderTextureCounts mTextureCounts;
    };
}

#endif
