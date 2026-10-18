#pragma once
#include <string>

namespace Starlight {

enum class Outcome {
    WinA,
    WinB,
    Draw
};

class IGameRules {
public:
    virtual ~IGameRules() = default;
    virtual int moveCount() const = 0;
    virtual Outcome resolve(int a, int b) const = 0;
    virtual std::string moveName(int move) const = 0;
    virtual std::string ruleName() const = 0;
};

} // namespace Starlight
