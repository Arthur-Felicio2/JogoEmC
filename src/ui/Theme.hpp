#pragma once
#include "raylib.h"
#include <vector>
#include <string>

namespace Starlight {

struct SkinToneInfo {
    std::string name;
    Color color;
};

class Theme {
public:
    // Core Colors
    static constexpr Color BG_TOP = Color{11, 61, 46, 255};       // #0B3D2E
    static constexpr Color BG_BOTTOM = Color{20, 90, 67, 255};    // #145A43

    static constexpr Color CARD_BG = Color{255, 255, 255, 20};     // 8% white
    static constexpr Color CARD_BORDER = Color{255, 255, 255, 34}; // #FFFFFF22
    static constexpr Color CARD_SHADOW = Color{0, 0, 0, 45};

    static constexpr Color ACCENT_GOLD = Color{244, 185, 66, 255}; // #F4B942
    static constexpr Color TEXT_PRIMARY = Color{245, 247, 242, 255}; // #F5F7F2
    static constexpr Color TEXT_SECONDARY = Color{184, 201, 191, 255}; // #B8C9BF
    static constexpr Color ALERT_DANGER = Color{228, 87, 46, 255};  // #E4572E
    static constexpr Color SUCCESS_GREEN = Color{52, 211, 153, 255}; // #34D399

    // Skin tones
    static const std::vector<SkinToneInfo>& skinTones();
    static Color getSkinColor(int index);

    // Font & Typography (Latin-1 0x20..0xFF)
    static void initFont();
    static void unloadFont();
    static Font font();
    static void drawText(const std::string& text, int posX, int posY, int fontSize, Color color);
    static int measureText(const std::string& text, int fontSize);

    // Drawing Helpers
    static void drawBackgroundGradient();
    static void drawCard(Rectangle rect, float radius = 16.0f, bool shadow = true, Color customBg = CARD_BG);
    static void drawCardOutline(Rectangle rect, float radius = 16.0f, Color outlineColor = CARD_BORDER, float thickness = 2.0f);
};

} // namespace Starlight
