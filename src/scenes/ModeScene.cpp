#include "ModeScene.hpp"
#include "NameEntryScene.hpp"
#include "MenuScene.hpp"
#include "core/SceneManager.hpp"
#include "ui/Theme.hpp"
#include "ui/UiLayout.h"

namespace Starlight {

ModeScene::ModeScene(GameType gameType)
    : m_gameType(gameType),
      m_btn1Player("1 Jogador (vs CPU)", Rectangle{340, 200, 280, 56}, ButtonStyle::Primary),
      m_btn2Players("2 Jogadores (Local)", Rectangle{660, 200, 280, 56}, ButtonStyle::Secondary),
      m_btnChoiceEven("Par", Rectangle{490, 360, 140, 50}, ButtonStyle::Primary),
      m_btnChoiceOdd("Ímpar", Rectangle{650, 360, 140, 50}, ButtonStyle::Secondary),
      m_btnContinue("Continuar ->", Rectangle{490, 480, 300, 56}, ButtonStyle::Primary),
      m_btnBack("<- Voltar ao Menu", Rectangle{40, 24, 160, 44}, ButtonStyle::Secondary) {

    m_btn1Player.setOnClick([this]() {
        m_isTwoPlayers = false;
        m_btn1Player.setStyle(ButtonStyle::Primary);
        m_btn2Players.setStyle(ButtonStyle::Secondary);
    });

    m_btn2Players.setOnClick([this]() {
        m_isTwoPlayers = true;
        m_btn1Player.setStyle(ButtonStyle::Secondary);
        m_btn2Players.setStyle(ButtonStyle::Primary);
    });

    m_btnChoiceEven.setOnClick([this]() {
        m_player1IsEven = true;
        m_btnChoiceEven.setStyle(ButtonStyle::Primary);
        m_btnChoiceOdd.setStyle(ButtonStyle::Secondary);
    });

    m_btnChoiceOdd.setOnClick([this]() {
        m_player1IsEven = false;
        m_btnChoiceEven.setStyle(ButtonStyle::Secondary);
        m_btnChoiceOdd.setStyle(ButtonStyle::Primary);
    });

    m_btnBack.setOnClick([this]() {
        if (m_manager) {
            m_manager->changeScene(std::make_unique<MenuScene>());
        }
    });

    m_btnContinue.setOnClick([this]() {
        if (!m_manager) return;
        m_manager->changeScene(std::make_unique<NameEntryScene>(m_gameType, m_isTwoPlayers, m_player1IsEven));
    });
}

void ModeScene::update(float dt) {
    m_animTimer += dt;
    m_btn1Player.update(dt);
    m_btn2Players.update(dt);

    if (m_gameType == GameType::OddEven) {
        m_btnChoiceEven.update(dt);
        m_btnChoiceOdd.update(dt);
    }

    m_btnContinue.update(dt);
    m_btnBack.update(dt);

    if (IsKeyPressed(KEY_ESCAPE)) {
        if (m_manager) {
            m_manager->changeScene(std::make_unique<MenuScene>());
        }
    }
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) {
        if (m_manager) {
            m_manager->changeScene(std::make_unique<NameEntryScene>(m_gameType, m_isTwoPlayers, m_player1IsEven));
        }
    }
}

void ModeScene::draw() {
    Theme::drawBackgroundGradient();
    m_btnBack.draw();

    Rectangle card = UiLayout::centerCard(720.0f, 540.0f);
    Theme::drawCard(card, 22.0f, true);

    std::string title = (m_gameType == GameType::OddEven) ? "CONFIGURAÇÃO — PAR OU ÍMPAR" : "CONFIGURAÇÃO — JOKENPÔ";
    int tw = Theme::measureText(title, 26);
    Theme::drawText(title, static_cast<int>(card.x + (card.width - tw) * 0.5f), static_cast<int>(card.y + 28), 26, Theme::NeonPurple);

    const char* sub = "Escolha o modo de jogo desejado:";
    int sw = Theme::measureText(sub, 18);
    Theme::drawText(sub, static_cast<int>(card.x + (card.width - sw) * 0.5f), static_cast<int>(card.y + 70), 18, Theme::TextMuted);

    // Section 1: 1P vs 2P
    Theme::drawText("Quantidade de Jogadores:", 340, 160, 18, Theme::TextNormal);
    m_btn1Player.draw();
    m_btn2Players.draw();

    // Section 2: Par ou Ímpar choice
    if (m_gameType == GameType::OddEven) {
        Theme::drawText("Jogador 1 escolhe:", 490, 320, 18, Theme::TextNormal);
        m_btnChoiceEven.draw();
        m_btnChoiceOdd.draw();
    }

    m_btnContinue.draw();
}

} // namespace Starlight
