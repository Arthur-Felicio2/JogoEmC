#pragma once
#include "core/Scene.hpp"
#include "ui/Button.hpp"

namespace Starlight {

class CreditsScene : public Scene {
public:
    CreditsScene();

    void update(float dt) override;
    void draw() override;

private:
    Button m_btnBack;
    float m_animTimer = 0.0f;
};

} // namespace Starlight
