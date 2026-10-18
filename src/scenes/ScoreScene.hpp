#pragma once
#include "core/Scene.hpp"
#include "ui/Button.hpp"
#include "save/ScoreStore.h"
#include <string>
#include <vector>

namespace Starlight {

class ScoreScene : public Scene {
public:
    explicit ScoreScene(int initialTab = 0, std::string highlightedPlayer = "");

    void update(float dt) override;
    void draw() override;

private:
    void setTab(int tab);

    int m_currentTab = 0; // 0 = Geral, 1 = Par ou Ímpar, 2 = Jokenpô, 3 = Flappy
    std::string m_highlightedPlayer;

    Button m_btnTabGeral;
    Button m_btnTabOddEven;
    Button m_btnTabRps;
    Button m_btnTabFlappy;

    Button m_btnBack;

    float m_animTimer = 0.0f;
};

} // namespace Starlight
