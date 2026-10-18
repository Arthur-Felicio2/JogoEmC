#pragma once
#include <string>
#include "IGameRules.hpp"

namespace Starlight {

class IPlayer {
public:
    virtual ~IPlayer() = default;
    virtual const std::string& name() const = 0;
    virtual void setName(const std::string& name) = 0;
    virtual bool isHuman() const = 0;
    virtual int chooseMove(const IGameRules& rules) = 0;
    virtual int skinTone() const = 0;
    virtual void setSkinTone(int tone) = 0;
};

} // namespace Starlight
