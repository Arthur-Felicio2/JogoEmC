#pragma once
#include "IGameRules.hpp"

namespace Starlight {

enum RpsMove {
    Rock = 0,     // Pedra (pose 0)
    Paper = 1,    // Papel (pose 5)
    Scissors = 2  // Tesoura (pose 2)
};

class RpsRules : public IGameRules {
public:
    int moveCount() const override { return 3; }
    Outcome resolve(int a, int b) const override;
    std::string moveName(int move) const override;
    std::string ruleName() const override { return "Jokenpô"; }

    // Map RPS logical move (0, 1, 2) to Hand Pose index (0..5)
    static int toPoseIndex(int move) {
        switch (move) {
            case Rock:     return 0; // pose 0 (fechada)
            case Paper:    return 5; // pose 5 (aberta)
            case Scissors: return 2; // pose 2 (tesoura / V)
            default:       return 0;
        }
    }
};

} // namespace Starlight
