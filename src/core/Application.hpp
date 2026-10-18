#pragma once
#include "SceneManager.hpp"
#include "Config.hpp"
#include "raylib.h"
#include "save/ScoreStore.h"
#include "render/HandAtlasRaylib.h"
#include <memory>

namespace Starlight {

class Application {
public:
    static Application& instance();

    Application();
    ~Application();

    void run();

    sl::ScoreStore& scoreStore() { return *m_scoreStore; }
    sl::HandAtlas& handAtlas() { return *m_handAtlas; }
    SceneManager& sceneManager() { return m_sceneManager; }

private:
    void init();
    void cleanup();

    static Application* s_instance;

    SceneManager m_sceneManager;
    RenderTexture2D m_virtualCanvas;
    std::unique_ptr<sl::ScoreStore> m_scoreStore;
    std::unique_ptr<sl::HandAtlas> m_handAtlas;
    bool m_running = false;
};

} // namespace Starlight
