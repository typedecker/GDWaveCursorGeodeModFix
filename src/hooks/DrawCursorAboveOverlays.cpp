#include "../CursorManager.hpp"
#include <Geode/modify/CCDirector.hpp>

using namespace geode::prelude;

// Draw the WaveCursor after the complete Cocos2d scene has rendered.  This is
// intentionally handled at the director level instead of relying on a node's
// z-order, because in-level mod menus such as Eclipse can live in a different
// overlay branch of the scene graph.
class $modify(WaveCursorCCDirector, cocos2d::CCDirector) {
    static void onModify(auto& self) {
        if (!self.setHookPriorityPost("cocos2d::CCDirector::drawScene", Priority::Last)) {
            log::warn("WaveCursor: failed to set CCDirector::drawScene hook priority");
        }
    }

    void drawScene() {
        auto manager = CursorManager::get();

        // Keep the cursor out of the normal scene traversal so it is rendered
        // exactly once, after all other Cocos2d UI.
        manager->prepareForSceneDraw();

        cocos2d::CCDirector::drawScene();

        // Priority::Last + Post means this executes after lower-priority
        // drawScene hooks have completed, putting WaveCursor above their UI.
        manager->renderAfterScene();
    }
};
