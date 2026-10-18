#pragma once
#include "raylib.h"

namespace Starlight {

class Trophy {
public:
    Trophy(Vector2 center = {640, 320}, float scale = 1.0f);

    void setPosition(Vector2 center) { m_center = center; }
    void setScale(float scale) { m_scale = scale; }

    void update(float dt);
    void draw() const;

private:
    Vector2 m_center;
    float m_scale;
    float m_animTimer = 0.0f;
};

} // namespace Starlight
