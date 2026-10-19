#include "MenuScene.hpp"
#include "ModeScene.hpp"
#include "NameEntryScene.hpp"
#include "ScoreScene.hpp"
#include "CreditsScene.hpp"
#include "core/SceneManager.hpp"
#include "core/Application.hpp"
#include "ui/Theme.hpp"
#include "ui/UiLayout.h"
#include <cmath>

namespace Starlight {

MenuScene::MenuScene()
    : m_btnOddEven("1. PAR OU ÍMPAR", Rectangle{450, 195, 380, 56}, ButtonStyle::Primary),
      m_btnRps("2. JOKENPÔ", Rectangle{450, 263, 380, 56}, ButtonStyle::Primary),
      m_btnFlappy("3. STARLIGHT BIRD", Rectangle{450, 331, 380, 56}, ButtonStyle::Secondary),
      m_btnScores("🏆 PLACAR & RECORDES", Rectangle{450, 399, 380, 50}, ButtonStyle::Secondary),
      m_btnCredits("ℹ️ CRÉDITOS", Rectangle{450, 461, 380, 50}, ButtonStyle::Secondary),
      m_btnExit("🚪 SAIR DO JOGO", Rectangle{450, 523, 380, 50}, ButtonStyle::Danger) {

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

    // Number keys directly select games
    if (IsKeyPressed(KEY_ONE)) {
        if (m_manager) m_manager->changeScene(std::make_unique<ModeScene>(GameType::OddEven));
    } else if (IsKeyPressed(KEY_TWO)) {
        if (m_manager) m_manager->changeScene(std::make_unique<ModeScene>(GameType::Rps));
    } else if (IsKeyPressed(KEY_THREE)) {
        if (m_manager) m_manager->changeScene(std::make_unique<NameEntryScene>(GameType::Flappy));
    }

    for (auto* btn : m_buttons) {
        btn->update(dt);
    }
}

void MenuScene::draw() {
    Theme::drawBackgroundGradient();

    // Responsive Center Card (Blueprint 4.2)
    Rectangle card = UiLayout::centerCard(480.0f, 620.0f);
    card.y = 45.0f;
    Theme::drawCard(card, 22.0f, true);

    // Header Emblem
    Vector2 starPos = { card.x + card.width * 0.5f, card.y + 36.0f };
    DrawPoly(starPos, 5, 18.0f, m_animTimer * 25.0f, Theme::NeonPurple);
    DrawPolyLinesEx(starPos, 5, 18.0f, m_animTimer * 25.0f, 2.0f, Theme::TextBright);

    // Title
    const char* title = "STARLIGHT GAMES";
    int titleW = Theme::measureText(title, 30);
    Theme::drawText(title, static_cast<int>(card.x + (card.width - titleW) * 0.5f), static_cast<int>(card.y + 64.0f), 30, Theme::TextBright);

    const char* sub = "Par ou Ímpar  •  Jokenpô  •  Bird";
    int subW = Theme::measureText(sub, 17);
    Theme::drawText(sub, static_cast<int>(card.x + (card.width - subW) * 0.5f), static_cast<int>(card.y + 104.0f), 17, Theme::NeonPurple);

    const char* prompt = "Selecione uma opção para jogar:";
    int promptW = Theme::measureText(prompt, 15);
    Theme::drawText(prompt, static_cast<int>(card.x + (card.width - promptW) * 0.5f), static_cast<int>(card.y + 130.0f), 15, Theme::TextMuted);

    // Reposition buttons responsively inside card
    float startY = card.y + 158.0f;
    float btnW = card.width * 0.86f;
    float btnX = card.x + (card.width - btnW) * 0.5f;

    m_btnOddEven.setBounds(Rectangle{btnX, startY, btnW, 54.0f});
    m_btnRps.setBounds(Rectangle{btnX, startY + 62.0f, btnW, 54.0f});
    m_btnFlappy.setBounds(Rectangle{btnX, startY + 124.0f, btnW, 54.0f});
    m_btnScores.setBounds(Rectangle{btnX, startY + 186.0f, btnW, 48.0f});
    m_btnCredits.setBounds(Rectangle{btnX, startY + 242.0f, btnW, 48.0f});
    m_btnExit.setBounds(Rectangle{btnX, startY + 298.0f, btnW, 48.0f});

    // Draw all menu buttons
    for (auto* btn : m_buttons) {
        btn->draw();
    }

    auto& atlas = Application::instance().handAtlas();

    // Decorative waving hand on the left (Pose 5 = waving open hand, blue sleeve)
    float waveAngle = sinf(m_animTimer * 4.0f) * 14.0f;
    Vector2 handPos = { 180, 570 + sinf(m_animTimer * 2.0f) * 8.0f };
    DrawEllipse(static_cast<int>(handPos.x), 575, 60, 16, Theme::ShadowColor);
    atlas.draw(5, 1, 0, false, handPos, 4.5f, waveAngle, WHITE);

    // Decorative fist on the right (Pose 0 = fist, red sleeve)
    Vector2 handRightPos = { 1100, 570 + cosf(m_animTimer * 2.0f) * 8.0f };
    DrawEllipse(static_cast<int>(handRightPos.x), 575, 60, 16, Theme::ShadowColor);
    atlas.draw(0, 2, 1, true, handRightPos, 4.5f, -waveAngle, WHITE);

    // Footer info
    const char* footL = "v2.0 • Paleta Preto & Roxo";
    Theme::drawText(footL, 40, 685, 15, Theme::TextMuted);

    const char* footR = "Pressione [ENTER] / Clique";
    int footRW = Theme::measureText(footR, 15);
    Theme::drawText(footR, 1280 - 40 - footRW, 685, 15, Theme::TextMuted);
}

} // namespace Starlight
