#pragma once
#include "Widget.hpp"
#include <string>

namespace Starlight {

class TextField : public Widget {
public:
    TextField(std::string text = "", std::string placeholder = "Digite o nome...", Rectangle bounds = {0, 0, 260, 48}, int maxChars = 12);

    void setText(const std::string& text) { m_text = text; }
    const std::string& text() const { return m_text.empty() ? m_placeholder : m_text; }
    const std::string& rawText() const { return m_text; }

    void setPlaceholder(const std::string& ph) { m_placeholder = ph; }
    const std::string& placeholder() const { return m_placeholder; }

    void setActive(bool active) { m_active = active; }
    bool isActive() const { return m_active; }

    void update(float dt) override;
    void draw() override;

private:
    std::string m_text;
    std::string m_placeholder;
    int m_maxChars;
    bool m_active = false;
    float m_cursorTimer = 0.0f;
};

} // namespace Starlight
