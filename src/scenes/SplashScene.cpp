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

    // Starlight halo glow
    float glowRadius = 140.0f + m_pulse * 15.0f;
    DrawCircleGradient(640, 310, glowRadius, Color{244, 185, 66, 60}, Color{0, 0, 0, 0});

    // Central Emblem (Starlight Star)
    Vector2 center = {640, 310};
    DrawPoly(center, 5, 45.0f + m_pulse * 3.0f, m_timer * 15.0f, Theme::ACCENT_GOLD);
    DrawPolyLinesEx(center, 5, 45.0f + m_pulse * 3.0f, m_timer * 15.0f, 3.0f, Color{255, 235, 160, 255});

    // Title
    const char* brand = "STARLIGHT COMPANY";
    int brandW = MeasureText(brand, 38);
    DrawText(brand, 640 - brandW / 2, 385, 38, Theme::ACCENT_GOLD);

    const char* sub = "Apresenta a Coleção de Jogos Clássicos";
    int subW = MeasureText(sub, 20);
    DrawText(sub, 640 - subW / 2, 435, 20, Theme::TEXT_SECONDARY);

    // Subtle press to continue tip
    const char* tip = "Pressione ESPAÇO para continuar...";
    int tipW = MeasureText(tip, 16);
    float alphaTip = 0.5f + 0.5f * sinf(m_timer * 4.0f);
    DrawText(tip, 640 - tipW / 2, 620, 16, Color{245, 247, 242, (unsigned char)(255 * alphaTip)});
}

} // namespace Starlight
