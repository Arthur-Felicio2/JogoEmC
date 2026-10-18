#pragma once
#include "IPlayer.hpp"

namespace Starlight {

class HumanPlayer : public IPlayer {
public:
    HumanPlayer(std::string name = "Jogador", int skinTone = 0);

    const std::string& name() const override { return m_name; }
    void setName(const std::string& name) override { m_name = name; }
    bool isHuman() const override { return true; }

    void setSelectedMove(int move) { m_selectedMove = move; }
    int selectedMove() const { return m_selectedMove; }
    int chooseMove(const IGameRules& rules) override { return m_selectedMove; }

    int skinTone() const override { return m_skinTone; }
    void setSkinTone(int tone) override { m_skinTone = tone; }

private:
    std::string m_name;
    int m_skinTone;
    int m_selectedMove;
};

} // namespace Starlight
