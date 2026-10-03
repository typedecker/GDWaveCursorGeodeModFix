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
        // Put the cursor directly on the running scene instead of Geode's
        // persistent OverlayManager. Eclipse's Cocos UI can live above the
        // OverlayManager in the scene graph, so local z-order there cannot
        // place the cursor above Eclipse. If no scene exists yet, keep the
        // cursor in OverlayManager until the next update() reparents it.
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

    // auto trailType = Mod::get()->getSettingValue<std::string>("trail-type");
    // log::info("Creating trailType {}", trailType);
    if(this->m_enableTrail) {
        // if (trailType == "Plain Trail") {
            // this->m_cursor->m_trail = SimpleCursor::Plain;
            this->m_cursor->createPlainTrail();
        /* }  else if (trailType == "Ghost Trail") {
            this->m_cursor->createGhostTrail();
        } else {
            log::error("Could not find trailType {}", trailType);
        } */
    } else {
        this->m_cursor->disableAllTrails();
    }
}

void CursorManager::update() {
    auto scene = cocos2d::CCDirector::get()->getRunningScene();
    if (scene) {
        // Eclipse's Cocos UI can be a sibling of Geode's OverlayManager.
        // Reparent the cursor/trails to the actual scene and reorder them on
        // every frame so they are the last Cocos nodes drawn, including when
        // an overlay is opened after WaveCursor.
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

    this->m_cursor->setPosition(getMousePos());
    this->m_cursor->bringToFront();

    // Outside an active level, WaveCursor should always be available.
    //
    // While a level is actively running, Geometry Dash normally requests that
    // the cursor be hidden (m_show becomes false), so the custom cursor stays
    // hidden too. Overlays such as Eclipse can request the cursor again while
    // the level is still active; in that case m_show becomes true and the
    // custom cursor is shown over the overlay. Pause/completion screens also
    // keep it visible regardless of the last cursor request.
    bool shouldShow = true;
    if (auto* playLayer = PlayLayer::get()) {
        const bool pausedOrCompleted =
            playLayer->m_isPaused || playLayer->m_hasCompletedLevel;

        shouldShow = pausedOrCompleted || this->m_show;
    }

    this->m_cursor->setVisible(shouldShow);

    if (this->m_enableTrail) {
        this->m_cursor->setVisibleTrail(shouldShow);
    }
}

void CursorManager::setCursorSize(int size) {
    this->m_cursorSize = ((float)size)/100;
}

void CursorManager::enableDisableTrail(bool state) {
    this->m_enableTrail = state;
}
