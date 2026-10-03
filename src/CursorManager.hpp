#pragma once

#include "Singleton.hpp"
#include "Cursor.hpp"

class CursorManager: public Singleton<CursorManager> {
private:
    float m_cursorSize;
    bool m_enableTrail = false;


    Ref<SimpleCursor> m_cursor = nullptr;

public:
    void createCursor();
    void enableDisableTrail(bool state);
    void update();
    void prepareForSceneDraw();
    void renderAfterScene();
    void setCursorSize(int size);
    bool m_show = false;
    bool m_shouldShow = false;
};