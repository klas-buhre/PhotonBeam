#ifndef RowBlastApplication_hpp
#define RowBlastApplication_hpp

// Engine includes.
#include "IApplication.hpp"

namespace Pht {
    class IEngine;
}

namespace Game {
    class GameApplication: public Pht::IApplication {
    public:
        GameApplication(Pht::IEngine& engine);
    
        void OnInitialize() override {}
        void OnUpdate() override;
    };
}

#endif
