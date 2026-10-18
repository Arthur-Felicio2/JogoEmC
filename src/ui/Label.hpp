#pragma once
#include "Widget.hpp"
#include <string>

namespace Starlight {

enum class TextAlign {
    Left,
    Center,
    Right
};

class Label : public Widget {
public:
    Label(std::string text = "", Vector2 pos = {0, 0}, int fontSize = 24, Color color = Color{245, 247, 242, 255}, TextAlign align = TextAlign::Left);

    void setText(const std::string& text) { m_text = text; }
    const std::string& text() const { return m_text; }

    void setFontSize(int size) { m_fontSize = size; }
    int fontSize() const { return m_fontSize; }

    void setColor(Color color) { m_color = color; }
    Color color() const { return m_color; }

    void setAlign(TextAlign align) { m_align = align; }
    TextAlign align() const { return m_align; }

    void setPosition(Vector2 pos) { m_position = pos; }
    Vector2 position() const { return m_position; }

    void draw() override;

private:
    std::string m_text;
    Vector2 m_position;
    int m_fontSize;
    Color m_color;
    TextAlign m_align;
};

} // namespace Starlight
