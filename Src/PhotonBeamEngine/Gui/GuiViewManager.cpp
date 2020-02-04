#include "GuiViewManager.hpp"

#include <assert.h>

// Engine includes.
#include "GuiView.hpp"
#include "SceneObject.hpp"

using namespace Pht;

void GuiViewManager::Init(SceneObject& parentObject) {
    mActiveView = nullptr;

    for (auto& viewEntry: mViews) {
        auto* view = viewEntry.second;
        view->SetIsActive(false);
        parentObject.AddChild(view->GetRoot());
    }
}

void GuiViewManager::AddView(int viewId, GuiView& view) {
    mViews[viewId] = &view;
}

void GuiViewManager::ActivateView(int viewIndex) {
    DeactivateAllViews();
    
    auto viewEntry = mViews.find(viewIndex);
    if (viewEntry == std::end(mViews)) {
        assert(!"View not found");
    }
    
    mActiveView = viewEntry->second;
    mActiveView->SetIsActive(true);
}

void GuiViewManager::DeactivateAllViews() {
    if (mActiveView) {
        mActiveView->OnDeactivate();
        mActiveView = nullptr;
    }

    for (auto& viewEntry: mViews) {
        auto* view = viewEntry.second;
        view->SetIsActive(false);
    }
}
