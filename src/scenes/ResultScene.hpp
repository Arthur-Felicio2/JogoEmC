#pragma once
#include "core/Scene.hpp"
#include "games/Match.hpp"
#include "ui/Button.hpp"
#include "render/Trophy.hpp"
#include "fx/ParticleSystem.hpp"
#include "core/GameSession.hpp"
#include <memory>
#include <string>

namespace Starlight {

class ResultScene : public Scene {
public:
    explicit ResultScene(std::shared_ptr<Match> match);

    void onEnter() override;
    void update(float dt) override;
    void draw() override;

    void onPlayAgain();
    void onBackToMenu();

private:
    std::shared_ptr<Match> m_match;
    Trophy m_trophy;
    ParticleSystem m_particles;

    // Decision Modal Buttons
    Button m_btnPlayAgain;      // [ SIM - JOGAR NOVAMENTE ]
    Button m_btnBackToMenu;      // [ NÃO - VOLTAR AO MENU ]
    Button m_btnScores;          // [ 🏆 Ver Placar ]
    Button m_btnChangePlayers;   // [ 👥 Trocar Nomes ]

    float m_animTimer = 0.0f;
    std::string m_winnerName;
    int m_winnerIndex = 0;
    bool m_scoreRecorded = false;
};

} // namespace Starlight
