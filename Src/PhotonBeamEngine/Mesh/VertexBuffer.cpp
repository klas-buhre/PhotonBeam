#include "VertexBuffer.hpp"

using namespace Pht;

VertexBuffer::VertexBuffer(int vertexCapacity, int indexCapacity, const VertexFlags& attributeFlags) :
    mFlags {attributeFlags},
    mFloatsPerVertex {3} {
    
    if (mFlags.mNormals) {
        mFloatsPerVertex += 3;
    }
    if (mFlags.mTextureCoords) {
        mFloatsPerVertex += 2;
    }
    if (mFlags.mColors) {
        mFloatsPerVertex += 4;
    }
    if (mFlags.mPointSizes) {
        mFloatsPerVertex += 1;
    }
    
    mVertexBuffer.resize(vertexCapacity * mFloatsPerVertex);
    mVertexWritePtr = mVertexBuffer.data();
    mTriangleIndices.resize(indexCapacity);
    mIndexWritePtr = mTriangleIndices.data();
}

VertexBuffer::VertexBuffer(const VertexBuffer& other) {
    Copy(other);
}

VertexBuffer& VertexBuffer::operator=(const VertexBuffer& other) {
    if (&other == this) {
        return *this;
    }
    
    Copy(other);
    return *this;
}

void VertexBuffer::Copy(const VertexBuffer& other) {
    mFlags = other.mFlags;
    mFloatsPerVertex = other.mFloatsPerVertex;
    mVertexBuffer = other.mVertexBuffer;
    mTriangleIndices = other.mTriangleIndices;
    mSurfaceBeginVertex = other.mSurfaceBeginVertex;
    mNumVertices = other.mNumVertices;
    mNumIndices = other.mNumIndices;
    mVertexWritePtr = mVertexBuffer.data() + (mNumVertices * mFloatsPerVertex);
    mIndexWritePtr = mTriangleIndices.data() + mNumIndices;
}

void VertexBuffer::BeginSurface() {
    mSurfaceBeginVertex = mNumVertices;
}

void VertexBuffer::Reset() {
    mVertexWritePtr = mVertexBuffer.data();
    mIndexWritePtr = mTriangleIndices.data();
    mNumVertices = 0;
    mNumIndices = 0;
    BeginSurface();
}

void VertexBuffer::Write(const Vec3& position,
                         const Vec3& normal,
                         const Vec2& textureCoord,
                         const Vec4& color) {
    ReallocateIfNeeded();
    
    mVertexWritePtr = position.Write(mVertexWritePtr);
    
    if (mFlags.mNormals) {
        mVertexWritePtr = normal.Write(mVertexWritePtr);
    }
    
    if (mFlags.mTextureCoords) {
        mVertexWritePtr = textureCoord.Write(mVertexWritePtr);
    }

    if (mFlags.mColors) {
        mVertexWritePtr = color.Write(mVertexWritePtr);
    }

    ++mNumVertices;
}

void VertexBuffer::Write(const Vec3& position, const Vec2& textureCoord, const Vec4& color) {
    ReallocateIfNeeded();

    mVertexWritePtr = position.Write(mVertexWritePtr);

    if (mFlags.mTextureCoords) {
        mVertexWritePtr = textureCoord.Write(mVertexWritePtr);
    }

    if (mFlags.mColors) {
        mVertexWritePtr = color.Write(mVertexWritePtr);
    }

    ++mNumVertices;
}

void VertexBuffer::Write(const Vec3& position, const Vec4& color, float pointSize) {
    ReallocateIfNeeded();

    mVertexWritePtr = position.Write(mVertexWritePtr);
    
    if (mFlags.mColors) {
        mVertexWritePtr = color.Write(mVertexWritePtr);
    }
    
    if (mFlags.mPointSizes) {
        *mVertexWritePtr++ = pointSize;
    }
    
    ++mNumVertices;
}

void VertexBuffer::AddIndex(uint16_t index) {
    if (GetIndexBufferSize() >= GetIndexBufferCapacity()) {
        ReallocateIndexBuffer(GetIndexBufferCapacity() * 2 + 1);
    }
    
    *mIndexWritePtr++ = mSurfaceBeginVertex + index;
    ++mNumIndices;
}

const float* VertexBuffer::GetVertexBuffer() const {
    return mVertexBuffer.data();
}

const uint16_t* VertexBuffer::GetIndexBuffer() const {
    return mTriangleIndices.data();
}

int VertexBuffer::GetVertexBufferSize() const {
    return mNumVertices * mFloatsPerVertex;
}

int VertexBuffer::GetIndexBufferSize() const {
    return mNumIndices;
}

void VertexBuffer::ReallocateIfNeeded() {
    if (GetVertexBufferSize() >= GetVertexBufferCapacity()) {
        ReallocateVertexBuffer(static_cast<int>(GetVertexBufferSize() * 2 + mFloatsPerVertex));
    }
}

void VertexBuffer::ReallocateVertexBuffer(int newCapacity) {
    assert(!"Vertex buffer realocation!");
    mVertexBuffer.resize(newCapacity);
    mVertexWritePtr = mVertexBuffer.data() + (mNumVertices * mFloatsPerVertex);
}

void VertexBuffer::ReallocateIndexBuffer(int newCapacity) {
    assert(!"Index buffer realocation!");
    mTriangleIndices.resize(newCapacity);
    mIndexWritePtr = mTriangleIndices.data() + mNumIndices;
}

