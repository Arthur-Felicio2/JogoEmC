#pragma once
#include "Widget.hpp"
#include <string>
#include <functional>

namespace Starlight {

enum class ButtonStyle {
    Primary,   // Purple Primary (#6C2BD9)
    Secondary, // Dark Card Surface (#1A1333)
    Danger,    // Action Red (#EF4444)
    Ghost,     // Transparent with subtle border
    Success    // Action Green (#10B981)
};

class Button : public Widget {
public:
    Button(std::string text = "Botão", Rectangle bounds = {0, 0, 220, 56}, ButtonStyle style = ButtonStyle::Secondary);

    void setText(const std::string& text) { m_text = text; }
    const std::string& text() const { return m_text; }

    void setStyle(ButtonStyle style) { m_style = style; }
    ButtonStyle style() const { return m_style; }

    void setOnClick(std::function<void()> onClick) { m_onClick = std::move(onClick); }

    void update(float dt) override;
    void draw() override;

    bool isHovered() const { return m_hovered; }
    bool isPressed() const { return m_pressed; }

private:
    std::string m_text;
    ButtonStyle m_style;
    std::function<void()> m_onClick;

    bool m_hovered = false;
    bool m_pressed = false;
    float m_scaleAnim = 1.0f;
    float m_hoverAnim = 0.0f;
};

} // namespace Starlight
