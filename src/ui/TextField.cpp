#include "TextField.hpp"
#include "Theme.hpp"
#include "core/Input.hpp"

namespace Starlight {

static size_t countUtf8Characters(const std::string& str) {
    size_t count = 0;
    for (size_t i = 0; i < str.size(); ++i) {
        // Increment for single-byte or multi-byte leading bytes (not continuation bytes 0x80..0xBF)
        if ((static_cast<unsigned char>(str[i]) & 0xC0) != 0x80) {
            count++;
        }
    }
    return count;
}

static void popUtf8Character(std::string& str) {
    if (str.empty()) return;
    // Remove continuation bytes if any
    while (!str.empty() && (static_cast<unsigned char>(str.back()) & 0xC0) == 0x80) {
        str.pop_back();
    }
    // Remove leading byte
    if (!str.empty()) {
        str.pop_back();
    }
}

TextField::TextField(std::string text, std::string placeholder, Rectangle bounds, int maxChars)
    : m_text(std::move(text)), m_placeholder(std::move(placeholder)), m_maxChars(maxChars) {
    m_bounds = bounds;
}

void TextField::update(float dt) {
    if (!m_visible || !m_enabled) return;

    Vector2 mouse = Input::getVirtualMousePosition();
    bool hovered = CheckCollisionPointRec(mouse, m_bounds);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        m_active = hovered;
    }

    if (m_focused && IsKeyPressed(KEY_TAB)) {
        m_active = !m_active;
    }

    if (m_active) {
        m_cursorTimer += dt;
        int key = GetCharPressed();
        while (key > 0) {
            // Aceita caracteres imprimíveis ASCII e Latin-1 estendido (0x20 a 0xFF)
            if ((key >= 32 && key <= 255) && countUtf8Characters(m_text) < static_cast<size_t>(m_maxChars)) {
                if (key <= 0x7F) {
                    m_text.push_back(static_cast<char>(key));
                } else {
                    // Codifica caractere Latin-1 em UTF-8 de 2 bytes
                    m_text.push_back(static_cast<char>(0xC0 | ((key >> 6) & 0x1F)));
                    m_text.push_back(static_cast<char>(0x80 | (key & 0x3F)));
                }
            }
            key = GetCharPressed();
        }

        // Backspace remove exatamente um caractere UTF-8
        if (IsKeyPressed(KEY_BACKSPACE)) {
            popUtf8Character(m_text);
        }
    } else {
        m_cursorTimer = 0.0f;
    }
}

void TextField::draw() {
    if (!m_visible) return;

    Color bgColor = Color{255, 255, 255, static_cast<unsigned char>(m_active ? 30 : 15)};
    Color borderColor = m_active ? Theme::ACCENT_GOLD : Theme::CARD_BORDER;

    DrawRectangleRounded(m_bounds, 10.0f / m_bounds.height, 6, bgColor);
    DrawRectangleRoundedLines(m_bounds, 10.0f / m_bounds.height, 6, m_active ? 2.0f : 1.5f, borderColor);

    int fontSize = 20;
    int drawY = static_cast<int>(m_bounds.y + (m_bounds.height - fontSize) * 0.5f);
    int drawX = static_cast<int>(m_bounds.x + 14.0f);

    if (m_text.empty()) {
        Theme::drawText(m_placeholder, drawX, drawY, fontSize, Theme::TEXT_SECONDARY);
    } else {
        Theme::drawText(m_text, drawX, drawY, fontSize, Theme::TEXT_PRIMARY);
    }

    // Cursor
    if (m_active) {
        bool showCursor = (static_cast<int>(m_cursorTimer * 2.0f) % 2) == 0;
        if (showCursor) {
            int textW = Theme::measureText(m_text, fontSize);
            DrawRectangle(drawX + textW + 2, drawY + 2, 2, fontSize - 4, Theme::ACCENT_GOLD);
        }
    }
}

} // namespace Starlight
