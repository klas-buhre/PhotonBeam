#include "ObjMeshLoader.hpp"

#include <fstream>
#include <vector>
#include <map>
#include <assert.h>

#include "FileSystem.hpp"
#include "Vector.hpp"

using namespace Pht;

namespace {
    constexpr auto maxLineSize = 128;
    constexpr auto unknownIndex = -1;
    
    struct Quantities {
        int mVertexCount {0};
        int mTextureCoordCount {0};
        int mNormalCount {0};
        int mIndexCount {0};
    };
    
    struct VertexRef {
        int mVertexIndex {0};
        int mTextureCoordIndex {0};
        int mNormalIndex {0};
        
        bool operator<(const VertexRef& other) const {
            if (mVertexIndex < other.mVertexIndex) {
                return true;
            } else if (mVertexIndex == other.mVertexIndex) {
                if (mTextureCoordIndex < other.mTextureCoordIndex) {
                    return true;
                } else if (mTextureCoordIndex == other.mTextureCoordIndex) {
                    return mNormalIndex < other.mNormalIndex;
                } else {
                    return false;
                }
            } else {
                return false;
            }
        }
    };
    
    struct VertexRefs {
        std::map<VertexRef, int> mNewIndices;
        std::vector<VertexRef> mVertexRefVector;
    };
    
    Quantities GetQuantities(const std::string& filename) {
        Quantities quantities;
        std::ifstream objFile {filename};
        assert(objFile.is_open());
        
        while (objFile) {
            switch (objFile.get()) {
                case 'v':
                    switch (objFile.get()) {
                        case ' ':
                            quantities.mVertexCount++;
                            break;
                        case 't':
                            quantities.mTextureCoordCount++;
                            break;
                        case 'n':
                            quantities.mNormalCount++;
                            break;
                        default:
                            break;
                    }
                    break;
                case 'f':
                    quantities.mIndexCount += 3;
                    break;
                default:
                    break;
            }
            
            objFile.ignore(maxLineSize, '\n');
        }
        
        return quantities;
    }
    
    void ReadVertices(std::vector<Vec3>& vertices, const std::string& filename) {
        std::ifstream objFile {filename};
        assert(objFile.is_open());
        auto index = 0;
        
        while (objFile) {
            if (objFile.get() == 'v' && objFile.get() == ' ') {
                assert(index < vertices.size());
                
                auto& vertex = vertices[index];
                objFile >> vertex.x;
                objFile >> vertex.y;
                objFile >> vertex.z;
                
                ++index;
            }
            
            objFile.ignore(maxLineSize, '\n');
        }
    }

    void ReadTextureCoords(std::vector<Vec2>& textureCoords, const std::string& filename) {
        std::ifstream objFile {filename};
        assert(objFile.is_open());
        auto index = 0;
        
        while (objFile) {
            if (objFile.get() == 'v' && objFile.get() == 't') {
                assert(index < textureCoords.size());
                
                auto& textureCoord = textureCoords[index];
                objFile >> textureCoord.x;
                objFile >> textureCoord.y;
                
                textureCoord.y = 1.0f - textureCoord.y;
                
                ++index;
            }
            
            objFile.ignore(maxLineSize, '\n');
        }
    }

    void ReadNormals(std::vector<Vec3>& normals, const std::string& filename) {
        std::ifstream objFile {filename};
        assert(objFile.is_open());
        auto index = 0;
        
        while (objFile) {
            if (objFile.get() == 'v' && objFile.get() == 'n') {
                assert(index < normals.size());
                
                auto& normal = normals[index];
                objFile >> normal.x;
                objFile >> normal.y;
                objFile >> normal.z;
                
                normal.Normalize();
                
                ++index;
            }
            
            objFile.ignore(maxLineSize, '\n');
        }
    }
    
    void ReadVertexIndices(VertexRefs& vertexRefs,
                           const Quantities& quantities,
                           std::ifstream& objFile) {
        VertexRef vertexRef;
        
        objFile >> vertexRef.mVertexIndex;
        vertexRef.mVertexIndex--;
        assert(vertexRef.mVertexIndex < quantities.mVertexCount);
        
        if (objFile.peek() == '/') {
            objFile.get();
            
            if (objFile.peek() != '/') {
                objFile >> vertexRef.mTextureCoordIndex;
                vertexRef.mTextureCoordIndex--;
                assert(vertexRef.mTextureCoordIndex < quantities.mTextureCoordCount);
            }
            
            if (objFile.peek() == '/') {
                objFile.get();
            
                objFile >> vertexRef.mNormalIndex;
                vertexRef.mNormalIndex--;
                assert(vertexRef.mNormalIndex < quantities.mNormalCount);
            } else {
                vertexRef.mNormalIndex = vertexRef.mVertexIndex;
            }
        } else {
            vertexRef.mNormalIndex = vertexRef.mVertexIndex;
        }
        
        vertexRefs.mNewIndices.insert(std::make_pair(vertexRef, unknownIndex));
        vertexRefs.mVertexRefVector.push_back(vertexRef);
    }
    
    void ReadFaces(VertexRefs& vertexRefs,
                   const Quantities& quantities,
                   const std::string& filename) {
        std::ifstream objFile {filename};
        assert(objFile.is_open());
        
        while (objFile) {
            if (objFile.get() == 'f' && objFile.get() == ' ') {
                ReadVertexIndices(vertexRefs, quantities, objFile);
                ReadVertexIndices(vertexRefs, quantities, objFile);
                ReadVertexIndices(vertexRefs, quantities, objFile);
            }
            
            objFile.ignore(maxLineSize, '\n');
        }
    }
    
