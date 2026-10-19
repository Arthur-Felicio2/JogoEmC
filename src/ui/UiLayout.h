#pragma once
#include <raylib.h>

namespace sl {

struct UiLayout {
    // Retorna um retângulo centralizado na tela lógica
    static Rectangle centerCard(float width, float height) {
        float sw = (float)GetScreenWidth();
        float sh = (float)GetScreenHeight();
        if (sw <= 0.0f) sw = 1280.0f;
        if (sh <= 0.0f) sh = 720.0f;
        return Rectangle{ (sw - width) * 0.5f, (sh - height) * 0.5f, width, height };
    }

    // Calcula retângulo de botão em uma lista vertical centralizada
    static Rectangle vButton(Rectangle container, int index, int totalButtons, float btnHeight, float gap) {
        float totalHeight = (totalButtons * btnHeight) + ((totalButtons - 1) * gap);
        float startY = container.y + (container.height - totalHeight) * 0.5f;
        float btnY = startY + index * (btnHeight + gap);
        float btnWidth = container.width * 0.85f;
        float btnX = container.x + (container.width - btnWidth) * 0.5f;

        return Rectangle{ btnX, btnY, btnWidth, btnHeight };
    }
};

} // namespace sl

namespace Starlight {
    using UiLayout = sl::UiLayout;
}
