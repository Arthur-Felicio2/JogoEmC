#include "Button.hpp"
#include "Theme.hpp"
#include "core/Input.hpp"
#include <algorithm>

namespace Starlight {

Button::Button(std::string text, Rectangle bounds, ButtonStyle style)
    : m_text(std::move(text)), m_style(style) {
    m_bounds = bounds;
}

void Button::update(float dt) {
    if (!m_visible || !m_enabled) {
        m_hovered = false;
        m_pressed = false;
        m_scaleAnim = 1.0f;
        m_hoverAnim = 0.0f;
        return;
    }

    Vector2 mouse = Input::getVirtualMousePosition();
    m_hovered = CheckCollisionPointRec(mouse, m_bounds);

    if (m_hovered) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            m_pressed = true;
        } else {
            if (m_pressed) { // Mouse released while still hovered
                m_pressed = false;
                if (m_onClick) m_onClick();
            }
        }
    } else {
        m_pressed = false;
    }

    // Keyboard navigation trigger
    if (m_focused && (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))) {
        if (m_onClick) m_onClick();
    }

    // Smooth animations
    float targetScale = m_pressed ? 0.97f : (m_hovered ? 1.02f : 1.0f);
    m_scaleAnim += (targetScale - m_scaleAnim) * (dt * 15.0f);

    float targetHover = m_hovered ? 1.0f : 0.0f;
    m_hoverAnim += (targetHover - m_hoverAnim) * (dt * 12.0f);
}

void Button::draw() {
    if (!m_visible) return;

    // Apply scale around center of button
    Vector2 center = {m_bounds.x + m_bounds.width * 0.5f, m_bounds.y + m_bounds.height * 0.5f};
    Rectangle drawRect = {
        center.x - (m_bounds.width * 0.5f) * m_scaleAnim,
        center.y - (m_bounds.height * 0.5f) * m_scaleAnim,
        m_bounds.width * m_scaleAnim,
        m_bounds.height * m_scaleAnim
    };

    // Determine colors
    Color bgColor;
    Color textColor = Theme::TEXT_PRIMARY;
    Color borderColor = Theme::CARD_BORDER;

    switch (m_style) {
        case ButtonStyle::Primary:
            bgColor = Theme::ACCENT_GOLD;
            textColor = Color{20, 30, 25, 255}; // Dark contrast
            borderColor = Color{255, 220, 120, 255};
            break;
        case ButtonStyle::Danger:
            bgColor = Theme::ALERT_DANGER;
            textColor = Color{255, 255, 255, 255};
            borderColor = Color{255, 120, 90, 255};
            break;
        case ButtonStyle::Ghost:
            bgColor = Color{255, 255, 255, (unsigned char)(20 + 30 * m_hoverAnim)};
            borderColor = Color{255, 255, 255, (unsigned char)(40 + 80 * m_hoverAnim)};
            break;
        case ButtonStyle::Secondary:
        default:
            bgColor = Color{
                (unsigned char)(255),
                (unsigned char)(255),
                (unsigned char)(255),
                (unsigned char)(24 + 36 * m_hoverAnim)
            };
            borderColor = Color{
                (unsigned char)(255),
                (unsigned char)(255),
                (unsigned char)(255),
                (unsigned char)(40 + 100 * m_hoverAnim)
            };
            break;
    }

    if (!m_enabled) {
        bgColor.a = (unsigned char)(bgColor.a * 0.5f);
        textColor.a = (unsigned char)(textColor.a * 0.5f);
        borderColor.a = (unsigned char)(borderColor.a * 0.5f);
    } else if (m_hovered && m_style == ButtonStyle::Primary) {
        // Brighten primary button slightly
        bgColor = Color{255, 200, 90, 255};
    }

    // Shadow
    if (m_enabled) {
        Rectangle shadowRect = {drawRect.x, drawRect.y + 4.0f * m_scaleAnim, drawRect.width, drawRect.height};
        DrawRectangleRounded(shadowRect, 14.0f / drawRect.height, 8, Color{0, 0, 0, 35});
    }

    // Body
    DrawRectangleRounded(drawRect, 14.0f / drawRect.height, 8, bgColor);
    DrawRectangleRoundedLines(drawRect, 14.0f / drawRect.height, 8, 2.0f, m_focused ? Theme::ACCENT_GOLD : borderColor);

    // Text rendering centered
    int fontSize = 20;
    int textWidth = MeasureText(m_text.c_str(), fontSize);
    int textX = (int)(center.x - textWidth * 0.5f);
    int textY = (int)(center.y - fontSize * 0.5f);

    DrawText(m_text.c_str(), textX, textY, fontSize, textColor);
}

} // namespace Starlight
