#pragma once
#include "raylib.h"

namespace Starlight {

class Input {
public:
    static void setViewport(const Rectangle& viewport, float scale) {
        s_viewport = viewport;
        s_scale = scale;
    }

    static Vector2 getVirtualMousePosition() {
        Vector2 screenMouse = GetMousePosition();
        Vector2 virtualPos;
        virtualPos.x = (screenMouse.x - s_viewport.x) / s_scale;
        virtualPos.y = (screenMouse.y - s_viewport.y) / s_scale;
        return virtualPos;
    }

    static bool isVirtualMouseInside(const Rectangle& rect) {
        Vector2 m = getVirtualMousePosition();
        return CheckCollisionPointRec(m, rect);
    }

private:
    inline static Rectangle s_viewport = {0, 0, 1280, 720};
    inline static float s_scale = 1.0f;
};

} // namespace Starlight
