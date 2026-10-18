#pragma once
// HandAtlasRaylib.h — Renderizador de mãos pixel art com pivô no pulso e filtro ponto
// Starlight Company

#include "HandSprites.h"
#include "raylib.h"
#include <algorithm>

namespace sl {

class HandAtlas {
public:
    HandAtlas() {
        for (int p = 0; p < POSE_COUNT; ++p) {
            for (int s = 0; s < SKIN_COUNT; ++s) {
                for (int sl = 0; sl < SLEEVE_COUNT; ++sl) {
                    m_loaded[p][s][sl] = false;
                }
            }
        }
    }

    ~HandAtlas() {
        unloadAll();
    }

    // Pré-carrega todas as texturas se desejado após InitWindow()
    void init() {
        for (int p = 0; p < POSE_COUNT; ++p) {
            for (int s = 0; s < SKIN_COUNT; ++s) {
                for (int sl = 0; sl < SLEEVE_COUNT; ++sl) {
                    getTexture(p, s, sl);
                }
            }
        }
    }

    void unloadAll() {
        for (int p = 0; p < POSE_COUNT; ++p) {
            for (int s = 0; s < SKIN_COUNT; ++s) {
                for (int sl = 0; sl < SLEEVE_COUNT; ++sl) {
                    if (m_loaded[p][s][sl]) {
                        UnloadTexture(m_textures[p][s][sl]);
                        m_loaded[p][s][sl] = false;
                    }
                }
            }
        }
    }

    Texture2D getTexture(int pose, int skin, int sleeve) {
        pose = std::clamp(pose, 0, POSE_COUNT - 1);
        skin = std::clamp(skin, 0, SKIN_COUNT - 1);
        sleeve = std::clamp(sleeve, 0, SLEEVE_COUNT - 1);

        if (!m_loaded[pose][skin][sleeve]) {
            Image img = GenImageColor(HAND_WIDTH, HAND_HEIGHT, BLANK);
            Color* pixels = reinterpret_cast<Color*>(img.data);
            const uint8_t* sprite = kPosePixels[pose];
            const SkinPalette& sp = kSkins[skin];
            const SleevePalette& slp = kSleeves[sleeve];

            for (int i = 0; i < HAND_WIDTH * HAND_HEIGHT; ++i) {
                uint8_t val = sprite[i];
                switch (val) {
                    case 1: pixels[i] = Color{sp.outline.r, sp.outline.g, sp.outline.b, sp.outline.a}; break;
                    case 2: pixels[i] = Color{sp.shadow.r, sp.shadow.g, sp.shadow.b, sp.shadow.a}; break;
                    case 3: pixels[i] = Color{sp.base.r, sp.base.g, sp.base.b, sp.base.a}; break;
                    case 4: pixels[i] = Color{sp.highlight.r, sp.highlight.g, sp.highlight.b, sp.highlight.a}; break;
                    case 5: pixels[i] = Color{slp.outline.r, slp.outline.g, slp.outline.b, slp.outline.a}; break;
                    case 6: pixels[i] = Color{slp.shadow.r, slp.shadow.g, slp.shadow.b, slp.shadow.a}; break;
                    case 7: pixels[i] = Color{slp.base.r, slp.base.g, slp.base.b, slp.base.a}; break;
                    case 8: pixels[i] = Color{slp.highlight.r, slp.highlight.g, slp.highlight.b, slp.highlight.a}; break;
                    default: pixels[i] = BLANK; break;
                }
            }

            m_textures[pose][skin][sleeve] = LoadTextureFromImage(img);
            SetTextureFilter(m_textures[pose][skin][sleeve], TEXTURE_FILTER_POINT);
            UnloadImage(img);
            m_loaded[pose][skin][sleeve] = true;
        }

        return m_textures[pose][skin][sleeve];
    }

    void draw(int pose,
              int skin,
              int sleeve,
              bool mirror,
              Vector2 wristPos,
              float scale = 5.0f,
              float rotationDeg = 0.0f,
              Color tint = WHITE) {
        Texture2D tex = getTexture(pose, skin, sleeve);

        Rectangle srcRec = {
            0.0f,
            0.0f,
            mirror ? -static_cast<float>(HAND_WIDTH) : static_cast<float>(HAND_WIDTH),
            static_cast<float>(HAND_HEIGHT)
        };

        Rectangle destRec = {
            wristPos.x,
            wristPos.y,
            static_cast<float>(HAND_WIDTH) * scale,
            static_cast<float>(HAND_HEIGHT) * scale
        };

        Vector2 origin = {
            (mirror ? (static_cast<float>(HAND_WIDTH) - static_cast<float>(PIVOT_X)) : static_cast<float>(PIVOT_X)) * scale,
            static_cast<float>(PIVOT_Y) * scale
        };

        DrawTexturePro(tex, srcRec, destRec, origin, rotationDeg, tint);
    }

private:
    Texture2D m_textures[POSE_COUNT][SKIN_COUNT][SLEEVE_COUNT];
    bool m_loaded[POSE_COUNT][SKIN_COUNT][SLEEVE_COUNT];
};

} // namespace sl
