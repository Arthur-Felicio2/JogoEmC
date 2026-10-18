#pragma once
#include <string>

namespace Starlight {

class SaveData {
public:
    static SaveData& instance();

    void load(const std::string& filepath = "save/scores.dat");
    void save(const std::string& filepath = "save/scores.dat");

    int flappyHighScore() const { return m_flappyHighScore; }
    void setFlappyHighScore(int score);

    float soundVolume() const { return m_soundVolume; }
    void setSoundVolume(float vol) { m_soundVolume = vol; }

    bool soundEnabled() const { return m_soundEnabled; }
    void setSoundEnabled(bool enabled) { m_soundEnabled = enabled; }

    int player1SkinTone() const { return m_p1Skin; }
    void setPlayer1SkinTone(int tone) { m_p1Skin = tone; }

    int player2SkinTone() const { return m_p2Skin; }
    void setPlayer2SkinTone(int tone) { m_p2Skin = tone; }

private:
    SaveData();

    int m_flappyHighScore = 0;
    float m_soundVolume = 0.8f;
    bool m_soundEnabled = true;
    int m_p1Skin = 0;
    int m_p2Skin = 1;
};

} // namespace Starlight
