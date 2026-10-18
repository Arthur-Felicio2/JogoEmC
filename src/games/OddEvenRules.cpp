#include "OddEvenRules.hpp"

namespace Starlight {

OddEvenRules::OddEvenRules(bool playerAIsEven)
    : m_playerAIsEven(playerAIsEven) {}

Outcome OddEvenRules::resolve(int a, int b) const {
    int sum = a + b;
    bool sumIsEven = ((sum % 2) == 0);
    if (sumIsEven == m_playerAIsEven) {
        return Outcome::WinA;
    }
    return Outcome::WinB;
}

std::string OddEvenRules::moveName(int move) const {
    return std::to_string(move);
}

} // namespace Starlight
