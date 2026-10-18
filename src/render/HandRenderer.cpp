#include "HandRenderer.hpp"
#include "ui/Theme.hpp"
#include <algorithm>

namespace Starlight {

HandRenderer& HandRenderer::instance() {
    static HandRenderer inst;
    return inst;
}

HandRenderer::HandRenderer() = default;

HandRenderer::~HandRenderer() {
    unload();
}

void HandRenderer::init() {
    if (m_initialized) return;
    m_atlas.init();
    m_initialized = true;
}

void HandRenderer::unload() {
    if (!m_initialized) return;
    m_atlas.unloadAll();
    m_initialized = false;
}

void HandRenderer::drawHand(int pose,
                            Vector2 wristPos,
                            float scale,
                            bool mirror,
                            int skinTone,
                            int sleeve,
                            float rotation,
                            float alpha) {
    Color tint = WHITE;
    tint.a = static_cast<unsigned char>(std::clamp(alpha, 0.0f, 1.0f) * 255.0f);
    m_atlas.draw(pose, skinTone, sleeve, mirror, wristPos, scale, rotation, tint);
}

void HandRenderer::drawHand(int pose,
                            Vector2 wristPos,
                            float scale,
                            bool mirror,
                            Color skinColor,
                            float rotation,
                            float alpha) {
    // Map skinColor to skin tone index (0..3) based on Theme::skinTones
    int skinIndex = 0;
    const auto& tones = Theme::skinTones();
    int minDiff = 999999;
    for (size_t i = 0; i < tones.size(); ++i) {
        int dr = static_cast<int>(skinColor.r) - static_cast<int>(tones[i].color.r);
        int dg = static_cast<int>(skinColor.g) - static_cast<int>(tones[i].color.g);
        int db = static_cast<int>(skinColor.b) - static_cast<int>(tones[i].color.b);
        int diff = dr * dr + dg * dg + db * db;
        if (diff < minDiff) {
            minDiff = diff;
            skinIndex = static_cast<int>(i);
        }
    }

    int sleeveIndex = mirror ? 1 : 0; // Left = Blue (0), Right = Red (1)
    drawHand(pose, wristPos, scale, mirror, skinIndex, sleeveIndex, rotation, alpha);
}

} // namespace Starlight
