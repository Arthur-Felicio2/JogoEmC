#include "FlappyScene.hpp"
#include "NameEntryScene.hpp"
#include "ScoreScene.hpp"
#include "MenuScene.hpp"
#include "core/SceneManager.hpp"
#include "core/Application.hpp"
#include "core/Input.hpp"
#include "ui/Theme.hpp"
#include <cmath>
#include <cstdlib>

namespace Starlight {

FlappyScene::FlappyScene(std::string playerName, int skinTone)
    : m_playerName(std::move(playerName)),
      m_skinTone(skinTone),
      m_btnBack("Voltar", Rectangle{30, 24, 100, 44}, ButtonStyle::Secondary),
      m_btnRestart("Jogar Novamente", Rectangle{420, 360, 210, 46}, ButtonStyle::Primary),
      m_btnChangePlayer("Trocar Jogador", Rectangle{650, 360, 210, 46}, ButtonStyle::Secondary),
      m_btnScores("Ver Placar", Rectangle{420, 420, 210, 46}, ButtonStyle::Secondary),
      m_btnMenu("Menu Principal", Rectangle{650, 420, 210, 46}, ButtonStyle::Secondary) {

    m_btnBack.setOnClick([this]() {
        if (m_manager) {
            m_manager->changeScene(std::make_unique<MenuScene>());
        }
    });

    m_btnRestart.setOnClick([this]() {
        resetGame();
    });

    m_btnChangePlayer.setOnClick([this]() {
        if (m_manager) {
            m_manager->changeScene(std::make_unique<NameEntryScene>(GameType::Flappy, false, false, m_playerName, "Jogador 2", m_skinTone));
        }
    });

    m_btnScores.setOnClick([this]() {
        if (m_manager) {
            m_manager->changeScene(std::make_unique<ScoreScene>(3, m_playerName));
        }
    });

    m_btnMenu.setOnClick([this]() {
        if (m_manager) {
            m_manager->changeScene(std::make_unique<MenuScene>());
        }
    });
}

void FlappyScene::onEnter() {
    // Read this player's high score from ScoreStore
    const auto* prof = Application::instance().scoreStore().find(m_playerName);
    m_highScore = prof ? prof->flappyHighScore : 0;
    resetGame();
}

void FlappyScene::resetGame() {
    m_state = FlappyState::Ready;
    m_birdPos = {260, 340};
    m_birdVelocityY = 0.0f;
    m_birdRotation = 0.0f;
    m_score = 0;
    m_isNewHighScore = false;
    m_currentPipeSpeed = BASE_PIPE_SPEED;
    m_currentGap = BASE_GAP;
    m_pipes.clear();

    // Spawn 3 initial pipes spaced out
    for (int i = 0; i < 3; ++i) {
        spawnPipe(800.0f + i * PIPE_SPACING);
    }
}

void FlappyScene::spawnPipe(float xPos) {
    Pipe p;
    p.x = xPos;
    p.gap = m_currentGap;
    // Top height between 100 and 360 px
    p.topHeight = 100.0f + static_cast<float>(rand() % 260);
    p.scored = false;
    m_pipes.push_back(p);
}

void FlappyScene::flap() {
    m_birdVelocityY = FLAP_IMPULSE;
    m_particles.spawnConfetti(m_birdPos.x - 10.0f, m_birdPos.y + 10.0f, 6);
}

void FlappyScene::updatePhysics(float dt) {
    // Apply gravity
    m_birdVelocityY += GRAVITY * dt;
    m_birdPos.y += m_birdVelocityY * dt;

    // Bird rotation based on velocity
    float targetRot = m_birdVelocityY * 0.08f;
    if (targetRot > 70.0f) targetRot = 70.0f;
    if (targetRot < -30.0f) targetRot = -30.0f;
    m_birdRotation += (targetRot - m_birdRotation) * 10.0f * dt;

    // Wing flap animation
    m_wingTimer += dt;
    if (m_wingTimer > 0.1f) {
        m_wingTimer = 0.0f;
        m_wingFrame = (m_wingFrame + 1) % 3;
    }

    // Move pipes
    for (auto& p : m_pipes) {
        p.x -= m_currentPipeSpeed * dt;

        // Score pipe pass
        if (!p.scored && p.x + PIPE_WIDTH < m_birdPos.x) {
            p.scored = true;
            m_score++;
            m_particles.spawnConfetti(m_birdPos.x, m_birdPos.y - 30.0f, 10);

            // Progressive difficulty: slightly increase speed and narrow gap
            if (m_score % 5 == 0) {
                m_currentPipeSpeed = std::min(360.0f, m_currentPipeSpeed + 15.0f);
                m_currentGap = std::max(140.0f, m_currentGap - 5.0f);
            }
        }
    }

    // Recycle pipes that went off screen
    if (!m_pipes.empty() && m_pipes.front().x + PIPE_WIDTH < -50.0f) {
        m_pipes.erase(m_pipes.begin());
        float lastX = m_pipes.back().x;
        spawnPipe(lastX + PIPE_SPACING);
    }

    // Parallax scrolling
    m_parallaxFarX -= (m_currentPipeSpeed * 0.25f) * dt;
    m_parallaxNearX -= (m_currentPipeSpeed * 0.5f) * dt;
    m_groundX -= m_currentPipeSpeed * dt;

    if (m_parallaxFarX <= -1280.0f) m_parallaxFarX += 1280.0f;
    if (m_parallaxNearX <= -1280.0f) m_parallaxNearX += 1280.0f;
    if (m_groundX <= -1280.0f) m_groundX += 1280.0f;
}

void FlappyScene::update(float dt) {
    m_btnBack.update(dt);
    m_particles.update(dt);

    if (m_shakeTimer > 0.0f) {
        m_shakeTimer -= dt;
    }

    if (m_state == FlappyState::Ready) {
        // Hover bird slightly
        static float readyHover = 0.0f;
        readyHover += dt * 4.0f;
        m_birdPos.y = 340.0f + sinf(readyHover) * 12.0f;

        bool jumpInput = IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP) ||
                         (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && Input::getVirtualMousePosition().x > 150.0f);

        if (jumpInput) {
            m_state = FlappyState::Playing;
            flap();
        }
    } else if (m_state == FlappyState::Playing) {
        bool jumpInput = IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP) ||
                         (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && Input::getVirtualMousePosition().x > 150.0f);

