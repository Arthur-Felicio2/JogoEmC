#pragma once
#include "core/Scene.hpp"

namespace Starlight {

class SplashScene : public Scene {
public:
    SplashScene();

    void update(float dt) override;
    void draw() override;

private:
    float m_timer = 0.0f;
    float m_pulse = 0.0f;
};

} // namespace Starlight
