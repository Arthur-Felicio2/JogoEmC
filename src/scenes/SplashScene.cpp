#include "SplashScene.hpp"
#include "MenuScene.hpp"
#include "core/SceneManager.hpp"
#include "ui/Theme.hpp"
#include <cmath>

namespace Starlight {

SplashScene::SplashScene() = default;

void SplashScene::update(float dt) {
    m_timer += dt;
    m_pulse = sinf(m_timer * 3.0f);

    // Skip with click or space, or auto-transition after 2.2 seconds
    if (m_timer >= 2.2f || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (m_manager) {
            m_manager->changeScene(std::make_unique<MenuScene>(), 0.4f);
        }
    }
}

void SplashScene::draw() {
    Theme::drawBackgroundGradient();

    // Starlight purple neon halo glow
    float glowRadius = 160.0f + m_pulse * 20.0f;
    DrawCircleGradient(640, 310, glowRadius, Color{138, 63, 252, 70}, Color{0, 0, 0, 0});

    // Central Emblem (Starlight Star)
    Vector2 center = {640, 310};
    DrawPoly(center, 5, 46.0f + m_pulse * 3.0f, m_timer * 15.0f, Theme::NeonPurple);
    DrawPolyLinesEx(center, 5, 46.0f + m_pulse * 3.0f, m_timer * 15.0f, 3.0f, Theme::TextBright);

    // Title
    const char* brand = "STARLIGHT COMPANY";
    int brandW = Theme::measureText(brand, 38);
    Theme::drawText(brand, 640 - brandW / 2, 385, 38, Theme::TextBright);

    const char* sub = "Apresenta a Coleção de Jogos Clássicos";
    int subW = Theme::measureText(sub, 20);
    Theme::drawText(sub, 640 - subW / 2, 435, 20, Theme::NeonPurple);

    // Subtle press to continue tip
    const char* tip = "Pressione ESPAÇO ou CLIQUE para continuar...";
    int tipW = Theme::measureText(tip, 16);
    float alphaTip = 0.5f + 0.5f * sinf(m_timer * 4.0f);
    Color tipCol = Theme::TextMuted;
    tipCol.a = static_cast<unsigned char>(255 * alphaTip);
    Theme::drawText(tip, 640 - tipW / 2, 620, 16, tipCol);
}

} // namespace Starlight
