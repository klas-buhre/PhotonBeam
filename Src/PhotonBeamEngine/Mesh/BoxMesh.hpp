#ifndef BoxMesh_hpp
#define BoxMesh_hpp

#include "IMesh.hpp"

namespace Pht {
    struct BoxTextureCounts {
        Vec2 mZSides;
        Vec2 mXSides;
        Vec2 mYSides;
    };
    
    class BoxMesh: public IMesh {
    public:
        BoxMesh(float xSize, float ySize, float zSize, const Optional<std::string>& name = {});
        BoxMesh(float xSize,
                float ySize,
                float zSize,
                const BoxTextureCounts& textureCounts,
                const Optional<std::string>& name = {});
        
        Optional<std::string> GetName() const override;
        std::unique_ptr<VertexBuffer> CreateVertexBuffer(VertexFlags attributeFlags) const override;
        
    private:
        Optional<std::string> mName;
        float mXSize;
        float mYSize;
        float mZSize;
        BoxTextureCounts mTextureCounts;
    };
}

#endif