        if (jumpInput) {
            flap();
        }

        updatePhysics(dt);
        checkCollisions();
    } else if (m_state == FlappyState::Dying) {
        // Fall to ground after collision
        m_birdVelocityY += GRAVITY * dt;
        m_birdPos.y += m_birdVelocityY * dt;
        m_birdRotation += 200.0f * dt;

        if (m_birdPos.y + m_birdRadius >= GROUND_Y) {
            m_birdPos.y = GROUND_Y - m_birdRadius;
            triggerGameOver();
        }
    } else if (m_state == FlappyState::GameOver) {
        m_btnRestart.update(dt);
        m_btnChangePlayer.update(dt);
        m_btnScores.update(dt);
        m_btnMenu.update(dt);

        if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
            resetGame();
        }
    }
}

void FlappyScene::checkCollisions() {
    // Ceiling
    if (m_birdPos.y - m_birdRadius <= 0) {
        m_birdPos.y = m_birdRadius;
        m_birdVelocityY = 0;
    }

    // Ground
    if (m_birdPos.y + m_birdRadius >= GROUND_Y) {
        m_birdPos.y = GROUND_Y - m_birdRadius;
        m_shakeTimer = 0.25f;
        triggerGameOver();
        return;
    }

    // Pipes collision (Circle vs Rectangle)
    for (const auto& p : m_pipes) {
        Rectangle topPipeRect = { p.x, 0, PIPE_WIDTH, p.topHeight };
        Rectangle botPipeRect = { p.x, p.topHeight + p.gap, PIPE_WIDTH, GROUND_Y - (p.topHeight + p.gap) };

        if (CheckCollisionCircleRec(m_birdPos, m_birdRadius, topPipeRect) ||
            CheckCollisionCircleRec(m_birdPos, m_birdRadius, botPipeRect)) {
            m_shakeTimer = 0.25f;
            m_state = FlappyState::Dying;
            m_birdVelocityY = -200.0f; // tiny bounce
            break;
        }
    }
}

void FlappyScene::triggerGameOver() {
    m_state = FlappyState::GameOver;
    auto& store = Application::instance().scoreStore();
    bool isNew = store.recordFlappy(m_playerName, m_score);
    store.save();

    if (isNew) {
        m_highScore = m_score;
        m_isNewHighScore = true;
        m_particles.spawnConfettiShower(120);
    }
}

