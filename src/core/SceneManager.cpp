#include "SceneManager.hpp"
#include <algorithm>

namespace Starlight {

SceneManager::SceneManager() = default;

SceneManager::~SceneManager() {
    if (m_currentScene) {
        m_currentScene->onExit();
    }
}

void SceneManager::setScene(std::unique_ptr<Scene> scene) {
    if (m_currentScene) {
        m_currentScene->onExit();
    }
    m_currentScene = std::move(scene);
    if (m_currentScene) {
        m_currentScene->setManager(this);
        m_currentScene->onEnter();
    }
    m_transitionState = TransitionState::None;
    m_fadeAlpha = 0.0f;
}

void SceneManager::changeScene(std::unique_ptr<Scene> nextScene, float transitionDuration) {
    if (!nextScene) return;

    if (!m_currentScene) {
        setScene(std::move(nextScene));
        return;
    }

    m_nextScene = std::move(nextScene);
    m_transitionDuration = transitionDuration > 0.0f ? transitionDuration : 0.3f;
    m_transitionTimer = 0.0f;
    m_transitionState = TransitionState::FadeOut;
}

void SceneManager::update(float dt) {
    if (m_transitionState == TransitionState::FadeOut) {
        float halfDuration = m_transitionDuration * 0.5f;
        m_transitionTimer += dt;
        m_fadeAlpha = std::clamp(m_transitionTimer / halfDuration, 0.0f, 1.0f);

        if (m_transitionTimer >= halfDuration) {
            // Swap scene
            if (m_currentScene) {
                m_currentScene->onExit();
            }
            m_currentScene = std::move(m_nextScene);
            if (m_currentScene) {
                m_currentScene->setManager(this);
                m_currentScene->onEnter();
            }
            m_transitionTimer = 0.0f;
            m_transitionState = TransitionState::FadeIn;
        }
    } else if (m_transitionState == TransitionState::FadeIn) {
        float halfDuration = m_transitionDuration * 0.5f;
        m_transitionTimer += dt;
        m_fadeAlpha = std::clamp(1.0f - (m_transitionTimer / halfDuration), 0.0f, 1.0f);

        if (m_transitionTimer >= halfDuration) {
            m_transitionState = TransitionState::None;
            m_fadeAlpha = 0.0f;
        }
    }

    if (m_currentScene) {
        m_currentScene->update(dt);
    }
}

void SceneManager::draw() {
    if (m_currentScene) {
        m_currentScene->draw();
    }

    if (m_fadeAlpha > 0.001f) {
        Color fadeColor = Color{11, 61, 46, (unsigned char)(255.0f * m_fadeAlpha)};
        DrawRectangle(0, 0, 1280, 720, fadeColor);
    }
}

} // namespace Starlight
