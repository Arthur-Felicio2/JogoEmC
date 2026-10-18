#include "Match.hpp"

namespace Starlight {

Match::Match(std::shared_ptr<IPlayer> playerA,
             std::shared_ptr<IPlayer> playerB,
             std::shared_ptr<IGameRules> rules,
             int targetWins)
    : m_playerA(std::move(playerA)),
      m_playerB(std::move(playerB)),
      m_rules(std::move(rules)),
      m_targetWins(targetWins),
      m_scoreA(0),
      m_scoreB(0),
      m_roundNumber(1) {}

void Match::reset() {
    m_scoreA = 0;
    m_scoreB = 0;
    m_roundNumber = 1;
    m_history.clear();
}

Outcome Match::playRound(int moveA, int moveB) {
    if (isOver()) {
        return Outcome::Draw;
    }

    Outcome result = m_rules->resolve(moveA, moveB);

    RoundResult record;
    record.roundNumber = m_roundNumber;
    record.moveA = moveA;
    record.moveB = moveB;
    record.outcome = result;
    m_history.push_back(record);

    if (result == Outcome::WinA) {
        m_scoreA++;
        m_roundNumber++;
    } else if (result == Outcome::WinB) {
        m_scoreB++;
        m_roundNumber++;
    } else {
        // Empate: no Jokenpo, repete a rodada e não conta no placar
    }

    return result;
}

bool Match::isOver() const {
    return (m_scoreA >= m_targetWins || m_scoreB >= m_targetWins);
}

int Match::winner() const {
    if (m_scoreA >= m_targetWins) return 0;
    if (m_scoreB >= m_targetWins) return 1;
    return -1;
}

} // namespace Starlight
