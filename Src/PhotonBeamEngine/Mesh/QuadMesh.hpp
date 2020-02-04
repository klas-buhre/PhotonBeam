#ifndef QuadMesh_hpp
#define QuadMesh_hpp

#include "IMesh.hpp"

namespace Pht {
    class QuadMesh: public IMesh {
    public:
        struct Vertex {
            Vec3 mPosition;
            Vec4 mColor;
            Vec2 mTextureCoord {0.0f, 0.0f};
        };
        
        struct Vertices {
            Vertex mV0;
            Vertex mV1;
            Vertex mV2;
            Vertex mV3;
        };

        QuadMesh(float xSize, float ySize, const Optional<std::string>& name = {});
        QuadMesh(float xSize, float ySize, float tilt, const Optional<std::string>& name = {});
        QuadMesh(float xSize,
                 float ySize,
                 const Vec2& textureCount,
                 float tilt,
                 const Optional<std::string>& name = {});
        QuadMesh(const Vertices& vertices, const Optional<std::string>& name = {});
        
        Optional<std::string> GetName() const override;
        std::unique_ptr<VertexBuffer> CreateVertexBuffer(VertexFlags attributeFlags) const override;
        
    private:
        Optional<std::string> mName;
        float mXSize {0.0f};
        float mYSize {0.0f};
        float mTilt {0.0f};
        Vertices mVertices;
        Vec2 mTextureCount;
    };
}

#endif
