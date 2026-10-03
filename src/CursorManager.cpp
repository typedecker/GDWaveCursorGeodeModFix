#include "CursorManager.hpp"
#include "Geode/ui/OverlayManager.hpp"
#include "Cursor.hpp"
#include <Geode/binding/GameManager.hpp>
#include <Geode/binding/PlatformToolbox.hpp>
#include <Geode/binding/PlayLayer.hpp>
#include <limits>

namespace {
    constexpr float kCursorGlobalZOrder = 1000000.0f;
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
        OverlayManager::get()->addChild(this->m_cursor);
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
    this->m_cursor->setZOrder(std::numeric_limits<int>::max());
    this->m_cursor->setGlobalZOrder(kCursorGlobalZOrder);
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
    this->m_cursor->setPosition(getMousePos());
    // Eclipse can be a separate branch of the scene graph, so local z-order
    // alone cannot put WaveCursor above it. Use global z-order and refresh the
    // whole cursor subtree because SimplePlayer may replace child sprites.
    this->m_cursor->setZOrder(std::numeric_limits<int>::max());
    this->m_cursor->setGlobalZOrder(kCursorGlobalZOrder);
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
