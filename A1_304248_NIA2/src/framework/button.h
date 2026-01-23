#pragma once
#include "image.h"
#include "framework.h" // Vector2

enum ButtonType {
    BTN_PENCIL,
    BTN_ERASER,
    BTN_LINE,
    BTN_RECT,
    BTN_TRI,

    BTN_COLOR_WHITE,
    BTN_COLOR_RED,
    BTN_COLOR_GREEN,
    BTN_COLOR_BLUE,
    BTN_COLOR_YELLOW,
    BTN_COLOR_PINK,
    BTN_COLOR_CYAN,
    BTN_COLOR_BLACK,

    BTN_CLEAR,
    BTN_LOAD,
    BTN_SAVE
};


class Button {
public:
    Image icon;
    Vector2 pos;
    ButtonType type;

    int w() const { return (int)icon.width; }
    int h() const { return (int)icon.height; }

    bool IsMouseInside(Vector2 m) const
    {
        return m.x >= pos.x && m.x < pos.x + w() &&
            m.y >= pos.y && m.y < pos.y + h();
    }
};
