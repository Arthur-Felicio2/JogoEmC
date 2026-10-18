#pragma once
#include "raylib.h"
#include <vector>

namespace Starlight {

class Layout {
public:
    static Rectangle center(const Rectangle& container, float width, float height) {
        return Rectangle{
            container.x + (container.width - width) * 0.5f,
            container.y + (container.height - height) * 0.5f,
            width,
            height
        };
    }

    static std::vector<Rectangle> distributeVertical(const Rectangle& container, int count, float itemHeight, float spacing) {
        std::vector<Rectangle> rects;
        rects.reserve(count);
        float totalH = count * itemHeight + (count - 1) * spacing;
        float startY = container.y + (container.height - totalH) * 0.5f;

        for (int i = 0; i < count; ++i) {
            rects.push_back(Rectangle{
                container.x + (container.width - container.width) * 0.5f,
                startY + i * (itemHeight + spacing),
                container.width,
                itemHeight
            });
        }
        return rects;
    }

    static std::vector<Rectangle> distributeHorizontal(const Rectangle& container, int count, float itemWidth, float spacing) {
        std::vector<Rectangle> rects;
        rects.reserve(count);
        float totalW = count * itemWidth + (count - 1) * spacing;
        float startX = container.x + (container.width - totalW) * 0.5f;

        for (int i = 0; i < count; ++i) {
            rects.push_back(Rectangle{
                startX + i * (itemWidth + spacing),
                container.y,
                itemWidth,
                container.height
            });
        }
        return rects;
    }
};

} // namespace Starlight
