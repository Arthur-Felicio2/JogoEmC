#include "RpsRules.hpp"

namespace Starlight {

Outcome RpsRules::resolve(int a, int b) const {
    if (a == b) {
        return Outcome::Draw;
    }

    // 0 beats 2, 2 beats 1, 1 beats 0
    if ((a == Rock && b == Scissors) ||
        (a == Scissors && b == Paper) ||
        (a == Paper && b == Rock)) {
        return Outcome::WinA;
    }

    return Outcome::WinB;
}

std::string RpsRules::moveName(int move) const {
    switch (move) {
        case Rock:     return "Pedra";
        case Paper:    return "Papel";
        case Scissors: return "Tesoura";
        default:       return "Desconhecido";
    }
}

} // namespace Starlight