void FlappyScene::draw() {
    // Screen shake offset
    float shakeX = 0.0f;
    float shakeY = 0.0f;
    if (m_shakeTimer > 0.0f) {
        shakeX = (static_cast<float>(rand() % 16) - 8.0f);
        shakeY = (static_cast<float>(rand() % 16) - 8.0f);
    }

    // Sky Background gradient
    DrawRectangleGradientV(0, 0, 1280, 720, Color{10, 25, 47, 255}, Color{20, 60, 65, 255});

    // Parallax Layer 1: Far Mountain Silhouettes
    for (int offset = -1; offset <= 2; ++offset) {
        float x = m_parallaxFarX + offset * 1280.0f + shakeX;
        DrawTriangle(Vector2{x, GROUND_Y}, Vector2{x + 300, 380}, Vector2{x + 600, GROUND_Y}, Color{15, 45, 55, 255});
        DrawTriangle(Vector2{x + 500, GROUND_Y}, Vector2{x + 850, 420}, Vector2{x + 1200, GROUND_Y}, Color{18, 48, 60, 255});
    }

    // Parallax Layer 2: Near Spire City Silhouettes
    for (int offset = -1; offset <= 2; ++offset) {
        float x = m_parallaxNearX + offset * 1280.0f + shakeX;
        DrawRectangle(static_cast<int>(x + 100), 480, 120, static_cast<int>(GROUND_Y - 480), Color{14, 38, 48, 255});
        DrawRectangle(static_cast<int>(x + 260), 520, 90, static_cast<int>(GROUND_Y - 520), Color{12, 34, 44, 255});
        DrawRectangle(static_cast<int>(x + 500), 460, 140, static_cast<int>(GROUND_Y - 460), Color{14, 38, 48, 255});
        DrawRectangle(static_cast<int>(x + 780), 500, 110, static_cast<int>(GROUND_Y - 500), Color{12, 34, 44, 255});
    }

    // Pipes
    Color pipeColor = Color{34, 139, 94, 255};
    Color pipeBorder = Color{20, 80, 55, 255};
    Color pipeHighlight = Color{56, 189, 130, 255};

    for (const auto& p : m_pipes) {
        float px = p.x + shakeX;

        // Top Pipe
        Rectangle topRect = { px, 0, PIPE_WIDTH, p.topHeight };
        DrawRectangleRec(topRect, pipeColor);
        DrawRectangle(static_cast<int>(px + 10), 0, 14, static_cast<int>(p.topHeight), pipeHighlight);
        DrawRectangleLinesEx(topRect, 3.0f, pipeBorder);

        // Top Pipe Rim Collar
        Rectangle topCollar = { px - 6, p.topHeight - 26, PIPE_WIDTH + 12, 26 };
        DrawRectangleRounded(topCollar, 0.2f, 4, pipeColor);
        DrawRectangleRoundedLines(topCollar, 0.2f, 4, 3.0f, pipeBorder);

        // Bottom Pipe
        float botY = p.topHeight + p.gap;
        Rectangle botRect = { px, botY, PIPE_WIDTH, GROUND_Y - botY };
        DrawRectangleRec(botRect, pipeColor);
        DrawRectangle(static_cast<int>(px + 10), static_cast<int>(botY), 14, static_cast<int>(GROUND_Y - botY), pipeHighlight);
        DrawRectangleLinesEx(botRect, 3.0f, pipeBorder);

        // Bottom Pipe Rim Collar
        Rectangle botCollar = { px - 6, botY, PIPE_WIDTH + 12, 26 };
        DrawRectangleRounded(botCollar, 0.2f, 4, pipeColor);
        DrawRectangleRoundedLines(botCollar, 0.2f, 4, 3.0f, pipeBorder);
    }

    // Ground Strip
    DrawRectangle(0, static_cast<int>(GROUND_Y), 1280, static_cast<int>(720 - GROUND_Y), Color{22, 60, 48, 255});
    DrawLine(0, static_cast<int>(GROUND_Y), 1280, static_cast<int>(GROUND_Y), Color{52, 160, 110, 255});
    for (int offset = -1; offset <= 2; ++offset) {
        float gx = m_groundX + offset * 1280.0f;
        for (int i = 0; i < 16; ++i) {
            DrawRectangle(static_cast<int>(gx + i * 80), static_cast<int>(GROUND_Y + 10), 40, 8, Color{16, 45, 36, 255});
        }
    }

    // Bird
    Vector2 bPos = { m_birdPos.x + shakeX, m_birdPos.y + shakeY };
    DrawCircle(static_cast<int>(bPos.x), static_cast<int>(bPos.y), m_birdRadius, Color{244, 185, 66, 255});
    DrawCircleLines(static_cast<int>(bPos.x), static_cast<int>(bPos.y), m_birdRadius, Color{190, 120, 20, 255});

    // Bird Belly
    DrawCircle(static_cast<int>(bPos.x - 2), static_cast<int>(bPos.y + 4), m_birdRadius * 0.65f, Color{255, 235, 170, 255});

    // Eye
    DrawCircle(static_cast<int>(bPos.x + 8), static_cast<int>(bPos.y - 6), 6, WHITE);
    DrawCircle(static_cast<int>(bPos.x + 10), static_cast<int>(bPos.y - 6), 3, BLACK);

    // Beak
    DrawTriangle(Vector2{bPos.x + 12, bPos.y - 2}, Vector2{bPos.x + 24, bPos.y + 4}, Vector2{bPos.x + 12, bPos.y + 8}, Color{235, 94, 40, 255});

    // Wing
    float wingOffsetY = (m_wingFrame == 0) ? -6.0f : (m_wingFrame == 1 ? 0.0f : 6.0f);
    DrawEllipse(static_cast<int>(bPos.x - 8), static_cast<int>(bPos.y + wingOffsetY), 9, 6, Color{225, 150, 30, 255});

    // Particles
    m_particles.draw();

    // Player name badge in HUD
    std::string badge = "Jogador: " + m_playerName;
    Theme::drawText(badge, 150, 34, 18, Theme::TEXT_SECONDARY);

    // In-game score counter HUD
    if (m_state == FlappyState::Playing || m_state == FlappyState::Ready) {
        std::string sStr = std::to_string(m_score);
        int sw = Theme::measureText(sStr, 64);
        Theme::drawText(sStr, static_cast<int>(640 - sw * 0.5f) + 3, 53, 64, Color{0, 0, 0, 120});
        Theme::drawText(sStr, static_cast<int>(640 - sw * 0.5f), 50, 64, Theme::TEXT_PRIMARY);
    }

    // Ready prompt overlay
    if (m_state == FlappyState::Ready) {
        Rectangle card = { 380, 220, 520, 150 };
        Theme::drawCard(card, 16.0f, true);
        const char* rTitle = "STARLIGHT BIRD";
        int rtw = Theme::measureText(rTitle, 28);
        Theme::drawText(rTitle, static_cast<int>(card.x + (card.width - rtw) * 0.5f), static_cast<int>(card.y + 24), 28, Theme::ACCENT_GOLD);

        const char* rPrompt = "Pressione ESPAÇO ou CLIQUE para voar!";
        int rpw = Theme::measureText(rPrompt, 20);
        Theme::drawText(rPrompt, static_cast<int>(card.x + (card.width - rpw) * 0.5f), static_cast<int>(card.y + 75), 20, Theme::TEXT_PRIMARY);

        std::string rRec = "Recorde de " + m_playerName + ": " + std::to_string(m_highScore) + " canos";
        int rrw = Theme::measureText(rRec, 16);
        Theme::drawText(rRec, static_cast<int>(card.x + (card.width - rrw) * 0.5f), static_cast<int>(card.y + 110), 16, Theme::TEXT_SECONDARY);
    }

    // Game Over Panel
    if (m_state == FlappyState::GameOver) {
        m_particles.draw();

        Rectangle modal = { 390, 120, 500, 410 };
        Theme::drawCard(modal, 20.0f, true);

        const char* goTitle = "FIM DE JOGO!";
        int gow = Theme::measureText(goTitle, 32);
        Theme::drawText(goTitle, static_cast<int>(modal.x + (modal.width - gow) * 0.5f), static_cast<int>(modal.y + 24), 32, Theme::ALERT_DANGER);

        if (m_isNewHighScore) {
            const char* nhs = "★ NOVO RECORDE PESSOAL! ★";
            int nw = Theme::measureText(nhs, 22);
            Theme::drawText(nhs, static_cast<int>(modal.x + (modal.width - nw) * 0.5f), static_cast<int>(modal.y + 70), 22, Theme::ACCENT_GOLD);
        } else {
            std::string subUser = "Boa tentativa, " + m_playerName + "!";
            int uw = Theme::measureText(subUser, 18);
            Theme::drawText(subUser, static_cast<int>(modal.x + (modal.width - uw) * 0.5f), static_cast<int>(modal.y + 70), 18, Theme::TEXT_SECONDARY);
        }

        // Score row
        Theme::drawText("Pontuação da Rodada:", static_cast<int>(modal.x + 60), static_cast<int>(modal.y + 115), 20, Theme::TEXT_SECONDARY);
        std::string sc = std::to_string(m_score);
        int scw = Theme::measureText(sc, 28);
        Theme::drawText(sc, static_cast<int>(modal.x + modal.width - 60 - scw), static_cast<int>(modal.y + 112), 28, Theme::ACCENT_GOLD);

        // Best score row
        Theme::drawText("Seu Melhor Recorde:", static_cast<int>(modal.x + 60), static_cast<int>(modal.y + 165), 20, Theme::TEXT_SECONDARY);
        std::string bc = std::to_string(m_highScore);
        int bcw = Theme::measureText(bc, 28);
        Theme::drawText(bc, static_cast<int>(modal.x + modal.width - 60 - bcw), static_cast<int>(modal.y + 162), 28, Theme::TEXT_PRIMARY);

        m_btnRestart.draw();
        m_btnChangePlayer.draw();
        m_btnScores.draw();
        m_btnMenu.draw();
    }

    m_btnBack.draw();
}

} // namespace Starlight
