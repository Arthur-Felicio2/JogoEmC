#include "Trophy.hpp"
#include "ui/Theme.hpp"
#include <cmath>

namespace Starlight {

Trophy::Trophy(Vector2 center, float scale)
    : m_center(center), m_scale(scale) {}

void Trophy::update(float dt) {
    m_animTimer += dt;
}

void Trophy::draw() const {
    float bounce = sinf(m_animTimer * 3.5f) * 8.0f;
    Vector2 c = {m_center.x, m_center.y + bounce};
    float s = m_scale * (1.0f + sinf(m_animTimer * 2.0f) * 0.03f);

    Color goldDark = Color{217, 145, 32, 255};
    Color goldMid = Color{244, 185, 66, 255};
    Color goldLight = Color{255, 225, 120, 255};
    Color baseWood = Color{70, 45, 30, 255};

    // Soft glow behind trophy
    DrawCircleGradient((int)c.x, (int)(c.y - 10 * s), 110 * s, Color{244, 185, 66, 40}, Color{0, 0, 0, 0});

    // Base pedestal
    Rectangle basePlinth = {c.x - 55 * s, c.y + 70 * s, 110 * s, 26 * s};
    DrawRectangleRounded(basePlinth, 0.3f, 6, baseWood);
    DrawRectangleRoundedLines(basePlinth, 0.3f, 6, 2.0f, goldDark);

    Rectangle baseStem = {c.x - 30 * s, c.y + 45 * s, 60 * s, 25 * s};
    DrawRectangleRounded(baseStem, 0.2f, 4, goldDark);

    // Cup Stem
    DrawCircle((int)c.x, (int)(c.y + 45 * s), 18 * s, goldMid);
    DrawRectangle((int)(c.x - 12 * s), (int)(c.y + 20 * s), (int)(24 * s), (int)(25 * s), goldMid);

    // Cup Handles (left and right)
    DrawRing(Vector2{c.x - 55 * s, c.y - 15 * s}, 18 * s, 28 * s, 90, 270, 16, goldMid);
    DrawRing(Vector2{c.x + 55 * s, c.y - 15 * s}, 18 * s, 28 * s, 270, 450, 16, goldMid);

    // Cup Body (golden chalice)
    Rectangle cupRect = {c.x - 55 * s, c.y - 50 * s, 110 * s, 70 * s};
    DrawRectangleRounded(cupRect, 0.6f, 16, goldMid);

    // Cup Rim
    DrawEllipse((int)c.x, (int)(c.y - 50 * s), 56 * s, 12 * s, goldLight);
    DrawEllipse((int)c.x, (int)(c.y - 50 * s), 48 * s, 8 * s, goldDark);

    // Star on the cup
    DrawPoly(Vector2{c.x, c.y - 15 * s}, 5, 18 * s, m_animTimer * 20.0f, goldLight);

    // Shimmer sparkles
    float sparkleTime = m_animTimer * 4.0f;
    float spAlpha = fabsf(sinf(sparkleTime));
    Color sparkleColor = Color{255, 255, 255, (unsigned char)(255 * spAlpha)};
    Vector2 spPos = {c.x + 35 * s, c.y - 45 * s};
    DrawLine((int)(spPos.x - 8 * s), (int)spPos.y, (int)(spPos.x + 8 * s), (int)spPos.y, sparkleColor);
    DrawLine((int)spPos.x, (int)(spPos.y - 8 * s), (int)spPos.x, (int)(spPos.y + 8 * s), sparkleColor);
}

} // namespace Starlight
