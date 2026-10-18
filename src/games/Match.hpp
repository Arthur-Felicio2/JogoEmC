#pragma once
#include <memory>
#include <vector>
#include "IGameRules.hpp"
#include "IPlayer.hpp"

namespace Starlight {

struct RoundResult {
    int roundNumber;
    int moveA;
    int moveB;
    Outcome outcome;
};

class Match {
public:
    Match(std::shared_ptr<IPlayer> playerA,
          std::shared_ptr<IPlayer> playerB,
          std::shared_ptr<IGameRules> rules,
          int targetWins = 2);

    void reset();

    Outcome playRound(int moveA, int moveB);

    bool isOver() const;
    int winner() const; // 0 for Player A, 1 for Player B, -1 if unfinished

    int scoreA() const { return m_scoreA; }
    int scoreB() const { return m_scoreB; }
    int roundNumber() const { return m_roundNumber; }
    int targetWins() const { return m_targetWins; }

    std::shared_ptr<IPlayer> playerA() const { return m_playerA; }
    std::shared_ptr<IPlayer> playerB() const { return m_playerB; }
    std::shared_ptr<IGameRules> rules() const { return m_rules; }

    const std::vector<RoundResult>& history() const { return m_history; }
    const RoundResult* lastRound() const {
        if (m_history.empty()) return nullptr;
        return &m_history.back();
    }

private:
    std::shared_ptr<IPlayer> m_playerA;
    std::shared_ptr<IPlayer> m_playerB;
    std::shared_ptr<IGameRules> m_rules;
    int m_targetWins;
    int m_scoreA;
    int m_scoreB;
    int m_roundNumber;
    std::vector<RoundResult> m_history;
};

} // namespace Starlight
