#pragma once
#include "raylib.h"
#include "render/HandAtlasRaylib.h"

namespace Starlight {

class HandRenderer {
public:
    static HandRenderer& instance();

    void init();
    void unload();

    void drawHand(int pose,
                  Vector2 wristPos,
                  float scale,
                  bool mirror,
                  int skinTone,
                  int sleeve,
                  float rotation = 0.0f,
                  float alpha = 1.0f);

    void drawHand(int pose,
                  Vector2 wristPos,
                  float scale,
                  bool mirror,
                  Color skinColor,
                  float rotation = 0.0f,
                  float alpha = 1.0f);

    sl::HandAtlas& atlas() { return m_atlas; }
    bool isReady() const { return m_initialized; }

private:
    HandRenderer();
    ~HandRenderer();

    sl::HandAtlas m_atlas;
    bool m_initialized = false;
};

} // namespace Starlight
