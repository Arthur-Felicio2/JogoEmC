#pragma once
#include "IPlayer.hpp"
#include <random>

namespace Starlight {

class CpuPlayer : public IPlayer {
public:
    CpuPlayer(std::string name = "Computador", int skinTone = 1);

    const std::string& name() const override { return m_name; }
    void setName(const std::string& name) override { m_name = name; }
    bool isHuman() const override { return false; }

    int chooseMove(const IGameRules& rules) override;

    int skinTone() const override { return m_skinTone; }
    void setSkinTone(int tone) override { m_skinTone = tone; }

private:
    std::string m_name;
    int m_skinTone;
    std::mt19937 m_rng;
};

} // namespace Starlight
