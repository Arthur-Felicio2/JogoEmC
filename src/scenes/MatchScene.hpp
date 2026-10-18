#pragma once
#include "core/Scene.hpp"
#include "games/Match.hpp"
#include "ui/Button.hpp"
#include "render/ScoreBoard.hpp"
#include "fx/ParticleSystem.hpp"
#include "fx/Timeline.hpp"
#include <memory>
#include <vector>

namespace Starlight {

enum class MatchPhase {
    PlayerAChoice,
    PassDevicePrompt,
    PlayerBChoice,
    RevealAnimation
};

class MatchScene : public Scene {
public:
    explicit MatchScene(std::shared_ptr<Match> match);

    void onEnter() override;
    void update(float dt) override;
    void draw() override;

    void setPlayer1Move(int move);

private:
    void setupTurnUI();
    void startReveal(int moveA, int moveB);
    void finishRound();

    std::shared_ptr<Match> m_match;
    MatchPhase m_phase = MatchPhase::PlayerAChoice;

    int m_moveA = 0;
    int m_moveB = 0;
    Outcome m_roundOutcome = Outcome::Draw;

    ScoreBoard m_scoreBoard;
    ParticleSystem m_particles;
    Button m_btnMenu;

    // Move Selection Buttons
    std::vector<Button> m_moveButtons;

    // Pass Device Button
    Button m_btnReadyToPlay;

    // Animation Timing & States (PLAN.md 5.4)
    float m_revealTimer = 0.0f;
    float m_hand1Offset = -500.0f;
    float m_hand2Offset = 500.0f;
    float m_handScalePunch = 1.0f;
    float m_handRotationA = 0.0f;
    float m_handRotationB = 0.0f;

    std::string m_countdownText;
    std::string m_sumText;
    std::string m_outcomeText;
    Color m_outcomeColor = Color{245, 247, 242, 255};
    float m_outcomeAlpha = 0.0f;

    bool m_pointAwarded = false;
};

} // namespace Starlight
