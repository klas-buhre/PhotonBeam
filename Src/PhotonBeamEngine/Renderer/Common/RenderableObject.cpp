#include "RenderableObject.hpp"

#include <assert.h>

#include "VertexBuffer.hpp"
#include "IMesh.hpp"
#include "VertexBufferCache.hpp"

using namespace Pht;

namespace {
    std::shared_ptr<GpuVertexBuffer> CreateGpuVertexBuffer(RenderMode renderMode) {
        auto generateIndexBuffer =
            renderMode == RenderMode::Triangles ? GenerateIndexBuffer::Yes : GenerateIndexBuffer::No;
    
        return std::make_shared<GpuVertexBuffer>(generateIndexBuffer);
    }
}

RenderableObject::RenderableObject(const Material& material,
                                   const IMesh& mesh,
                                   const VertexFlags& attributeFlags,
                                   VertexBufferLocation bufferLocation) :
    mMaterial {material} {

    auto meshName = mesh.GetName();
    if (meshName.HasValue()) {
        mGpuVertexBuffer = VertexBufferCache::Get(meshName.GetValue());
        if (mGpuVertexBuffer == nullptr) {
            mGpuVertexBuffer = CreateGpuVertexBuffer(mRenderMode);
            UploadMeshVertexData(mesh, attributeFlags, bufferLocation);
            VertexBufferCache::Add(meshName.GetValue(), mGpuVertexBuffer);
        } else {
            if (bufferLocation == VertexBufferLocation::AtGpuAndCpu &&
                mGpuVertexBuffer->GetCpuSideBuffer() == nullptr) {
                
                auto vertexBuffer = mesh.CreateVertexBuffer(attributeFlags);
                mGpuVertexBuffer->SetCpuSideBuffer(std::move(vertexBuffer));
            }
        }
    } else {
        mGpuVertexBuffer = CreateGpuVertexBuffer(mRenderMode);
        UploadMeshVertexData(mesh, attributeFlags, bufferLocation);
    }
}

RenderableObject::RenderableObject(const Material& material,
                                   std::unique_ptr<VertexBuffer> cpuSideBuffer,
                                   RenderMode renderMode) :
    mRenderMode {renderMode},
    mMaterial {material},
    mGpuVertexBuffer {CreateGpuVertexBuffer(renderMode)} {

    mGpuVertexBuffer->SetCpuSideBuffer(std::move(cpuSideBuffer));
}

RenderableObject::RenderableObject(const Material& material,
                                   const VertexBuffer& fromBuffer,
                                   const Optional<std::string>& bufferName) :
    mMaterial {material} {
    
    mGpuVertexBuffer = CreateGpuVertexBuffer(mRenderMode);
    mGpuVertexBuffer->UploadTriangles(fromBuffer, BufferUsage::StaticDraw);
    
    if (bufferName.HasValue()) {
        VertexBufferCache::Add(bufferName.GetValue(), mGpuVertexBuffer);
    }
}

RenderableObject::RenderableObject(const Material& material,
                                   std::shared_ptr<GpuVertexBuffer> buffer) :
    mMaterial {material},
    mGpuVertexBuffer {buffer} {}

RenderableObject::~RenderableObject() {}

void RenderableObject::UploadMeshVertexData(const IMesh& mesh,
                                            const VertexFlags& attributeFlags,
                                            VertexBufferLocation bufferLocation) {
    auto vertexBuffer = mesh.CreateVertexBuffer(attributeFlags);
    mGpuVertexBuffer->UploadTriangles(*vertexBuffer, BufferUsage::StaticDraw);
    
    if (bufferLocation == VertexBufferLocation::AtGpuAndCpu) {
        mGpuVertexBuffer->SetCpuSideBuffer(std::move(vertexBuffer));
    }
}

void RenderableObject::UploadTriangles(BufferUsage bufferUsage) {
    assert(mRenderMode == RenderMode::Triangles);
    auto* cpuSideBuffer = mGpuVertexBuffer->GetCpuSideBuffer();
    assert(cpuSideBuffer);
    mGpuVertexBuffer->UploadTriangles(*cpuSideBuffer, bufferUsage);
}

void RenderableObject::UploadPoints(BufferUsage bufferUsage) {
    assert(mRenderMode == RenderMode::Points);
    auto* cpuSideBuffer = mGpuVertexBuffer->GetCpuSideBuffer();
    assert(cpuSideBuffer);
    mGpuVertexBuffer->UploadPoints(*cpuSideBuffer, bufferUsage);
}
