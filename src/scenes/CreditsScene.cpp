#include "CreditsScene.hpp"
#include "MenuScene.hpp"
#include "core/SceneManager.hpp"
#include "ui/Theme.hpp"
#include <cmath>

namespace Starlight {

CreditsScene::CreditsScene()
    : m_btnBack("Voltar ao Menu", Rectangle{490, 580, 300, 56}, ButtonStyle::Primary) {
    m_btnBack.setOnClick([this]() {
        if (m_manager) {
            m_manager->changeScene(std::make_unique<MenuScene>());
        }
    });
}

void CreditsScene::update(float dt) {
    m_animTimer += dt;
    m_btnBack.update(dt);
}

void CreditsScene::draw() {
    Theme::drawBackgroundGradient();

    // Main Card
    Rectangle card = {340, 70, 600, 480};
    Theme::drawCard(card, 18.0f, true);

    // Title
    const char* title = "CRÉDITOS DO PROJETO";
    int titleW = MeasureText(title, 28);
    DrawText(title, (int)(card.x + (card.width - titleW) * 0.5f), (int)(card.y + 30), 28, Theme::ACCENT_GOLD);

    // Star icon
    Vector2 starPos = {card.x + card.width * 0.5f, card.y + 85};
    DrawPoly(starPos, 5, 14.0f, m_animTimer * 20.0f, Theme::ACCENT_GOLD);

    // Organization
    const char* org = "STARLIGHT COMPANY";
    int orgW = MeasureText(org, 22);
    DrawText(org, (int)(card.x + (card.width - orgW) * 0.5f), (int)(card.y + 115), 22, Theme::TEXT_PRIMARY);

    DrawLine((int)(card.x + 80), (int)(card.y + 155), (int)(card.x + card.width - 80), (int)(card.y + 155), Color{255, 255, 255, 30});

    // Authors
    DrawText("Desenvolvido por:", (int)(card.x + 80), (int)(card.y + 180), 18, Theme::TEXT_SECONDARY);

    DrawText("• Arthur Moreira Felicio", (int)(card.x + 80), (int)(card.y + 215), 22, Theme::TEXT_PRIMARY);
    DrawText("Número: 5", (int)(card.x + 420), (int)(card.y + 215), 20, Theme::ACCENT_GOLD);

    DrawText("• Gabriel Sulzbacher Coimbra de Souza", (int)(card.x + 80), (int)(card.y + 260), 22, Theme::TEXT_PRIMARY);
    DrawText("Número: 10", (int)(card.x + 420), (int)(card.y + 260), 20, Theme::ACCENT_GOLD);

    // Academic Class
    DrawText("Turma:", (int)(card.x + 80), (int)(card.y + 315), 18, Theme::TEXT_SECONDARY);
    DrawText("INI-1A", (int)(card.x + 160), (int)(card.y + 312), 24, Theme::ACCENT_GOLD);

    DrawLine((int)(card.x + 80), (int)(card.y + 360), (int)(card.x + card.width - 80), (int)(card.y + 360), Color{255, 255, 255, 30});

    // Thank you message
    const char* thanks = "Obrigado por Jogar! :)";
    int thanksW = MeasureText(thanks, 24);
    DrawText(thanks, (int)(card.x + (card.width - thanksW) * 0.5f), (int)(card.y + 390), 24, Color{52, 211, 153, 255});

    const char* tech = "Construído em C++17 com Raylib & Design System Moderno";
    int techW = MeasureText(tech, 16);
    DrawText(tech, (int)(card.x + (card.width - techW) * 0.5f), (int)(card.y + 430), 16, Theme::TEXT_SECONDARY);

    m_btnBack.draw();
}

} // namespace Starlight
