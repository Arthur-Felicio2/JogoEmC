#include "PassDeviceScene.hpp"
#include "MatchScene.hpp"
#include "core/SceneManager.hpp"
#include "ui/Theme.hpp"
#include "ui/UiLayout.h"
#include <cmath>

namespace Starlight {

PassDeviceScene::PassDeviceScene(std::shared_ptr<Match> match, int moveA)
    : m_match(std::move(match)),
      m_moveA(moveA),
      m_btnReady("Estou Pronto! ->", Rectangle{490, 470, 300, 56}, ButtonStyle::Primary) {

    m_btnReady.setOnClick([this]() {
        if (m_manager) {
            auto matchScene = std::make_unique<MatchScene>(m_match);
            matchScene->setPlayer1Move(m_moveA);
            m_manager->changeScene(std::move(matchScene), 0.25f);
        }
    });
}

void PassDeviceScene::update(float dt) {
    m_animTimer += dt;
    m_btnReady.update(dt);

    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER) || IsKeyPressed(KEY_SPACE)) {
        if (m_manager) {
            auto matchScene = std::make_unique<MatchScene>(m_match);
            matchScene->setPlayer1Move(m_moveA);
            m_manager->changeScene(std::move(matchScene), 0.25f);
        }
    }
}

void PassDeviceScene::draw() {
    Theme::drawBackgroundGradient();

    Rectangle card = UiLayout::centerCard(580.0f, 440.0f);
    Theme::drawCard(card, 22.0f, true);

    // Lock / Privacy icon
    Vector2 lockCenter = { card.x + card.width * 0.5f, card.y + 65 };
    DrawCircle(static_cast<int>(lockCenter.x), static_cast<int>(lockCenter.y), 28, Theme::NeonPurple);
    DrawRectangle(static_cast<int>(lockCenter.x - 14), static_cast<int>(lockCenter.y), 28, 20, Theme::CardSurfaceDark);
    DrawCircleLines(static_cast<int>(lockCenter.x), static_cast<int>(lockCenter.y - 10), 10, Theme::CardSurfaceDark);

    // Heading
    std::string title = "PASSE O DISPOSITIVO!";
    int titleW = Theme::measureText(title, 26);
    Theme::drawText(title, static_cast<int>(card.x + (card.width - titleW) * 0.5f), static_cast<int>(card.y + 115), 26, Theme::NeonPurple);

    // Player 2 instruction
    std::string p1Name = m_match->playerA()->name();
    std::string p2Name = m_match->playerB()->name();
    std::string prompt1 = "A jogada de " + p1Name + " foi registrada em segredo.";
    int p1W = Theme::measureText(prompt1, 18);
    Theme::drawText(prompt1, static_cast<int>(card.x + (card.width - p1W) * 0.5f), static_cast<int>(card.y + 165), 18, Theme::TextMuted);

    std::string prompt2 = "Agora é a vez de: " + p2Name;
    int p2W = Theme::measureText(prompt2, 24);
    Theme::drawText(prompt2, static_cast<int>(card.x + (card.width - p2W) * 0.5f), static_cast<int>(card.y + 205), 24, Theme::TextBright);

    std::string prompt3 = "Passe o dispositivo e clique quando estiver pronto.";
    int p3W = Theme::measureText(prompt3, 17);
    Theme::drawText(prompt3, static_cast<int>(card.x + (card.width - p3W) * 0.5f), static_cast<int>(card.y + 250), 17, Theme::TextNormal);

    m_btnReady.setBounds(Rectangle{card.x + (card.width - 300.0f) * 0.5f, card.y + card.height - 85.0f, 300.0f, 56.0f});
    m_btnReady.draw();
}

} // namespace Starlight
