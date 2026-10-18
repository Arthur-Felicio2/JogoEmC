#pragma once
#include "core/Scene.hpp"
#include "games/Match.hpp"
#include "ui/Button.hpp"
#include "render/Trophy.hpp"
#include "fx/ParticleSystem.hpp"
#include <memory>
#include <string>

namespace Starlight {

class ResultScene : public Scene {
public:
    explicit ResultScene(std::shared_ptr<Match> match);

    void onEnter() override;
    void update(float dt) override;
    void draw() override;

private:
    std::shared_ptr<Match> m_match;
    Trophy m_trophy;
    ParticleSystem m_particles;

    Button m_btnRematch;
    Button m_btnChangePlayers;
    Button m_btnScores;
    Button m_btnMenu;

    float m_animTimer = 0.0f;
    std::string m_winnerName;
    int m_winnerIndex = 0;
    bool m_alreadyRecorded = false;
};

} // namespace Starlight
