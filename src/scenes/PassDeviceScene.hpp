#pragma once
#include "core/Scene.hpp"
#include "games/Match.hpp"
#include "ui/Button.hpp"
#include "core/GameSession.hpp"
#include <memory>

namespace Starlight {

class PassDeviceScene : public Scene {
public:
    PassDeviceScene(std::shared_ptr<Match> match, int moveA, sl::GameSessionContext context = {});

    void update(float dt) override;
    void draw() override;

private:
    std::shared_ptr<Match> m_match;
    int m_moveA;
    sl::GameSessionContext m_context;
    Button m_btnReady;
    float m_animTimer = 0.0f;
};

} // namespace Starlight
