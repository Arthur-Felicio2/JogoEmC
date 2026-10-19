#include "NameEntryScene.hpp"
#include "MatchScene.hpp"
#include "FlappyScene.hpp"
#include "ModeScene.hpp"
#include "MenuScene.hpp"
#include "core/SceneManager.hpp"
#include "core/Application.hpp"
#include "core/Input.hpp"
#include "ui/Theme.hpp"
#include "ui/UiLayout.h"
#include "games/OddEvenRules.hpp"
#include "games/RpsRules.hpp"
#include "games/HumanPlayer.hpp"
#include "games/CpuPlayer.hpp"
#include "games/Match.hpp"

namespace Starlight {

NameEntryScene::NameEntryScene(GameType gameType,
                               bool isTwoPlayers,
                               bool p1IsEven,
                               std::string defaultName1,
                               std::string defaultName2,
                               int skinTone1,
                               int skinTone2)
    : m_gameType(gameType),
      m_isTwoPlayers(isTwoPlayers),
      m_p1IsEven(p1IsEven),
      m_nameFieldP1(defaultName1, "Digite o nome...", Rectangle{0, 0, 320, 48}, 12),
      m_nameFieldP2(defaultName2, "Digite o nome...", Rectangle{0, 0, 320, 48}, 12),
      m_skinToneP1(skinTone1),
      m_skinToneP2(skinTone2),
      m_btnConfirm("Iniciar Partida ->", Rectangle{490, 560, 300, 56}, ButtonStyle::Primary),
      m_btnBack("<- Voltar", Rectangle{40, 24, 110, 44}, ButtonStyle::Secondary) {

    // Configure layout depending on 1P vs 2P vs Flappy
    if (m_isTwoPlayers) {
        m_nameFieldP1 = TextField(defaultName1, "Nome Jogador 1", Rectangle{280, 240, 320, 48}, 12);
        m_nameFieldP2 = TextField(defaultName2, "Nome Jogador 2", Rectangle{680, 240, 320, 48}, 12);
    } else {
        m_nameFieldP1 = TextField(defaultName1, "Seu nome", Rectangle{480, 260, 320, 48}, 12);
        if (m_gameType == GameType::Flappy) {
            m_btnConfirm.setText("Começar a Voar! ->");
        }
    }

    m_nameFieldP1.setActive(true);

    // Build quick profile shortcut chips from existing profiles
    const auto& profiles = Application::instance().scoreStore().all();
    int maxChips = 4;
    int chipCount = 0;

    for (const auto& p : profiles) {
        if (chipCount >= maxChips) break;
        std::string pName = p.name;

        if (m_isTwoPlayers) {
            Rectangle rectP1 = { 280.0f + chipCount * 80.0f, 380.0f, 74.0f, 32.0f };
            Button chip1(pName, rectP1, ButtonStyle::Secondary);
            chip1.setOnClick([this, pName]() {
                m_nameFieldP1.setText(pName);
            });
            m_chipsP1.push_back(chip1);

            Rectangle rectP2 = { 680.0f + chipCount * 80.0f, 380.0f, 74.0f, 32.0f };
            Button chip2(pName, rectP2, ButtonStyle::Secondary);
            chip2.setOnClick([this, pName]() {
                m_nameFieldP2.setText(pName);
            });
            m_chipsP2.push_back(chip2);
        } else {
            Rectangle rectP1 = { 480.0f + chipCount * 82.0f, 400.0f, 76.0f, 34.0f };
            Button chip1(pName, rectP1, ButtonStyle::Secondary);
            chip1.setOnClick([this, pName]() {
                m_nameFieldP1.setText(pName);
            });
            m_chipsP1.push_back(chip1);
        }
        chipCount++;
    }

    m_btnBack.setOnClick([this]() {
        if (!m_manager) return;
        if (m_gameType == GameType::Flappy) {
            m_manager->changeScene(std::make_unique<MenuScene>());
        } else {
            m_manager->changeScene(std::make_unique<ModeScene>(m_gameType));
        }
    });

    m_btnConfirm.setOnClick([this]() {
        confirmAndStart();
    });
}

void NameEntryScene::confirmAndStart() {
    if (!m_manager) return;

    std::string name1 = m_nameFieldP1.text();
    // Trim spaces
    while (!name1.empty() && name1.front() == ' ') name1.erase(name1.begin());
    while (!name1.empty() && name1.back() == ' ') name1.pop_back();
    if (name1.empty()) name1 = "Jogador 1";

    std::string name2 = m_nameFieldP2.text();
    while (!name2.empty() && name2.front() == ' ') name2.erase(name2.begin());
    while (!name2.empty() && name2.back() == ' ') name2.pop_back();
    if (name2.empty()) name2 = "Jogador 2";

    // Validation: in 2P, names cannot be identical (case-insensitive)
    if (m_isTwoPlayers && sl::ScoreStore::iequals(name1, name2)) {
        m_warningMessage = "Atenção: Os jogadores devem ter nomes diferentes!";
        return;
    }

    if (m_gameType == GameType::Flappy) {
        m_manager->changeScene(std::make_unique<FlappyScene>(name1, m_skinToneP1));
        return;
    }

    sl::GameSessionContext ctx;
    ctx.gameId = (m_gameType == GameType::OddEven) ? sl::GameId::OddEven : sl::GameId::Rps;
    ctx.mode = m_isTwoPlayers ? sl::GameMode::TwoPlayers : sl::GameMode::SinglePlayer;
    ctx.player1Name = name1;
    ctx.player2Name = m_isTwoPlayers ? name2 : "Computador";
    ctx.skinP1 = m_skinToneP1;
    ctx.skinP2 = m_skinToneP2;
    ctx.p1IsEven = m_p1IsEven;
    ctx.resetSession();

    auto matchScene = std::make_unique<MatchScene>(ctx, Application::instance().scoreStore());
    m_manager->changeScene(std::move(matchScene));
}

void NameEntryScene::update(float dt) {
    m_animTimer += dt;

    Rectangle card = m_isTwoPlayers ? UiLayout::centerCard(880.0f, 560.0f) : UiLayout::centerCard(540.0f, 540.0f);
    m_btnConfirm.setBounds(Rectangle{card.x + (card.width - 320.0f) * 0.5f, card.y + card.height - 75.0f, 320.0f, 54.0f});

    if (m_isTwoPlayers) {
        Rectangle p1Box = { card.x + 20, card.y + 90, (card.width - 60) * 0.5f, 310 };
        Rectangle p2Box = { p1Box.x + p1Box.width + 20, card.y + 90, p1Box.width, 310 };
        m_nameFieldP1.setBounds(Rectangle{p1Box.x + 20, p1Box.y + 75, p1Box.width - 40, 48});
        m_nameFieldP2.setBounds(Rectangle{p2Box.x + 20, p2Box.y + 75, p2Box.width - 40, 48});

        for (size_t c = 0; c < m_chipsP1.size(); ++c) {
            m_chipsP1[c].setBounds(Rectangle{p1Box.x + 20.0f + c * 82.0f, p1Box.y + 245.0f, 76.0f, 32.0f});
        }
        for (size_t c = 0; c < m_chipsP2.size(); ++c) {
            m_chipsP2[c].setBounds(Rectangle{p2Box.x + 20.0f + c * 82.0f, p2Box.y + 245.0f, 76.0f, 32.0f});
        }
    } else {
        m_nameFieldP1.setBounds(Rectangle{card.x + 80, card.y + 150, card.width - 160, 48});
        for (size_t c = 0; c < m_chipsP1.size(); ++c) {
            m_chipsP1[c].setBounds(Rectangle{card.x + 80.0f + c * 86.0f, card.y + 345.0f, 80.0f, 34.0f});
        }
    }

    m_btnBack.update(dt);
    m_btnConfirm.update(dt);

    m_nameFieldP1.update(dt);
    if (m_isTwoPlayers) {
        m_nameFieldP2.update(dt);
    }

    for (auto& c : m_chipsP1) c.update(dt);
    for (auto& c : m_chipsP2) c.update(dt);

    // Keyboard shortcuts: Enter confirms, Esc returns
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) {
        confirmAndStart();
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        if (m_manager) {
            if (m_gameType == GameType::Flappy) {
                m_manager->changeScene(std::make_unique<MenuScene>());
            } else {
                m_manager->changeScene(std::make_unique<ModeScene>(m_gameType));
            }
        }
    }

