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
    if (m_focused && (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER) || IsKeyPressed(KEY_SPACE))) {
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

    // Determine colors based on Dark Violet Theme
    Color bgColor;
    Color textColor = Theme::TextBright;
    Color borderColor = Theme::BorderSubtle;

    switch (m_style) {
        case ButtonStyle::Primary:
            if (m_pressed) {
                bgColor = Theme::PrimaryPressed;
            } else if (m_hovered) {
                bgColor = Theme::PrimaryHover;
            } else {
                bgColor = Theme::Primary;
            }
            textColor = Theme::TextBright;
            borderColor = m_hovered ? Theme::NeonPurple : Theme::BorderGlow;
            break;

        case ButtonStyle::Success:
            bgColor = m_hovered ? Color{22, 210, 145, 255} : Theme::ActionPlayAgain;
            textColor = Theme::TextBright;
            borderColor = m_hovered ? Color{110, 245, 185, 255} : Color{50, 220, 155, 255};
            break;

        case ButtonStyle::Danger:
            bgColor = m_hovered ? Color{248, 85, 85, 255} : Theme::ActionMenu;
            textColor = Theme::TextBright;
            borderColor = m_hovered ? Color{255, 140, 140, 255} : Color{245, 95, 95, 255};
            break;

        case ButtonStyle::Ghost:
            bgColor = Color{26, 19, 51, static_cast<unsigned char>(40 + 70 * m_hoverAnim)};
            textColor = m_hovered ? Theme::TextBright : Theme::TextNormal;
            borderColor = m_hovered ? Theme::BorderGlow : Theme::BorderSubtle;
            break;

        case ButtonStyle::Secondary:
        default:
            bgColor = m_hovered ? Theme::CardSurfaceHover : Theme::CardSurface;
            textColor = m_hovered ? Theme::TextBright : Theme::TextNormal;
            borderColor = m_hovered ? Theme::BorderGlow : Theme::BorderSubtle;
            break;
    }

    if (!m_enabled) {
        bgColor.a = static_cast<unsigned char>(bgColor.a * 0.45f);
        textColor.a = static_cast<unsigned char>(textColor.a * 0.45f);
        borderColor.a = static_cast<unsigned char>(borderColor.a * 0.45f);
    }

    // Shadow on hover / active
    if (m_enabled) {
        float shadowOffset = (m_hovered ? 6.0f : 3.0f) * m_scaleAnim;
        Rectangle shadowRect = {drawRect.x, drawRect.y + shadowOffset, drawRect.width, drawRect.height};
        DrawRectangleRounded(shadowRect, 14.0f / drawRect.height, 8, Theme::ShadowColor);
    }

    // Body
    DrawRectangleRounded(drawRect, 14.0f / drawRect.height, 8, bgColor);
    DrawRectangleRoundedLines(drawRect, 14.0f / drawRect.height, 8, (m_focused || m_hovered) ? 2.5f : 1.5f,
                              m_focused ? Theme::BorderGlow : borderColor);

    // Text rendering centered using Theme font
    int fontSize = (drawRect.height >= 52.0f) ? 20 : 18;
    int textWidth = Theme::measureText(m_text, fontSize);
    int textX = static_cast<int>(center.x - textWidth * 0.5f);
    int textY = static_cast<int>(center.y - fontSize * 0.5f);

    Theme::drawText(m_text, textX, textY, fontSize, textColor);
}

} // namespace Starlight
