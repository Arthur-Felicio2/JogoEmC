#include "Panel.hpp"
#include "Theme.hpp"

namespace Starlight {

Panel::Panel(Rectangle bounds, std::string title, float radius)
    : m_title(std::move(title)), m_radius(radius) {
    m_bounds = bounds;
}

void Panel::draw() {
    if (!m_visible) return;

    Theme::drawCard(m_bounds, m_radius, m_shadow);

    if (!m_title.empty()) {
        int titleFontSize = 22;
        int titleWidth = Theme::measureText(m_title, titleFontSize);
        Theme::drawText(m_title,
                        static_cast<int>(m_bounds.x + (m_bounds.width - titleWidth) * 0.5f),
                        static_cast<int>(m_bounds.y + 18.0f),
                        titleFontSize,
                        Theme::NeonPurple);

        // Subtle divider
        DrawLine(static_cast<int>(m_bounds.x + 24.0f),
                 static_cast<int>(m_bounds.y + 50.0f),
                 static_cast<int>(m_bounds.x + m_bounds.width - 24.0f),
                 static_cast<int>(m_bounds.y + 50.0f),
                 Theme::BorderSubtle);
    }
}

} // namespace Starlight