    // Tab toggles active text field in 2P
    if (m_isTwoPlayers && IsKeyPressed(KEY_TAB)) {
        bool p1Active = m_nameFieldP1.isActive();
        m_nameFieldP1.setActive(!p1Active);
        m_nameFieldP2.setActive(p1Active);
    }

    // Skin Tone selector clicks (exatamente nas mesmas coordenadas do layout)
    Vector2 mouse = Input::getVirtualMousePosition();
    if (m_isTwoPlayers) {
        Rectangle p1Box = { card.x + 20.0f, card.y + 90.0f, (card.width - 60.0f) * 0.5f, 310.0f };
        Rectangle p2Box = { p1Box.x + p1Box.width + 20.0f, card.y + 90.0f, p1Box.width, 310.0f };

        // Player 1 skins
        for (int i = 0; i < 4; ++i) {
            Vector2 center = { p1Box.x + 40.0f + i * 46.0f, p1Box.y + 175.0f };
            if (CheckCollisionPointCircle(mouse, center, 22.0f)) {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    m_skinToneP1 = i;
                }
            }
        }

        // Player 2 skins
        for (int i = 0; i < 4; ++i) {
            Vector2 center = { p2Box.x + 40.0f + i * 46.0f, p2Box.y + 175.0f };
            if (CheckCollisionPointCircle(mouse, center, 22.0f)) {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    m_skinToneP2 = i;
                }
            }
        }
    } else {
        // Player 1 skins (1P / Flappy)
        for (int i = 0; i < 4; ++i) {
            Vector2 center = { card.x + 120.0f + i * 54.0f, card.y + 265.0f };
            if (CheckCollisionPointCircle(mouse, center, 24.0f)) {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    m_skinToneP1 = i;
                }
            }
        }
    }
}

