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
        int titleWidth = MeasureText(m_title.c_str(), titleFontSize);
        DrawText(m_title.c_str(),
                 (int)(m_bounds.x + (m_bounds.width - titleWidth) * 0.5f),
                 (int)(m_bounds.y + 18.0f),
                 titleFontSize,
                 Theme::ACCENT_GOLD);

        // Subtle divider
        DrawLine((int)(m_bounds.x + 24.0f),
                 (int)(m_bounds.y + 50.0f),
                 (int)(m_bounds.x + m_bounds.width - 24.0f),
                 (int)(m_bounds.y + 50.0f),
                 Color{255, 255, 255, 30});
    }
}

} // namespace Starlight
