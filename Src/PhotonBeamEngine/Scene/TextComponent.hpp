#ifndef TextComponent_hpp
#define TextComponent_hpp

#include "ISceneObjectComponent.hpp"
#include "Font.hpp"

namespace Pht {
    class SceneObject;

    class TextComponent: public ISceneObjectComponent {
    public:
        static const ComponentId id;
        
        TextComponent(SceneObject& sceneObject,
                      const std::string& text,
                      const TextProperties& properties);
        
        SceneObject& GetSceneObject() {
            return mSceneObject;
        }
        
        const SceneObject& GetSceneObject() const {
            return mSceneObject;
        }
        
        const std::string& GetText() const {
            return mText;
        }
        
        std::string& GetText() {
            return mText;
        }

        const TextProperties& GetProperties() const {
            return mProperties;
        }
        
        TextProperties& GetProperties() {
            return mProperties;
        }
        
    private:
        SceneObject& mSceneObject;
        std::string mText;
        TextProperties mProperties;
    };
}

#endif
