#include "MenuScene.hpp"
#include "ModeScene.hpp"
#include "NameEntryScene.hpp"
#include "ScoreScene.hpp"
#include "CreditsScene.hpp"
#include "core/SceneManager.hpp"
#include "core/Application.hpp"
#include "ui/Theme.hpp"
#include <cmath>

namespace Starlight {

MenuScene::MenuScene()
    : m_btnOddEven("Par ou Ímpar", Rectangle{490, 205, 300, 50}, ButtonStyle::Primary),
      m_btnRps("Jokenpô", Rectangle{490, 265, 300, 50}, ButtonStyle::Secondary),
      m_btnFlappy("Starlight Bird (Flappy)", Rectangle{490, 325, 300, 50}, ButtonStyle::Secondary),
      m_btnScores("Placar & Recordes", Rectangle{490, 385, 300, 50}, ButtonStyle::Secondary),
      m_btnCredits("Créditos", Rectangle{490, 445, 300, 50}, ButtonStyle::Secondary),
      m_btnExit("Sair do Jogo", Rectangle{490, 505, 300, 50}, ButtonStyle::Danger) {

    m_buttons = { &m_btnOddEven, &m_btnRps, &m_btnFlappy, &m_btnScores, &m_btnCredits, &m_btnExit };
    m_buttons[0]->setFocused(true);

    m_btnOddEven.setOnClick([this]() {
        if (m_manager) {
            m_manager->changeScene(std::make_unique<ModeScene>(GameType::OddEven));
        }
    });

    m_btnRps.setOnClick([this]() {
        if (m_manager) {
            m_manager->changeScene(std::make_unique<ModeScene>(GameType::Rps));
        }
    });

    m_btnFlappy.setOnClick([this]() {
        if (m_manager) {
            m_manager->changeScene(std::make_unique<NameEntryScene>(GameType::Flappy));
        }
    });

    m_btnScores.setOnClick([this]() {
        if (m_manager) {
            m_manager->changeScene(std::make_unique<ScoreScene>(0));
        }
    });

    m_btnCredits.setOnClick([this]() {
        if (m_manager) {
            m_manager->changeScene(std::make_unique<CreditsScene>());
        }
    });

    m_btnExit.setOnClick([]() {
        CloseWindow();
    });
}

void MenuScene::update(float dt) {
    m_animTimer += dt;

    // Keyboard navigation (UP/DOWN/TAB)
    if (IsKeyPressed(KEY_DOWN) || (IsKeyPressed(KEY_TAB) && !IsKeyDown(KEY_LEFT_SHIFT))) {
        m_buttons[m_focusedIndex]->setFocused(false);
        m_focusedIndex = (m_focusedIndex + 1) % static_cast<int>(m_buttons.size());
        m_buttons[m_focusedIndex]->setFocused(true);
    } else if (IsKeyPressed(KEY_UP) || (IsKeyPressed(KEY_TAB) && IsKeyDown(KEY_LEFT_SHIFT))) {
        m_buttons[m_focusedIndex]->setFocused(false);
        m_focusedIndex = (m_focusedIndex - 1 + static_cast<int>(m_buttons.size())) % static_cast<int>(m_buttons.size());
        m_buttons[m_focusedIndex]->setFocused(true);
    }

    for (auto* btn : m_buttons) {
        btn->update(dt);
    }
}

void MenuScene::draw() {
    Theme::drawBackgroundGradient();

    // Central Glass Card for Menu
    Rectangle card = { 430, 50, 420, 570 };
    Theme::drawCard(card, 20.0f, true);

    // Header Emblem
    Vector2 starPos = { 640, 95 };
    DrawPoly(starPos, 5, 18.0f, m_animTimer * 25.0f, Theme::ACCENT_GOLD);

    // Title
    const char* title = "STARLIGHT GAMES";
    int titleW = Theme::measureText(title, 30);
    Theme::drawText(title, static_cast<int>(640 - titleW * 0.5f), 122, 30, Theme::ACCENT_GOLD);

    const char* sub = "Selecione uma opção para jogar:";
    int subW = Theme::measureText(sub, 17);
    Theme::drawText(sub, static_cast<int>(640 - subW * 0.5f), 162, 17, Theme::TEXT_SECONDARY);

    // Draw all menu buttons
    for (auto* btn : m_buttons) {
        btn->draw();
    }

    auto& atlas = Application::instance().handAtlas();

    // Decorative waving hand on the left (Pose 5 = waving open hand, blue sleeve)
    float waveAngle = sinf(m_animTimer * 4.0f) * 14.0f;
    Vector2 handPos = { 210, 560 + sinf(m_animTimer * 2.0f) * 8.0f };
    DrawEllipse(static_cast<int>(handPos.x), 565, 60, 16, Color{0, 0, 0, 70});
    atlas.draw(5, 1, 0, false, handPos, 4.5f, waveAngle, WHITE);

    // Decorative fist on the right (Pose 0 = fist, red sleeve)
    Vector2 handRightPos = { 1070, 560 + cosf(m_animTimer * 2.0f) * 8.0f };
    DrawEllipse(static_cast<int>(handRightPos.x), 565, 60, 16, Color{0, 0, 0, 70});
    atlas.draw(0, 2, 1, true, handRightPos, 4.5f, -waveAngle, WHITE);

    // Footer
    const char* foot = "Starlight Company © 2026 • Turma INI-1A";
    int footW = Theme::measureText(foot, 15);
    Theme::drawText(foot, static_cast<int>(640 - footW * 0.5f), 670, 15, Theme::TEXT_SECONDARY);
}

} // namespace Starlight
