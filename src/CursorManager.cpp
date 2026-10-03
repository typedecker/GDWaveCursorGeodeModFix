#include "CursorManager.hpp"
#include "Geode/ui/OverlayManager.hpp"
#include "Cursor.hpp"
#include <Geode/binding/GameManager.hpp>
#include <Geode/binding/PlatformToolbox.hpp>
#include <Geode/binding/PlayLayer.hpp>
#include <limits>

namespace {
    constexpr int kCursorTopZOrder = std::numeric_limits<int>::max();
}

void CursorManager::createCursor() {
    auto gameManager = GameManager::get();
    if (!this->m_cursor) {
        auto data = SimpleCursor::CursorData(
            gameManager->getPlayerColor(),
            gameManager->getPlayerColor2(),
            gameManager->getPlayerDart(),
            gameManager->getPlayerGlow(),
            gameManager->getPlayerGlowColor()
        );
        this->m_cursor = SimpleCursor::create(data);
        this->m_cursor->setID("cursor"_spr);

        // Keep the cursor in the running scene so its position uses the same
        // coordinate space as the game and its in-game trails. The final draw
        // is handled by the CCDirector hook after the scene is rendered.
        if (auto scene = cocos2d::CCDirector::get()->getRunningScene()) {
            scene->addChild(this->m_cursor, kCursorTopZOrder);
        } else {
            OverlayManager::get()->addChild(this->m_cursor, kCursorTopZOrder);
        }
    } else {
        auto data = SimpleCursor::CursorData(
            gameManager->getPlayerColor(),
            gameManager->getPlayerColor2(),
            gameManager->getPlayerDart(),
            gameManager->getPlayerGlow(),
            gameManager->getPlayerGlowColor()
        );
        this->m_cursor->updateCursor(data);
    }

    this->m_cursor->setAnchorPoint(ccp(1.0f, 0.5f));
    this->m_cursor->setScale(this->m_cursorSize);
    this->m_cursor->setZOrder(kCursorTopZOrder);
    this->m_cursor->bringToFront();

    if (this->m_enableTrail) {
        this->m_cursor->createPlainTrail();
    } else {
        this->m_cursor->disableAllTrails();
    }
}

void CursorManager::update() {
    auto scene = cocos2d::CCDirector::get()->getRunningScene();
    if (scene && this->m_cursor) {
        // Keep cursor/trails under the active scene. They are hidden during the
        // normal scene traversal and manually visited after drawScene().
        if (this->m_cursor->getParent() != scene) {
            this->m_cursor->removeFromParentAndCleanup(false);
            scene->addChild(this->m_cursor, kCursorTopZOrder);
        } else {
            scene->reorderChild(this->m_cursor, kCursorTopZOrder);
        }

        if (auto trail = this->m_cursor->getPlainTrail()) {
            if (trail->getParent() != scene) {
                trail->removeFromParentAndCleanup(false);
                scene->addChild(trail, kCursorTopZOrder);
            } else {
                scene->reorderChild(trail, kCursorTopZOrder);
            }
        }
        if (auto trail = this->m_cursor->getGhostTrail()) {
            if (trail->getParent() != scene) {
                trail->removeFromParentAndCleanup(false);
                scene->addChild(trail, kCursorTopZOrder);
            } else {
                scene->reorderChild(trail, kCursorTopZOrder);
            }
        }
        if (auto trail = this->m_cursor->getHardTrail()) {
            if (trail->getParent() != scene) {
                trail->removeFromParentAndCleanup(false);
                scene->addChild(trail, kCursorTopZOrder);
            } else {
                scene->reorderChild(trail, kCursorTopZOrder);
            }
        }
    }

    if (!this->m_cursor) return;

    this->m_cursor->setPosition(getMousePos());
    this->m_cursor->bringToFront();

    // Outside an active level, WaveCursor remains visible. During active
    // gameplay it follows the platform cursor visibility request; this lets
    // overlays such as Eclipse explicitly show the cursor while open.
    bool shouldShow = true;
    if (auto* playLayer = PlayLayer::get()) {
        const bool pausedOrCompleted =
            playLayer->m_isPaused || playLayer->m_hasCompletedLevel;
        shouldShow = pausedOrCompleted || this->m_show;
    }

    this->m_shouldShow = shouldShow;
    this->m_cursor->setVisible(shouldShow);

    if (this->m_enableTrail) {
        this->m_cursor->setVisibleTrail(shouldShow);
    }
}

void CursorManager::prepareForSceneDraw() {
    if (!this->m_cursor) return;

    // Prevent the cursor/trails from being drawn during the normal scene pass.
    // renderAfterScene() draws them once at the very end instead.
    this->m_cursor->setVisible(false);
    this->m_cursor->setVisibleTrail(false);
}

void CursorManager::renderAfterScene() {
    if (!this->m_cursor || !this->m_shouldShow) return;

    // drawScene() has finished, so this visit happens after the other Cocos2d
    // scene/UI nodes, independent of their parent z-order.
    this->m_cursor->setVisible(true);
    this->m_cursor->visit();

    if (this->m_enableTrail) {
        if (auto trail = this->m_cursor->getPlainTrail()) {
            trail->setVisible(true);
            trail->visit();
        }
        if (auto trail = this->m_cursor->getGhostTrail()) {
            trail->setVisible(true);
            trail->visit();
        }
        if (auto trail = this->m_cursor->getHardTrail()) {
            trail->setVisible(true);
            trail->visit();
        }
    }

    // Leave the nodes hidden after the final pass. The next update() will set
    // the desired state before the following frame is drawn.
    this->m_cursor->setVisible(false);
    this->m_cursor->setVisibleTrail(false);
}

void CursorManager::setCursorSize(int size) {
    this->m_cursorSize = ((float)size) / 100;
}

void CursorManager::enableDisableTrail(bool state) {
    this->m_enableTrail = state;
}
