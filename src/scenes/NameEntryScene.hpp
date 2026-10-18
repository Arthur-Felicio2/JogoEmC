#pragma once
#include "core/Scene.hpp"
#include "ui/Button.hpp"
#include "ui/TextField.hpp"
#include "MenuScene.hpp"
#include <string>
#include <vector>

namespace Starlight {

class NameEntryScene : public Scene {
public:
    NameEntryScene(GameType gameType,
                   bool isTwoPlayers = false,
                   bool p1IsEven = true,
                   std::string defaultName1 = "Jogador 1",
                   std::string defaultName2 = "Jogador 2",
                   int skinTone1 = 0,
                   int skinTone2 = 1);

    void update(float dt) override;
    void draw() override;

private:
    void confirmAndStart();

    GameType m_gameType;
    bool m_isTwoPlayers;
    bool m_p1IsEven;

    TextField m_nameFieldP1;
    TextField m_nameFieldP2;

    int m_skinToneP1 = 0;
    int m_skinToneP2 = 1;

    Button m_btnConfirm;
    Button m_btnBack;

    // Quick chips for existing profiles
    std::vector<Button> m_chipsP1;
    std::vector<Button> m_chipsP2;

    std::string m_warningMessage;
    float m_animTimer = 0.0f;
};

} // namespace Starlight
