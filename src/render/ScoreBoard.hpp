#pragma once
#include "raylib.h"
#include <string>

namespace Starlight {

class ScoreBoard {
public:
    ScoreBoard();

    void setPlayerA(const std::string& name, int score, Color skinColor);
    void setPlayerB(const std::string& name, int score, Color skinColor);
    void setRound(int roundNumber, int targetWins, const std::string& gameTitle);

    void draw() const;

private:
    std::string m_nameA;
    std::string m_nameB;
    int m_scoreA = 0;
    int m_scoreB = 0;
    Color m_skinColorA = Color{250, 217, 192, 255};
    Color m_skinColorB = Color{241, 194, 125, 255};
    int m_roundNumber = 1;
    int m_targetWins = 2;
    std::string m_gameTitle = "Par ou Ímpar";
};

} // namespace Starlight
