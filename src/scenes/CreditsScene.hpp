#pragma once
#if defined(__has_include)
  #if __has_include("core/Scene.hpp")
    #include "core/Scene.hpp"
  #else
    #include "../core/Scene.hpp"
  #endif
  #if __has_include("ui/Button.hpp")
    #include "ui/Button.hpp"
  #else
    #include "../ui/Button.hpp"
  #endif
#else
  #include "../core/Scene.hpp"
  #include "../ui/Button.hpp"
#endif

namespace Starlight {

class CreditsScene : public Scene {
public:
    CreditsScene();

    void update(float dt) override;
    void draw() override;

private:
    Button m_btnBack;
    float m_animTimer = 0.0f;
};

} // namespace Starlight
