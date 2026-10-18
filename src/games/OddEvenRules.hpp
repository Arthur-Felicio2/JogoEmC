#pragma once
#include "IGameRules.hpp"

namespace Starlight {

class OddEvenRules : public IGameRules {
public:
    explicit OddEvenRules(bool playerAIsEven = true);

    void setPlayerAIsEven(bool isEven) { m_playerAIsEven = isEven; }
    bool isPlayerAIsEven() const { return m_playerAIsEven; }

    int moveCount() const override { return 6; } // 0..5 dedos
    Outcome resolve(int a, int b) const override;
    std::string moveName(int move) const override;
    std::string ruleName() const override { return "Par ou Ímpar"; }

private:
    bool m_playerAIsEven;
};

} // namespace Starlight