void VertexBuffer::TransformWithRotationAndAppendVertices(const VertexBuffer& sourceBuffer,
                                                          const Mat4& localTransformMatrix,
                                                          const Mat3& normalMatrix) {
    BeginSurface();
    
    auto newSize = GetVertexBufferSize() + sourceBuffer.GetVertexBufferSize();
    if (newSize > GetVertexBufferCapacity()) {
        ReallocateVertexBuffer(newSize);
    }

    auto attributeFlags = sourceBuffer.GetAttributeFlags();
    auto* vertexWrite = mVertexWritePtr;
    auto* vertexRead = sourceBuffer.GetVertexBuffer();
    auto sourceBufferNumVertices = sourceBuffer.GetNumVertices();

    for (auto i = 0; i < sourceBufferNumVertices; ++i) {
        // Vertex position is Vec3. Vec4 is needed for multiplication with 4x4 matrix.
        Vec4 position {*vertexRead++, *vertexRead++, *vertexRead++, 1.0f};
        auto transformedPosition = localTransformMatrix * position;
        *vertexWrite++ = transformedPosition.x;
        *vertexWrite++ = transformedPosition.y;
        *vertexWrite++ = transformedPosition.z;
    
        if (attributeFlags.mNormals) {
            // Vertex normal is Vec3.
            Vec3 normal {*vertexRead++, *vertexRead++, *vertexRead++};
            auto transformedNormal = normalMatrix * normal;
            *vertexWrite++ = transformedNormal.x;
            *vertexWrite++ = transformedNormal.y;
            *vertexWrite++ = transformedNormal.z;
        }
        
        if (attributeFlags.mTextureCoords) {
            // Vertex texture coord is Vec2.
            *vertexWrite++ = *vertexRead++;
            *vertexWrite++ = *vertexRead++;
        }
        
        if (attributeFlags.mColors) {
            // Vertex texture coord is Vec4.
            *vertexWrite++ = *vertexRead++;
            *vertexWrite++ = *vertexRead++;
            *vertexWrite++ = *vertexRead++;
            *vertexWrite++ = *vertexRead++;
        }
        
        if (attributeFlags.mPointSizes) {
            // Vertex texture coord is float.
            *vertexWrite++ = *vertexRead++;
        }
    }
    
    mVertexWritePtr = vertexWrite;
    mNumVertices += sourceBufferNumVertices;

    AppendIndices(sourceBuffer);
}

void VertexBuffer::TransformAndAppendVertices(const VertexBuffer& sourceBuffer,
                                              const Vec3& translation,
                                              const Vec3& scale) {
    BeginSurface();
    
    auto newSize = GetVertexBufferSize() + sourceBuffer.GetVertexBufferSize();
    if (newSize > GetVertexBufferCapacity()) {
        ReallocateVertexBuffer(newSize);
    }

    auto attributeFlags = sourceBuffer.GetAttributeFlags();
    auto* vertexWrite = mVertexWritePtr;
    auto* vertexRead = sourceBuffer.GetVertexBuffer();
    auto sourceBufferNumVertices = sourceBuffer.GetNumVertices();

    for (auto i = 0; i < sourceBufferNumVertices; ++i) {
        // Vertex position is Vec3.
        *vertexWrite++ = *vertexRead++ * scale.x + translation.x;
        *vertexWrite++ = *vertexRead++ * scale.y + translation.y;
        *vertexWrite++ = *vertexRead++ * scale.z + translation.z;
        
        if (attributeFlags.mNormals) {
            // Vertex normal is Vec3.
            *vertexWrite++ = *vertexRead++;
            *vertexWrite++ = *vertexRead++;
            *vertexWrite++ = *vertexRead++;
        }
        
        if (attributeFlags.mTextureCoords) {
            // Vertex texture coord is Vec2.
            *vertexWrite++ = *vertexRead++;
            *vertexWrite++ = *vertexRead++;
        }
        
        if (attributeFlags.mColors) {
            // Vertex texture coord is Vec4.
            *vertexWrite++ = *vertexRead++;
            *vertexWrite++ = *vertexRead++;
            *vertexWrite++ = *vertexRead++;
            *vertexWrite++ = *vertexRead++;
        }
        
        if (attributeFlags.mPointSizes) {
            // Vertex texture coord is float.
            *vertexWrite++ = *vertexRead++;
        }
    }
    
    mVertexWritePtr = vertexWrite;
    mNumVertices += sourceBufferNumVertices;

    AppendIndices(sourceBuffer);
}

void VertexBuffer::AppendIndices(const VertexBuffer& sourceBuffer) {
    auto sourceBufferNumIndices = sourceBuffer.GetNumIndices();
    if (sourceBufferNumIndices == 0) {
        return;
    }
    
    auto newSize = GetIndexBufferSize() + sourceBuffer.GetIndexBufferSize();
    if (newSize > GetIndexBufferCapacity()) {
        ReallocateIndexBuffer(newSize);
    }

    auto* indexWrite = mIndexWritePtr;
    auto* indexRead = sourceBuffer.GetIndexBuffer();
    for (auto i = 0; i < sourceBufferNumIndices; ++i) {
        *indexWrite++ = mSurfaceBeginVertex + *indexRead++;
    }
    
    mIndexWritePtr = indexWrite;
    mNumIndices += sourceBufferNumIndices;
}

int VertexBuffer::GetVertexBufferCapacity() const {
    return static_cast<int>(mVertexBuffer.size());
}

int VertexBuffer::GetIndexBufferCapacity() const {
    return static_cast<int>(mTriangleIndices.size());
}
