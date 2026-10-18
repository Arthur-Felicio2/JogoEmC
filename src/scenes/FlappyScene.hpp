#pragma once
#include "core/Scene.hpp"
#include "ui/Button.hpp"
#include "fx/ParticleSystem.hpp"
#include <vector>
#include <string>

namespace Starlight {

enum class FlappyState {
    Ready,
    Playing,
    Dying,
    GameOver
};

struct Pipe {
    float x;
    float topHeight;
    float gap;
    bool scored = false;
};

class FlappyScene : public Scene {
public:
    explicit FlappyScene(std::string playerName = "Jogador 1", int skinTone = 0);

    void onEnter() override;
    void update(float dt) override;
    void draw() override;

private:
    void resetGame();
    void flap();
    void spawnPipe(float xPos);
    void updatePhysics(float dt);
    void checkCollisions();
    void triggerGameOver();

    std::string m_playerName;
    int m_skinTone = 0;

    FlappyState m_state = FlappyState::Ready;

    // Bird
    Vector2 m_birdPos = {260, 340};
    float m_birdVelocityY = 0.0f;
    float m_birdRadius = 18.0f;
    float m_birdRotation = 0.0f;
    float m_wingTimer = 0.0f;
    int m_wingFrame = 0; // 0, 1, 2

    // Physics parameters
    static constexpr float GRAVITY = 1800.0f;
    static constexpr float FLAP_IMPULSE = -520.0f;
    static constexpr float BASE_PIPE_SPEED = 220.0f;
    static constexpr float BASE_GAP = 190.0f;
    static constexpr float PIPE_SPACING = 320.0f;
    static constexpr float PIPE_WIDTH = 90.0f;
    static constexpr float GROUND_Y = 650.0f;

    float m_currentPipeSpeed = BASE_PIPE_SPEED;
    float m_currentGap = BASE_GAP;

    std::vector<Pipe> m_pipes;

    // Parallax scrolling offsets
    float m_parallaxFarX = 0.0f;
    float m_parallaxNearX = 0.0f;
    float m_groundX = 0.0f;

    // Scoring & Highscore
    int m_score = 0;
    int m_highScore = 0;
    bool m_isNewHighScore = false;

    // Screen shake on death
    float m_shakeTimer = 0.0f;

    // Celebration
    ParticleSystem m_particles;

    // Buttons
    Button m_btnBack;
    Button m_btnRestart;
    Button m_btnChangePlayer;
    Button m_btnScores;
    Button m_btnMenu;
};

} // namespace Starlight
