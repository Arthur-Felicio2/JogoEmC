#pragma once
#include "core/Scene.hpp"
#include "ui/Button.hpp"
#include <vector>

namespace Starlight {

enum class GameType {
    OddEven,
    Rps,
    Flappy
};

class MenuScene : public Scene {
public:
    MenuScene();

    void update(float dt) override;
    void draw() override;

private:
    Button m_btnOddEven;
    Button m_btnRps;
    Button m_btnFlappy;
    Button m_btnScores;
    Button m_btnCredits;
    Button m_btnExit;

    std::vector<Button*> m_buttons;
    int m_focusedIndex = 0;

    float m_animTimer = 0.0f;
};

} // namespace Starlight
