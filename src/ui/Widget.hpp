#pragma once
#include "raylib.h"

namespace Starlight {

class Widget {
public:
    virtual ~Widget() = default;
    virtual void update(float dt) {}
    virtual void draw() = 0;

    void setBounds(const Rectangle& bounds) { m_bounds = bounds; }
    Rectangle bounds() const { return m_bounds; }

    void setVisible(bool visible) { m_visible = visible; }
    bool isVisible() const { return m_visible; }

    void setEnabled(bool enabled) { m_enabled = enabled; }
    bool isEnabled() const { return m_enabled; }

    void setFocused(bool focused) { m_focused = focused; }
    bool isFocused() const { return m_focused; }

protected:
    Rectangle m_bounds = {0, 0, 100, 50};
    bool m_visible = true;
    bool m_enabled = true;
    bool m_focused = false;
};

} // namespace Starlight
