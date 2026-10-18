#pragma once
#include "Widget.hpp"
#include <string>

namespace Starlight {

class Panel : public Widget {
public:
    Panel(Rectangle bounds = {0, 0, 400, 300}, std::string title = "", float radius = 16.0f);

    void setTitle(const std::string& title) { m_title = title; }
    const std::string& title() const { return m_title; }

    void setRadius(float r) { m_radius = r; }
    void setShadow(bool shadow) { m_shadow = shadow; }

    void draw() override;

private:
    std::string m_title;
    float m_radius;
    bool m_shadow = true;
};

} // namespace Starlight