    void InitNormals(std::vector<Vec3>& normals, int numNormals) {
        normals.resize(numNormals);
        
        for (auto& normal: normals) {
            normal = Vec3 {0.0f, 0.0f, 0.0f};
        }
    }
    
    void CalculateNormals(std::vector<Vec3>& normals,
                          const std::vector<Vec3>& vertices,
                          VertexRefs& vertexRefs) {
        InitNormals(normals, static_cast<int>(vertices.size()));
        
        const auto& faceVertexRefs = vertexRefs.mVertexRefVector;
        assert(faceVertexRefs.size() % 3 == 0);
        
        for (auto i = 0; i < faceVertexRefs.size(); ) {
            auto vertexIndex0 = faceVertexRefs[i].mVertexIndex;
            auto vertexIndex1 = faceVertexRefs[i + 1].mVertexIndex;
            auto vertexIndex2 = faceVertexRefs[i + 2].mVertexIndex;
            
            Vec3 u {vertices[vertexIndex1] - vertices[vertexIndex0]};
            Vec3 v {vertices[vertexIndex2] - vertices[vertexIndex0]};
            Vec3 faceNormal {u.Cross(v)};
            
            normals[vertexIndex0] += faceNormal;
            normals[vertexIndex1] += faceNormal;
            normals[vertexIndex2] += faceNormal;
            
            i += 3;
        }
        
        for (auto& normal: normals) {
            normal.Normalize();
        }
    }
    
    void MoveVerticesToOrigin(std::vector<Vec3>& vertices) {
        auto maxVal = 32000.0f;
        auto minVal = -maxVal;
        auto xMax = minVal;
        auto yMax = minVal;
        auto zMax = minVal;
        auto xMin = maxVal;
        auto yMin = maxVal;
        auto zMin = maxVal;
        
        for (auto& vertex: vertices) {
            if (vertex.x > xMax) {
                xMax = vertex.x;
            }

            if (vertex.y > yMax) {
                yMax = vertex.y;
            }

            if (vertex.z > zMax) {
                zMax = vertex.z;
            }
            
            if (vertex.x < xMin) {
                xMin = vertex.x;
            }

            if (vertex.y < yMin) {
                yMin = vertex.y;
            }

            if (vertex.z < zMin) {
                zMin = vertex.z;
            }
        }
        
        Vec3 objectCenter {(xMax + xMin) / 2.0f, (yMax + yMin) / 2.0f, (zMax + zMin) / 2.0f};
        
        for (auto& vertex: vertices) {
            vertex -= objectCenter;
        }
    }
    
    void SetNewIndices(VertexRefs& vertexRefs) {
        auto newIndex = 0;
        for (auto& keyValuePair: vertexRefs.mNewIndices) {
            keyValuePair.second = newIndex++;
        }
    }
    
    void WriteVertices(VertexBuffer& vertexBuffer,
                       VertexFlags flags,
                       const std::vector<Vec3>& vertices,
                       const std::vector<Vec2>& textureCoords,
                       const std::vector<Vec3>& normals,
                       VertexRefs& vertexRefs,
                       float scale) {
        SetNewIndices(vertexRefs);
        
        for (auto& keyValuePair: vertexRefs.mNewIndices) {
            auto& vertexRef = keyValuePair.first;
            vertexBuffer.Write(vertices[vertexRef.mVertexIndex] * scale,
                               normals[vertexRef.mNormalIndex],
                               flags.mTextureCoords ? textureCoords[vertexRef.mTextureCoordIndex] :
                               Pht::Vec2 {});
        }
        
        for (const auto& vertexRef: vertexRefs.mVertexRefVector) {
            auto i = vertexRefs.mNewIndices.find(vertexRef);
            assert(i != vertexRefs.mNewIndices.end());
            
            vertexBuffer.AddIndex(i->second);
        }
    }
}


std::unique_ptr<VertexBuffer> ObjMeshLoader::Load(const std::string& filename,
                                                  VertexFlags attributeFlags,
                                                  float scale,
                                                  MoveMeshToOrigin moveMeshToOrigin) {
    assert(attributeFlags.mNormals);

    auto fullPath = FileSystem::GetResourceDirectory() + "/" + filename;
    auto quantities = GetQuantities(fullPath);
    
    std::vector<Vec3> vertices(quantities.mVertexCount);
    ReadVertices(vertices, fullPath);
    
    if (moveMeshToOrigin == MoveMeshToOrigin::Yes) {
        MoveVerticesToOrigin(vertices);
    }
    
    std::vector<Vec2> textureCoords(quantities.mTextureCoordCount);
    ReadTextureCoords(textureCoords, fullPath);
    
    std::vector<Vec3> normals(quantities.mNormalCount);
    ReadNormals(normals, fullPath);

    VertexRefs vertexRefs;
    ReadFaces(vertexRefs, quantities, fullPath);
    
    auto vertexCount =
        static_cast<int>(normals.empty() ? vertices.size() : vertexRefs.mNewIndices.size());
    auto vertexBuffer = std::make_unique<VertexBuffer>(vertexCount,
                                                       quantities.mIndexCount,
                                                       attributeFlags);
    if (normals.empty()) {
        CalculateNormals(normals, vertices, vertexRefs);
    }
    
    WriteVertices(*vertexBuffer,
                  attributeFlags,
                  vertices,
                  textureCoords,
                  normals,
                  vertexRefs,
                  scale);
    return vertexBuffer;
}
