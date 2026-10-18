#pragma once

namespace Starlight {

class SceneManager;

class Scene {
public:
    virtual ~Scene() = default;
    virtual void onEnter() {}
    virtual void onExit() {}
    virtual void update(float dt) = 0;
    virtual void draw() = 0;

    void setManager(SceneManager* manager) { m_manager = manager; }
    SceneManager* manager() const { return m_manager; }

protected:
    SceneManager* m_manager = nullptr;
};

} // namespace Starlight
