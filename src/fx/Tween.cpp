#include "Tween.hpp"
#include <algorithm>

namespace Starlight {

Tween::Tween()
    : m_from(0.0f), m_to(0.0f), m_duration(0.0f), m_elapsed(0.0f),
      m_currentValue(0.0f), m_running(false), m_finished(true),
      m_ease(Easing::linear), m_onComplete(nullptr) {}

Tween::Tween(float from, float to, float duration, EasingFunc ease, std::function<void()> onComplete) {
    start(from, to, duration, ease, onComplete);
}

void Tween::start(float from, float to, float duration, EasingFunc ease, std::function<void()> onComplete) {
    m_from = from;
    m_to = to;
    m_duration = duration > 0.0f ? duration : 0.0001f;
    m_elapsed = 0.0f;
    m_currentValue = from;
    m_running = true;
    m_finished = false;
    m_ease = ease ? ease : Easing::linear;
    m_onComplete = onComplete;
}

void Tween::update(float dt) {
    if (!m_running || m_finished) return;

    m_elapsed += dt;
    float t = std::clamp(m_elapsed / m_duration, 0.0f, 1.0f);
    float easedT = m_ease(t);
    m_currentValue = Easing::lerp(m_from, m_to, easedT);

    if (m_elapsed >= m_duration) {
        m_currentValue = m_to;
        m_running = false;
        m_finished = true;
        if (m_onComplete) {
            auto cb = m_onComplete;
            m_onComplete = nullptr;
            cb();
        }
    }
}

void Tween::stop() {
    m_running = false;
    m_finished = true;
}

} // namespace Starlight
