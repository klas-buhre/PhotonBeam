#ifndef GuiView_hpp
#define GuiView_hpp

#include <memory>

#include "Vector.hpp"
#include "SceneObject.hpp"
#include "SceneResources.hpp"
#include "Font.hpp"

namespace Pht {
    class TextComponent;
    class ISceneManager;
    
    class GuiView {
    public:
        GuiView();
        virtual ~GuiView() {}
        
        virtual void OnDeactivate() {}
        
        void SetPosition(const Vec2& position);
        void SetPosition(const Vec3& position);
        void SetIsActive(bool isActive);
        void AddSceneObject(std::unique_ptr<SceneObject> sceneObject);
        TextComponent& CreateText(const Vec3& position,
                                  const std::string& text,
                                  const TextProperties& properties);
        TextComponent& CreateText(const Vec3& position,
                                  const std::string& text,
                                  const TextProperties& properties,
                                  Pht::SceneObject& parent);
        SceneObject& CreateSceneObject(const IMesh& mesh,
                                       const Material& material,
                                       ISceneManager& sceneManager);
        SceneObject& CreateSceneObject();
        Vec2 GetPosition() const;
        
        void SetSize(const Vec2& size) {
            mSize = size;
        }
        
        const Vec2& GetSize() const {
            return mSize;
        }
        
        Pht::SceneObject& GetRoot() {
            return *mRoot;
        }
        
        SceneResources& GetSceneResources() {
            return mSceneResources;
        }
        
    private:
        Vec2 mSize {0.0f, 0.0f};
        std::unique_ptr<Pht::SceneObject> mRoot;
        SceneResources mSceneResources;
    };
}

#endif
