#include "HumanPlayer.hpp"

namespace Starlight {

HumanPlayer::HumanPlayer(std::string name, int skinTone)
    : m_name(std::move(name)), m_skinTone(skinTone), m_selectedMove(0) {}

} // namespace Starlight
