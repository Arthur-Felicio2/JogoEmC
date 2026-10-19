#pragma once
#include <memory>
#include "Scene.hpp"
#include "raylib.h"

namespace Starlight {

enum class TransitionState {
    None,
    FadeOut,
    FadeIn
};

class SceneManager {
public:
    static SceneManager& get();

    SceneManager();
    ~SceneManager();

    void setScene(std::unique_ptr<Scene> scene);
    void changeScene(std::unique_ptr<Scene> nextScene, float transitionDuration = 0.3f);
    void replaceScene(std::unique_ptr<Scene> scene);
    void switchScene(std::unique_ptr<Scene> scene) { changeScene(std::move(scene)); }

    void update(float dt);
    void draw();

    Scene* currentScene() const { return m_currentScene.get(); }
    bool isTransitioning() const { return m_transitionState != TransitionState::None; }

private:
    std::unique_ptr<Scene> m_currentScene;
    std::unique_ptr<Scene> m_nextScene;

    TransitionState m_transitionState = TransitionState::None;
    float m_transitionDuration = 0.3f;
    float m_transitionTimer = 0.0f;
    float m_fadeAlpha = 0.0f;
};

} // namespace Starlight

namespace sl {
    using SceneManager = Starlight::SceneManager;
}

