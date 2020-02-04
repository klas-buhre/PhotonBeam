#ifndef StaticBatcher_hpp
#define StaticBatcher_hpp

#include <memory>

#include "Optional.hpp"

namespace Pht {
    class SceneObject;
    class RenderableObject;
    
    namespace StaticBatcher {
        std::unique_ptr<RenderableObject> CreateBatch(const SceneObject& sceneObject,
                                                      const Optional<std::string>& batchVertexBufferName);
    };
}
#endif
