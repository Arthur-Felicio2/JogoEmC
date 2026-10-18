#include "CpuPlayer.hpp"
#include <chrono>

namespace Starlight {

CpuPlayer::CpuPlayer(std::string name, int skinTone)
    : m_name(std::move(name)), m_skinTone(skinTone) {
    auto seed = static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    m_rng.seed(seed);
}

int CpuPlayer::chooseMove(const IGameRules& rules) {
    int maxMoves = rules.moveCount();
    if (maxMoves <= 1) return 0;
    std::uniform_int_distribution<int> dist(0, maxMoves - 1);
    return dist(m_rng);
}

} // namespace Starlight
