#pragma once
#include "core/Scene.hpp"
#include "games/Match.hpp"
#include "ui/Button.hpp"
#include "render/Trophy.hpp"
#include "fx/ParticleSystem.hpp"
#include "core/GameSession.hpp"
#include "save/ScoreStore.h"
#include <memory>
#include <string>

namespace Starlight {

class ResultScene : public Scene {
public:
    explicit ResultScene(std::shared_ptr<Match> match);
    ResultScene(sl::GameSessionContext ctx, const std::string& winner, int scoreP1, int scoreP2, sl::ScoreStore& store);

    void onEnter() override;
    void update(float dt) override;
    void draw() override;

    void onPlayAgain();
    void onBackToMenu();

    void setMatch(std::shared_ptr<Match> match) { m_match = std::move(match); }
    const sl::GameSessionContext& sessionContext() const { return m_context; }

private:
    void setupButtonCallbacks();

    sl::GameSessionContext m_context;
    std::string m_winnerName;
    int m_scoreP1 = 0;
    int m_scoreP2 = 0;
    bool m_scoreRecorded = false;
    sl::ScoreStore& m_store;

    std::shared_ptr<Match> m_match;
    Trophy m_trophy;
    ParticleSystem m_particles;

    // Decision Modal Buttons
    Button m_btnPlayAgain;      // [ SIM - JOGAR NOVAMENTE ]
    Button m_btnBackToMenu;      // [ NÃO - VOLTAR AO MENU ]
    Button m_btnScores;          // [ 🏆 Ver Placar ]
    Button m_btnChangePlayers;   // [ 👥 Trocar Nomes ]

    float m_animTimer = 0.0f;
    int m_winnerIndex = 0;
};

} // namespace Starlight
