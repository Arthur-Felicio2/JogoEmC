#pragma once
#include <functional>
#include "Easing.hpp"

namespace Starlight {

class Tween {
public:
    using EasingFunc = std::function<float(float)>;

    Tween();
    Tween(float from, float to, float duration, EasingFunc ease = Easing::linear, std::function<void()> onComplete = nullptr);

    void start(float from, float to, float duration, EasingFunc ease = Easing::linear, std::function<void()> onComplete = nullptr);
    void update(float dt);
    void stop();

    float value() const { return m_currentValue; }
    bool isFinished() const { return m_finished; }
    bool isRunning() const { return m_running; }
    float progress() const { return m_duration > 0.0f ? m_elapsed / m_duration : 1.0f; }

private:
    float m_from;
    float m_to;
    float m_duration;
    float m_elapsed;
    float m_currentValue;
    bool m_running;
    bool m_finished;
    EasingFunc m_ease;
    std::function<void()> m_onComplete;
};

} // namespace Starlight
