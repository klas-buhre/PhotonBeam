#ifndef MeshUtils_hpp
#define MeshUtils_hpp

#include "VertexBuffer.hpp"
#include "Vector.hpp"

namespace Pht {
    namespace MeshUtils {
        struct TriangleVertices {
            Vec3 mV0;
            Vec3 mV1; 
            Vec3 mV2;
        };
        
        struct TriangleTextureCoords {
            Vec2 mV0;
            Vec2 mV1;
            Vec2 mV2;
        };

        struct QuadVertices {
            Vec3 mV0;
            Vec3 mV1; 
            Vec3 mV2;
            Vec3 mV3;
        };

        struct QuadTextureCoords {
            Vec2 mV0;
            Vec2 mV1;
            Vec2 mV2;
            Vec2 mV3;
        };
        
        void CreateTriangle(VertexBuffer& vertexBuffer,
                            const TriangleVertices& vertices,
                            const TriangleTextureCoords& textureCoords);
        void CreateQuad(VertexBuffer& vertexBuffer,
                        const QuadVertices& vertices,
                        const QuadTextureCoords& textureCoords);
        QuadTextureCoords CreateQuadTextureCoords(const Vec2& textureCounts);
    }
}

#endif
