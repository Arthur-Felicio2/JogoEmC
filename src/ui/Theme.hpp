#pragma once
#include "raylib.h"
#include <vector>
#include <string>

namespace sl {

struct Theme {
    // Fundos e Superfícies
    static constexpr Color BgDark          = { 10, 7, 19, 255 };      // #0A0713
    static constexpr Color BgDeepPurple    = { 21, 13, 42, 255 };     // #150D2A
    static constexpr Color CardSurface     = { 26, 19, 51, 235 };     // #1A1333 (com alpha)
    static constexpr Color CardSurfaceHover= { 37, 28, 69, 245 };     // #251C45
    static constexpr Color CardSurfaceDark = { 16, 11, 31, 240 };     // #100B1F

    // Bordas e Efeitos
    static constexpr Color BorderSubtle    = { 57, 38, 102, 255 };    // #392666
    static constexpr Color BorderGlow      = { 138, 63, 252, 255 };   // #8A3FFC
    static constexpr Color ShadowColor     = { 4, 2, 8, 160 };

    // Roxo Principal (Botões e Destaques)
    static constexpr Color Primary         = { 108, 43, 217, 255 };   // #6C2BD9
    static constexpr Color PrimaryHover    = { 139, 69, 255, 255 };   // #8B45FF
    static constexpr Color PrimaryPressed  = { 83, 28, 184, 255 };    // #531CB8
    static constexpr Color NeonPurple      = { 183, 132, 255, 255 };  // #B784FF

    // Tipografia
    static constexpr Color TextBright      = { 255, 255, 255, 255 };  // #FFFFFF
    static constexpr Color TextNormal      = { 225, 219, 244, 255 };  // #E1DBF4
    static constexpr Color TextMuted       = { 155, 142, 184, 255 };  // #9B8EB8

    // Semânticas e Ações
    static constexpr Color ActionPlayAgain = { 16, 185, 129, 255 };   // #10B981 (Verde esmeralda harmônico)
    static constexpr Color ActionMenu      = { 239, 68, 68, 255 };    // #EF4444 (Vermelho rubi)
    static constexpr Color TrophyGold      = { 245, 158, 11, 255 };   // #F59E0B

    // Aliases para retrocompatibilidade
    static constexpr Color BG_TOP          = BgDark;
    static constexpr Color BG_BOTTOM       = BgDeepPurple;
    static constexpr Color CARD_BG         = CardSurface;
    static constexpr Color CARD_BORDER     = BorderSubtle;
    static constexpr Color CARD_SHADOW     = ShadowColor;
    static constexpr Color ACCENT_GOLD     = TrophyGold;
    static constexpr Color TEXT_PRIMARY    = TextBright;
    static constexpr Color TEXT_SECONDARY  = TextNormal;
    static constexpr Color ALERT_DANGER    = ActionMenu;
    static constexpr Color SUCCESS_GREEN   = ActionPlayAgain;

    // Skin tones
    static const std::vector<struct SkinToneInfo>& skinTones();
    static Color getSkinColor(int index);

    // Font & Typography (Latin-1 0x20..0xFF)
    static void initFont();
    static void unloadFont();
    static Font font();
    static void drawText(const std::string& text, int posX, int posY, int fontSize, Color color);
    static int measureText(const std::string& text, int fontSize);

    // Drawing Helpers
    static void drawBackgroundGradient();
    static void drawCard(Rectangle rect, float radius = 16.0f, bool shadow = true, Color customBg = CardSurface);
    static void drawCardOutline(Rectangle rect, float radius = 16.0f, Color outlineColor = BorderSubtle, float thickness = 2.0f);
};

struct SkinToneInfo {
    std::string name;
    Color color;
};

} // namespace sl

namespace Starlight {
    using Theme = sl::Theme;
    using SkinToneInfo = sl::SkinToneInfo;
}
