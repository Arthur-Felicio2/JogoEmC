#include "Application.hpp"
#include "Input.hpp"
#include "scenes/SplashScene.hpp"
#include "render/HandRenderer.hpp"
#include "ui/Theme.hpp"
#include <algorithm>

namespace Starlight {

Application* Application::s_instance = nullptr;

Application& Application::instance() {
    return *s_instance;
}

Application::Application() {
    s_instance = this;
}

Application::~Application() {
    cleanup();
    if (s_instance == this) {
        s_instance = nullptr;
    }
}

void Application::init() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(Config::DEFAULT_WINDOW_WIDTH, Config::DEFAULT_WINDOW_HEIGHT, Config::WINDOW_TITLE);
    SetTargetFPS(Config::TARGET_FPS);
    SetExitKey(KEY_NULL); // Prevent Esc from closing window immediately

    InitAudioDevice();

    Theme::initFont();

    m_virtualCanvas = LoadRenderTexture(Config::VIRTUAL_WIDTH, Config::VIRTUAL_HEIGHT);
    SetTextureFilter(m_virtualCanvas.texture, TEXTURE_FILTER_BILINEAR);

    // Initialize ScoreStore with scores.txt next to executable
    std::string appDir = GetApplicationDirectory();
    std::string scoresPath = appDir + "scores.txt";
    m_scoreStore = std::make_unique<sl::ScoreStore>(scoresPath);
    m_scoreStore->load();

    // Initialize Hand Atlas and Hand Renderer
    m_handAtlas = std::make_unique<sl::HandAtlas>();
    m_handAtlas->init();
    HandRenderer::instance().init();

    // Start with Splash Scene
    m_sceneManager.setScene(std::make_unique<SplashScene>());

    m_running = true;
}

void Application::cleanup() {
    if (m_running) {
        if (m_scoreStore) {
            m_scoreStore->save();
        }
        HandRenderer::instance().unload();
        if (m_handAtlas) {
            m_handAtlas->unloadAll();
        }
        Theme::unloadFont();
        UnloadRenderTexture(m_virtualCanvas);
        CloseAudioDevice();
        CloseWindow();
        m_running = false;
    }
}

void Application::run() {
    init();

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        if (dt > 0.1f) dt = 0.1f; // Clamp delta time to avoid large jumps

        // Fullscreen toggle with F11
        if (IsKeyPressed(KEY_F11)) {
            ToggleFullscreen();
        }

        // Calculate letterbox scaling
        int screenW = GetScreenWidth();
        int screenH = GetScreenHeight();
        float scale = std::min((float)screenW / (float)Config::VIRTUAL_WIDTH,
                               (float)screenH / (float)Config::VIRTUAL_HEIGHT);

        Rectangle viewport = {
            (screenW - ((float)Config::VIRTUAL_WIDTH * scale)) * 0.5f,
            (screenH - ((float)Config::VIRTUAL_HEIGHT * scale)) * 0.5f,
            (float)Config::VIRTUAL_WIDTH * scale,
            (float)Config::VIRTUAL_HEIGHT * scale
        };

        // Update virtual coordinates for mouse input
        Input::setViewport(viewport, scale);

        // Update active scene
        m_sceneManager.update(dt);

        // Render to virtual 1280x720 canvas
        BeginTextureMode(m_virtualCanvas);
        ClearBackground(BLACK);
        m_sceneManager.draw();
        EndTextureMode();

        // Render virtual canvas scaled to screen with letterbox
        BeginDrawing();
        ClearBackground(BLACK);
        Rectangle srcRect = { 0, 0, (float)Config::VIRTUAL_WIDTH, -(float)Config::VIRTUAL_HEIGHT };
        DrawTexturePro(m_virtualCanvas.texture, srcRect, viewport, Vector2{0, 0}, 0.0f, WHITE);
        EndDrawing();
    }

    cleanup();
}

} // namespace Starlight