void NameEntryScene::draw() {
    Theme::drawBackgroundGradient();
    m_btnBack.draw();

    Vector2 mouse = Input::getVirtualMousePosition();
    auto& atlas = Application::instance().handAtlas();

    // Backdrop Card
    Rectangle card = m_isTwoPlayers ? UiLayout::centerCard(880.0f, 560.0f) : UiLayout::centerCard(540.0f, 540.0f);
    Theme::drawCard(card, 22.0f, true);

    // Header Title
    std::string title;
    if (m_gameType == GameType::Flappy) {
        title = "STARLIGHT BIRD — IDENTIFICAÇÃO";
    } else if (m_isTwoPlayers) {
        title = "IDENTIFICAÇÃO DOS JOGADORES (2P)";
    } else {
        title = "IDENTIFICAÇÃO DO JOGADOR (1P)";
    }

    int tw = Theme::measureText(title, 26);
    Theme::drawText(title, static_cast<int>(card.x + (card.width - tw) * 0.5f), static_cast<int>(card.y + 24), 26, Theme::NeonPurple);

    const char* sub = "Seus resultados e recordes serão salvos no placar!";
    int sw = Theme::measureText(sub, 16);
    Theme::drawText(sub, static_cast<int>(card.x + (card.width - sw) * 0.5f), static_cast<int>(card.y + 60), 16, Theme::TextMuted);

    // Player 1 Card Area
    if (m_isTwoPlayers) {
        Rectangle p1Box = { card.x + 20.0f, card.y + 90.0f, (card.width - 60.0f) * 0.5f, 310.0f };
        Theme::drawCard(p1Box, 14.0f, false, Theme::CardSurfaceDark);
        Theme::drawText("JOGADOR 1 (Manga Azul)", static_cast<int>(p1Box.x + 20), static_cast<int>(p1Box.y + 16), 18, Theme::NeonPurple);
        Theme::drawText("Nome:", static_cast<int>(p1Box.x + 20), static_cast<int>(p1Box.y + 50), 16, Theme::TextNormal);
        m_nameFieldP1.setBounds(Rectangle{p1Box.x + 20, p1Box.y + 75, p1Box.width - 40, 48});
        m_nameFieldP1.draw();

        std::string skinLabel1 = "Tom de Pele: " + Theme::skinTones()[m_skinToneP1].name;
        Theme::drawText(skinLabel1, static_cast<int>(p1Box.x + 20), static_cast<int>(p1Box.y + 135), 16, Theme::TextNormal);
        for (int i = 0; i < 4; ++i) {
            Vector2 center = { p1Box.x + 40.0f + i * 46.0f, p1Box.y + 175.0f };
            bool isHovered = CheckCollisionPointCircle(mouse, center, 20.0f);
            DrawCircle(static_cast<int>(center.x), static_cast<int>(center.y), 16, Theme::getSkinColor(i));
            if (m_skinToneP1 == i) {
                DrawCircleLines(static_cast<int>(center.x), static_cast<int>(center.y), 19, Theme::BorderGlow);
                DrawCircleLines(static_cast<int>(center.x), static_cast<int>(center.y), 20, Theme::BorderGlow);
            } else if (isHovered) {
                DrawCircleLines(static_cast<int>(center.x), static_cast<int>(center.y), 18, Theme::NeonPurple);
            }
        }

        // Live hand preview Player 1
        atlas.draw(0, m_skinToneP1, 0, false, Vector2{ p1Box.x + p1Box.width - 55.0f, p1Box.y + 175.0f }, 2.8f, 0.0f, WHITE);

        if (!m_chipsP1.empty()) {
            Theme::drawText("Perfis existentes:", static_cast<int>(p1Box.x + 20), static_cast<int>(p1Box.y + 215), 15, Theme::TextMuted);
            for (size_t c = 0; c < m_chipsP1.size(); ++c) {
                m_chipsP1[c].setBounds(Rectangle{p1Box.x + 20.0f + c * 82.0f, p1Box.y + 245.0f, 76.0f, 32.0f});
                m_chipsP1[c].draw();
            }
        }

        // Player 2 Box Area
        Rectangle p2Box = { p1Box.x + p1Box.width + 20.0f, card.y + 90.0f, p1Box.width, 310.0f };
        Theme::drawCard(p2Box, 14.0f, false, Theme::CardSurfaceDark);
        Theme::drawText("JOGADOR 2 (Manga Vermelha)", static_cast<int>(p2Box.x + 20), static_cast<int>(p2Box.y + 16), 18, Theme::ActionMenu);
        Theme::drawText("Nome:", static_cast<int>(p2Box.x + 20), static_cast<int>(p2Box.y + 50), 16, Theme::TextNormal);
        m_nameFieldP2.setBounds(Rectangle{p2Box.x + 20, p2Box.y + 75, p2Box.width - 40, 48});
        m_nameFieldP2.draw();

        std::string skinLabel2 = "Tom de Pele: " + Theme::skinTones()[m_skinToneP2].name;
        Theme::drawText(skinLabel2, static_cast<int>(p2Box.x + 20), static_cast<int>(p2Box.y + 135), 16, Theme::TextNormal);
        for (int i = 0; i < 4; ++i) {
            Vector2 center = { p2Box.x + 40.0f + i * 46.0f, p2Box.y + 175.0f };
            bool isHovered = CheckCollisionPointCircle(mouse, center, 20.0f);
            DrawCircle(static_cast<int>(center.x), static_cast<int>(center.y), 16, Theme::getSkinColor(i));
            if (m_skinToneP2 == i) {
                DrawCircleLines(static_cast<int>(center.x), static_cast<int>(center.y), 19, Theme::BorderGlow);
                DrawCircleLines(static_cast<int>(center.x), static_cast<int>(center.y), 20, Theme::BorderGlow);
            } else if (isHovered) {
                DrawCircleLines(static_cast<int>(center.x), static_cast<int>(center.y), 18, Theme::NeonPurple);
            }
        }

        // Live hand preview Player 2
        atlas.draw(0, m_skinToneP2, 1, true, Vector2{ p2Box.x + p2Box.width - 55.0f, p2Box.y + 175.0f }, 2.8f, 0.0f, WHITE);

        if (!m_chipsP2.empty()) {
            Theme::drawText("Perfis existentes:", static_cast<int>(p2Box.x + 20), static_cast<int>(p2Box.y + 215), 15, Theme::TextMuted);
            for (size_t c = 0; c < m_chipsP2.size(); ++c) {
                m_chipsP2[c].setBounds(Rectangle{p2Box.x + 20.0f + c * 82.0f, p2Box.y + 245.0f, 76.0f, 32.0f});
                m_chipsP2[c].draw();
            }
        }
    } else {
        // 1 Player / Flappy
        Theme::drawText("Nome do Jogador:", static_cast<int>(card.x + 80), static_cast<int>(card.y + 120), 18, Theme::TextBright);
        m_nameFieldP1.setBounds(Rectangle{card.x + 80, card.y + 150, card.width - 160, 48});
        m_nameFieldP1.draw();

        std::string skinLabel = "Tom de Pele: " + Theme::skinTones()[m_skinToneP1].name;
        Theme::drawText(skinLabel, static_cast<int>(card.x + 80), static_cast<int>(card.y + 220), 16, Theme::TextNormal);
        for (int i = 0; i < 4; ++i) {
            Vector2 center = { card.x + 120.0f + i * 54.0f, card.y + 265.0f };
            bool isHovered = CheckCollisionPointCircle(mouse, center, 22.0f);
            DrawCircle(static_cast<int>(center.x), static_cast<int>(center.y), 18, Theme::getSkinColor(i));
            if (m_skinToneP1 == i) {
                DrawCircleLines(static_cast<int>(center.x), static_cast<int>(center.y), 21, Theme::BorderGlow);
                DrawCircleLines(static_cast<int>(center.x), static_cast<int>(center.y), 22, Theme::BorderGlow);
            } else if (isHovered) {
                DrawCircleLines(static_cast<int>(center.x), static_cast<int>(center.y), 20, Theme::NeonPurple);
            }
        }

        // Live hand preview Player 1
        atlas.draw(0, m_skinToneP1, 0, false, Vector2{ card.x + card.width - 110.0f, card.y + 265.0f }, 3.2f, 0.0f, WHITE);

        if (!m_chipsP1.empty()) {
            Theme::drawText("Perfis existentes (clique para selecionar):", static_cast<int>(card.x + 80), static_cast<int>(card.y + 315), 15, Theme::TextMuted);
            for (size_t c = 0; c < m_chipsP1.size(); ++c) {
                m_chipsP1[c].setBounds(Rectangle{card.x + 80.0f + c * 86.0f, card.y + 345.0f, 80.0f, 34.0f});
                m_chipsP1[c].draw();
            }
        }
    }


    // Warning message (if any)
    if (!m_warningMessage.empty()) {
        int ww = Theme::measureText(m_warningMessage, 18);
        Theme::drawText(m_warningMessage, static_cast<int>(card.x + (card.width - ww) * 0.5f), static_cast<int>(card.y + card.height - 110), 18, Theme::ActionMenu);
    }

    // Confirm button centered at the bottom of the card
    m_btnConfirm.setBounds(Rectangle{card.x + (card.width - 320.0f) * 0.5f, card.y + card.height - 75.0f, 320.0f, 54.0f});
    m_btnConfirm.draw();
}

} // namespace Starlight
