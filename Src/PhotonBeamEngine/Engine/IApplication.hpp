#ifndef IApplication_hpp
#define IApplication_hpp

#include <memory>

namespace Pht {
    class IApplication {
    public:
        virtual ~IApplication() {}
        
        virtual void OnInitialize() = 0;
        virtual void OnUpdate() = 0;
    };
    
    class IEngine;
}

std::unique_ptr<Pht::IApplication> CreateApplication(Pht::IEngine& engine);

#endif
