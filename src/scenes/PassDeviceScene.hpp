#pragma once
#include "core/Scene.hpp"
#include "games/Match.hpp"
#include "ui/Button.hpp"
#include <memory>

namespace Starlight {

class PassDeviceScene : public Scene {
public:
    PassDeviceScene(std::shared_ptr<Match> match, int moveA);

    void update(float dt) override;
    void draw() override;

private:
    std::shared_ptr<Match> m_match;
    int m_moveA;
    Button m_btnReady;
    float m_animTimer = 0.0f;
};

} // namespace Starlight
