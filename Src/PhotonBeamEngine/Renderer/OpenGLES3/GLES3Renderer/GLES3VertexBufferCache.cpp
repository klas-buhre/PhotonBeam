#include "VertexBufferCache.hpp"

#include <mutex>
#include <vector>

#define GLES_SILENCE_DEPRECATION

#include <OpenGLES/ES3/gl.h>

#include "GLES3Handles.hpp"

using namespace Pht;

namespace {
    static std::mutex mutex;
    static std::vector<std::pair<std::string, std::weak_ptr<GpuVertexBuffer>>> cache;
    
    GLenum ToGLBufferUsage(BufferUsage bufferUsage) {
        switch (bufferUsage) {
            case BufferUsage::StaticDraw:
                return GL_STATIC_DRAW;
            case BufferUsage::DynamicDraw:
                return GL_DYNAMIC_DRAW;
        }
    }
}

uint32_t GpuVertexBuffer::mIdCounter = 0;

GpuVertexBuffer::GpuVertexBuffer(GenerateIndexBuffer generateIndexBuffer) :
    mHandles {std::make_unique<GpuVertexBufferHandles>()} {
    
    glGenBuffers(1, &mHandles->mGLVertexBufferHandle);
    
    if (generateIndexBuffer == GenerateIndexBuffer::Yes) {
        glGenBuffers(1, &mHandles->mGLIndexBufferHandle);
    }
}

GpuVertexBuffer::~GpuVertexBuffer() {
    glDeleteBuffers(1, &mHandles->mGLVertexBufferHandle);
    glDeleteBuffers(1, &mHandles->mGLIndexBufferHandle);
}

void GpuVertexBuffer::UploadTriangles(const VertexBuffer& vertexBuffer, BufferUsage bufferUsage) {
    auto glBufferUsage = ToGLBufferUsage(bufferUsage);
    
    glBindBuffer(GL_ARRAY_BUFFER, mHandles->mGLVertexBufferHandle);
    glBufferData(GL_ARRAY_BUFFER,
                 vertexBuffer.GetVertexBufferSize() * sizeof(float),
                 vertexBuffer.GetVertexBuffer(),
                 glBufferUsage);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mHandles->mGLIndexBufferHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 vertexBuffer.GetIndexBufferSize() * sizeof(GLushort),
                 vertexBuffer.GetIndexBuffer(),
                 glBufferUsage);
    
    mIndexCount = vertexBuffer.GetIndexBufferSize();
}

void GpuVertexBuffer::UploadPoints(const VertexBuffer& vertexBuffer, BufferUsage bufferUsage) {
    glBindBuffer(GL_ARRAY_BUFFER, mHandles->mGLVertexBufferHandle);
    glBufferData(GL_ARRAY_BUFFER,
                 vertexBuffer.GetVertexBufferSize() * sizeof(float),
                 vertexBuffer.GetVertexBuffer(),
                 ToGLBufferUsage(bufferUsage));
    
    mPointCount = vertexBuffer.GetNumVertices();
}

std::shared_ptr<GpuVertexBuffer> VertexBufferCache::Get(const std::string& meshName) {
    std::lock_guard<std::mutex> guard {mutex};
    
    cache.erase(
        std::remove_if(
            std::begin(cache),
            std::end(cache),
            [] (const auto& entry) {
                return entry.second.lock() == std::shared_ptr<GpuVertexBuffer>();
            }),
        std::end(cache));

    for (const auto& entry: cache) {
        if (auto buffer = entry.second.lock()) {
            auto& key = entry.first;
            if (meshName == key) {
                return buffer;
            }
        }
    }
    
    return nullptr;
}

void VertexBufferCache::Add(const std::string& meshName, std::shared_ptr<GpuVertexBuffer> buffer) {
    std::lock_guard<std::mutex> guard {mutex};
    
    cache.emplace_back(meshName, buffer);
}
