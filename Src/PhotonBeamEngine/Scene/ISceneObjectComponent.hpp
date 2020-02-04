#ifndef ISceneObjectComponent_hpp
#define ISceneObjectComponent_hpp

#include <cstdint>

namespace Pht {
    using ComponentId = uint32_t;
    
    class ISceneObjectComponent {
    public:
        virtual ~ISceneObjectComponent() {}
    };
}

#endif
