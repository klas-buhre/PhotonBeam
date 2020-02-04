#ifndef IMesh_h
#define IMesh_h

#include <memory>

#include "VertexBuffer.hpp"
#include "Optional.hpp"

namespace Pht {
    class IMesh {
    public:
        virtual ~IMesh() {}
        
        virtual Optional<std::string> GetName() const = 0;
        virtual std::unique_ptr<VertexBuffer> CreateVertexBuffer(VertexFlags attributeFlags) const = 0;
    };
}

#endif
