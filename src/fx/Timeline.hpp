#pragma once
#include <vector>
#include <functional>

namespace Starlight {

struct TimelineEvent {
    float triggerTime;
    std::function<void()> action;
    bool triggered = false;
};

class Timeline {
public:
    Timeline() : m_currentTime(0.0f), m_running(false) {}

    void addEvent(float triggerTime, std::function<void()> action) {
        m_events.push_back({triggerTime, std::move(action), false});
    }

    void start() {
        m_currentTime = 0.0f;
        m_running = true;
        for (auto& ev : m_events) {
            ev.triggered = false;
        }
    }

    void clear() {
        m_events.clear();
        m_currentTime = 0.0f;
        m_running = false;
    }

    void update(float dt) {
        if (!m_running) return;
        m_currentTime += dt;
        bool allTriggered = true;
        for (auto& ev : m_events) {
            if (!ev.triggered) {
                if (m_currentTime >= ev.triggerTime) {
                    ev.triggered = true;
                    if (ev.action) {
                        ev.action();
                    }
                } else {
                    allTriggered = false;
                }
            }
        }
        if (allTriggered && !m_events.empty()) {
            m_running = false;
        }
    }

    float currentTime() const { return m_currentTime; }
    bool isRunning() const { return m_running; }
    void stop() { m_running = false; }

private:
    float m_currentTime;
    bool m_running;
    std::vector<TimelineEvent> m_events;
};

} // namespace Starlight
