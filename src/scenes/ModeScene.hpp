#pragma once
#include "core/Scene.hpp"
#include "ui/Button.hpp"
#include "MenuScene.hpp"

namespace Starlight {

class ModeScene : public Scene {
public:
    explicit ModeScene(GameType gameType);

    void update(float dt) override;
    void draw() override;

private:
    GameType m_gameType;
    bool m_isTwoPlayers = false;
    bool m_player1IsEven = true; // Para Par ou Ímpar

    // UI Buttons
    Button m_btn1Player;
    Button m_btn2Players;

    Button m_btnChoiceEven;
    Button m_btnChoiceOdd;

    Button m_btnContinue;
    Button m_btnBack;

    float m_animTimer = 0.0f;
};

} // namespace Starlight
