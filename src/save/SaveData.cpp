#include "SaveData.hpp"
#include <fstream>
#include <iostream>

namespace Starlight {

SaveData& SaveData::instance() {
    static SaveData inst;
    return inst;
}

SaveData::SaveData() {
    load();
}

void SaveData::load(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        return;
    }

    file.read(reinterpret_cast<char*>(&m_flappyHighScore), sizeof(m_flappyHighScore));
    file.read(reinterpret_cast<char*>(&m_soundVolume), sizeof(m_soundVolume));
    file.read(reinterpret_cast<char*>(&m_soundEnabled), sizeof(m_soundEnabled));
    file.read(reinterpret_cast<char*>(&m_p1Skin), sizeof(m_p1Skin));
    file.read(reinterpret_cast<char*>(&m_p2Skin), sizeof(m_p2Skin));
}

void SaveData::save(const std::string& filepath) {
    std::ofstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        return;
    }

    file.write(reinterpret_cast<const char*>(&m_flappyHighScore), sizeof(m_flappyHighScore));
    file.write(reinterpret_cast<const char*>(&m_soundVolume), sizeof(m_soundVolume));
    file.write(reinterpret_cast<const char*>(&m_soundEnabled), sizeof(m_soundEnabled));
    file.write(reinterpret_cast<const char*>(&m_p1Skin), sizeof(m_p1Skin));
    file.write(reinterpret_cast<const char*>(&m_p2Skin), sizeof(m_p2Skin));
}

void SaveData::setFlappyHighScore(int score) {
    if (score > m_flappyHighScore) {
        m_flappyHighScore = score;
        save();
    }
}

} // namespace Starlight
