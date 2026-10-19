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

    Rectangle card = UiLayout::centerCard(720.0f, 540.0f);

    float pW = 280.0f;
    float gap = 24.0f;
    float pX1 = card.x + (card.width - (pW * 2 + gap)) * 0.5f;
    float pX2 = pX1 + pW + gap;
    m_btn1Player.setBounds(Rectangle{pX1, card.y + 195.0f, pW, 56.0f});
    m_btn2Players.setBounds(Rectangle{pX2, card.y + 195.0f, pW, 56.0f});

    float cW = 140.0f;
    float cGap = 20.0f;
    float cX1 = card.x + (card.width - (cW * 2 + cGap)) * 0.5f;
    float cX2 = cX1 + cW + cGap;
    m_btnChoiceEven.setBounds(Rectangle{cX1, card.y + 355.0f, cW, 50.0f});
    m_btnChoiceOdd.setBounds(Rectangle{cX2, card.y + 355.0f, cW, 50.0f});

    m_btnContinue.setBounds(Rectangle{card.x + (card.width - 300.0f) * 0.5f, card.y + card.height - 75.0f, 300.0f, 56.0f});

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
    int sec1W = Theme::measureText("Quantidade de Jogadores:", 18);
    Theme::drawText("Quantidade de Jogadores:", static_cast<int>(card.x + (card.width - sec1W) * 0.5f), static_cast<int>(card.y + 155), 18, Theme::TextNormal);
    m_btn1Player.draw();
    m_btn2Players.draw();

    // Section 2: Par ou Ímpar choice
    if (m_gameType == GameType::OddEven) {
        int sec2W = Theme::measureText("Jogador 1 escolhe:", 18);
        Theme::drawText("Jogador 1 escolhe:", static_cast<int>(card.x + (card.width - sec2W) * 0.5f), static_cast<int>(card.y + 315), 18, Theme::TextNormal);
        m_btnChoiceEven.draw();
        m_btnChoiceOdd.draw();
    }

    m_btnContinue.draw();
}


} // namespace Starlight
