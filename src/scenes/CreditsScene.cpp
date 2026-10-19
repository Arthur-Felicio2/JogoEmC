#include "CreditsScene.hpp"
#include "MenuScene.hpp"

#if defined(__has_include)
  #if __has_include("core/SceneManager.hpp")
    #include "core/SceneManager.hpp"
    #include "ui/Theme.hpp"
    #include "ui/UiLayout.h"
  #else
    #include "../core/SceneManager.hpp"
    #include "../ui/Theme.hpp"
    #include "../ui/UiLayout.h"
  #endif
#else
  #include "../core/SceneManager.hpp"
  #include "../ui/Theme.hpp"
  #include "../ui/UiLayout.h"
#endif
#include <cmath>

namespace Starlight {

CreditsScene::CreditsScene()
    : m_btnBack("<- Voltar ao Menu", Rectangle{490, 580, 300, 56}, ButtonStyle::Primary) {
    m_btnBack.setOnClick([this]() {
        if (m_manager) {
            m_manager->changeScene(std::make_unique<MenuScene>());
        }
    });
}

void CreditsScene::update(float dt) {
    m_animTimer += dt;
    m_btnBack.update(dt);

    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) {
        if (m_manager) {
            m_manager->changeScene(std::make_unique<MenuScene>());
        }
    }
}

void CreditsScene::draw() {
    Theme::drawBackgroundGradient();

    // Main Card
    Rectangle card = UiLayout::centerCard(620.0f, 490.0f);
    card.y = 65.0f;
    Theme::drawCard(card, 22.0f, true);

    // Title
    const char* title = "CRÉDITOS DO PROJETO";
    int titleW = Theme::measureText(title, 26);
    Theme::drawText(title, static_cast<int>(card.x + (card.width - titleW) * 0.5f), static_cast<int>(card.y + 26), 26, Theme::NeonPurple);

    // Star icon
    Vector2 starPos = {card.x + card.width * 0.5f, card.y + 78};
    DrawPoly(starPos, 5, 14.0f, m_animTimer * 20.0f, Theme::NeonPurple);
    DrawPolyLinesEx(starPos, 5, 14.0f, m_animTimer * 20.0f, 2.0f, Theme::TextBright);

    // Organization
    const char* org = "STARLIGHT COMPANY";
    int orgW = Theme::measureText(org, 22);
    Theme::drawText(org, static_cast<int>(card.x + (card.width - orgW) * 0.5f), static_cast<int>(card.y + 104), 22, Theme::TextBright);

    DrawLine(static_cast<int>(card.x + 60), static_cast<int>(card.y + 144),
             static_cast<int>(card.x + card.width - 60), static_cast<int>(card.y + 144),
             Theme::BorderSubtle);

    // Authors
    Theme::drawText("Desenvolvido por:", static_cast<int>(card.x + 70), static_cast<int>(card.y + 168), 18, Theme::TextMuted);

    Theme::drawText("• Arthur Moreira Felicio", static_cast<int>(card.x + 70), static_cast<int>(card.y + 204), 22, Theme::TextBright);
    Theme::drawText("Número: 5", static_cast<int>(card.x + 440), static_cast<int>(card.y + 204), 20, Theme::NeonPurple);

    Theme::drawText("• Gabriel Sulzbacher Coimbra de Souza", static_cast<int>(card.x + 70), static_cast<int>(card.y + 248), 22, Theme::TextBright);
    Theme::drawText("Número: 10", static_cast<int>(card.x + 440), static_cast<int>(card.y + 248), 20, Theme::NeonPurple);

    // Academic Class
    Theme::drawText("Turma:", static_cast<int>(card.x + 70), static_cast<int>(card.y + 304), 18, Theme::TextMuted);
    Theme::drawText("INI-1A 2024", static_cast<int>(card.x + 150), static_cast<int>(card.y + 302), 22, Theme::NeonPurple);

    DrawLine(static_cast<int>(card.x + 60), static_cast<int>(card.y + 348),
             static_cast<int>(card.x + card.width - 60), static_cast<int>(card.y + 348),
             Theme::BorderSubtle);

    // Thank you message
    const char* thanks = "Obrigado por Jogar! :)";
    int thanksW = Theme::measureText(thanks, 24);
    Theme::drawText(thanks, static_cast<int>(card.x + (card.width - thanksW) * 0.5f), static_cast<int>(card.y + 375), 24, Theme::ActionPlayAgain);

    const char* tech = "Construído em C++17 com Raylib & Design System Moderno";
    int techW = Theme::measureText(tech, 16);
    Theme::drawText(tech, static_cast<int>(card.x + (card.width - techW) * 0.5f), static_cast<int>(card.y + 420), 16, Theme::TextMuted);

    m_btnBack.setBounds(Rectangle{card.x + (card.width - 300.0f) * 0.5f, card.y + card.height + 20.0f, 300.0f, 54.0f});
    m_btnBack.draw();
}

} // namespace Starlight
