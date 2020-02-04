#ifndef GuiViewManager_hpp
#define GuiViewManager_hpp

#include <map>

#include "Noncopyable.hpp"

namespace Pht {
    class GuiView;
    class SceneObject;

    class GuiViewManager: public Noncopyable {
    public:
        void Init(SceneObject& parentObject);
        void AddView(int viewId, GuiView& view);
        void ActivateView(int viewId);
        void DeactivateAllViews();
        
    private:
        GuiView* mActiveView {nullptr};
        std::map<int, GuiView*> mViews;
    };
}

#endif
