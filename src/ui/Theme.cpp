#include "Theme.hpp"
#include "core/AssetManager.hpp"

namespace sl {

static const std::vector<SkinToneInfo> S_SKIN_TONES = {
    {"Claro", Color{252, 215, 190, 255}},       // #FCD7BE
    {"Médio Claro", Color{238, 192, 132, 255}},  // #EEC084
    {"Moreno", Color{205, 145, 90, 255}},        // #CD915A
    {"Escuro", Color{130, 78, 48, 255}}          // #824E30
};

static Font s_font = {0};
static bool s_fontLoaded = false;

const std::vector<SkinToneInfo>& Theme::skinTones() {
    return S_SKIN_TONES;
}

Color Theme::getSkinColor(int index) {
    if (index < 0 || index >= static_cast<int>(S_SKIN_TONES.size())) {
        return S_SKIN_TONES[0].color;
    }
    return S_SKIN_TONES[index].color;
}

void Theme::initFont() {
    if (s_fontLoaded) return;
    std::string fontPath = Starlight::AssetManager::resolvePath("assets/fonts/segoeui.ttf");
    if (FileExists(fontPath.c_str())) {
        // Carrega glifos Latin-1 (0x20 até 0xFF = 224 glifos)
        int codepoints[224];
        for (int i = 0; i < 224; ++i) {
            codepoints[i] = 32 + i;
        }
        s_font = LoadFontEx(fontPath.c_str(), 64, codepoints, 224);
        SetTextureFilter(s_font.texture, TEXTURE_FILTER_BILINEAR);
        s_fontLoaded = true;
    }
}

void Theme::unloadFont() {
    if (s_fontLoaded) {
        UnloadFont(s_font);
        s_fontLoaded = false;
    }
}

Font Theme::font() {
    if (!s_fontLoaded) return GetFontDefault();
    return s_font;
}

void Theme::drawText(const std::string& text, int posX, int posY, int fontSize, Color color) {
    if (s_fontLoaded) {
        DrawTextEx(s_font, text.c_str(), Vector2{static_cast<float>(posX), static_cast<float>(posY)}, static_cast<float>(fontSize), 1.0f, color);
    } else {
        DrawText(text.c_str(), posX, posY, fontSize, color);
    }
}

int Theme::measureText(const std::string& text, int fontSize) {
    if (s_fontLoaded) {
        return static_cast<int>(MeasureTextEx(s_font, text.c_str(), static_cast<float>(fontSize), 1.0f).x);
    } else {
        return MeasureText(text.c_str(), fontSize);
    }
}

void Theme::drawBackgroundGradient() {
    // Gradiente Vertical: BgDark (#0A0713) ao topo e BgDeepPurple (#150D2A) à base
    DrawRectangleGradientV(0, 0, 1280, 720, BgDark, BgDeepPurple);

    // Iluminação ambiental roxa/violeta radial no centro
    DrawCircleGradient(640, 320, 560, Color{108, 43, 217, 32}, Color{0, 0, 0, 0});
}

void Theme::drawCard(Rectangle rect, float radius, bool shadow, Color customBg) {
    if (shadow) {
        Rectangle shadowRect = {rect.x, rect.y + 6.0f, rect.width, rect.height};
        DrawRectangleRounded(shadowRect, radius / (rect.height > 0 ? rect.height : 1.0f), 8, ShadowColor);
    }

    float roundness = radius / (rect.height > 0 ? rect.height : 1.0f);
    DrawRectangleRounded(rect, roundness, 8, customBg);
    DrawRectangleRoundedLines(rect, roundness, 8, 1.5f, BorderSubtle);
}

void Theme::drawCardOutline(Rectangle rect, float radius, Color outlineColor, float thickness) {
    float roundness = radius / (rect.height > 0 ? rect.height : 1.0f);
    DrawRectangleRoundedLines(rect, roundness, 8, thickness, outlineColor);
}

} // namespace sl
