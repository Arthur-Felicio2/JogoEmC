#include "Label.hpp"

namespace Starlight {

Label::Label(std::string text, Vector2 pos, int fontSize, Color color, TextAlign align)
    : m_text(std::move(text)), m_position(pos), m_fontSize(fontSize), m_color(color), m_align(align) {}

void Label::draw() {
    if (!m_visible || m_text.empty()) return;

    int textWidth = MeasureText(m_text.c_str(), m_fontSize);
    int drawX = (int)m_position.x;
    int drawY = (int)m_position.y;

    if (m_align == TextAlign::Center) {
        drawX -= textWidth / 2;
    } else if (m_align == TextAlign::Right) {
        drawX -= textWidth;
    }

    DrawText(m_text.c_str(), drawX, drawY, m_fontSize, m_color);
}

} // namespace Starlight
