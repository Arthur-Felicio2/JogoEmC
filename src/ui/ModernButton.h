#pragma once
#include <raylib.h>
#include <string>
#include "Theme.hpp"

namespace sl {

class ModernButton {
public:
    Rectangle bounds;
    std::string text;
    Color baseColor;
    Color hoverColor;
    Color textColor;
    bool isPrimary = false;

    ModernButton(Rectangle b, const std::string& t, bool primary = false)
        : bounds(b), text(t), isPrimary(primary) {
        if (primary) {
            baseColor = Theme::Primary;
            hoverColor = Theme::PrimaryHover;
            textColor = Theme::TextBright;
        } else {
            baseColor = Theme::CardSurface;
            hoverColor = Theme::CardSurfaceHover;
            textColor = Theme::TextNormal;
        }
    }

    bool updateAndDraw(Font font) {
        Vector2 mouse = GetMousePosition();
        bool hovered = CheckCollisionPointRec(mouse, bounds);
        bool clicked = hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

        // Feedback de cor e sombra
        Color currentBg = hovered ? hoverColor : baseColor;
        Color borderCol = hovered ? Theme::BorderGlow : Theme::BorderSubtle;

        // Efeito de elevação / sombra no hover
        if (hovered) {
            DrawRectangleRounded({ bounds.x, bounds.y + 4, bounds.width, bounds.height }, 0.25f, 8, Theme::ShadowColor);
        }

        // Desenho do corpo do botão
        DrawRectangleRounded(bounds, 0.25f, 8, currentBg);
        DrawRectangleRoundedLines(bounds, 0.25f, 8, hovered ? 2.5f : 1.5f, borderCol);

        // Alinhamento centralizado do texto
        float fontSize = 22.0f;
        Vector2 textSize = MeasureTextEx(font, text.c_str(), fontSize, 1.0f);
        Vector2 textPos = {
            bounds.x + (bounds.width - textSize.x) * 0.5f,
            bounds.y + (bounds.height - textSize.y) * 0.5f
        };

        DrawTextEx(font, text.c_str(), textPos, fontSize, 1.0f, hovered ? Theme::TextBright : textColor);

        return clicked;
    }
};

} // namespace sl

namespace Starlight {
    using ModernButton = sl::ModernButton;
}
