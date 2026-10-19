#include "MatchScene.hpp"
#include "PassDeviceScene.hpp"
#include "ResultScene.hpp"
#include "MenuScene.hpp"
#include "core/SceneManager.hpp"
#include "core/Application.hpp"
#include "core/Input.hpp"
#include "ui/Theme.hpp"
#include "ui/UiLayout.h"
#include "fx/Easing.hpp"
#include "games/RpsRules.hpp"
#include "games/OddEvenRules.hpp"
#include <cmath>
#include <algorithm>

namespace Starlight {

MatchScene::MatchScene(std::shared_ptr<Match> match)
    : m_match(std::move(match)),
      m_btnMenu("Menu", Rectangle{30, 24, 100, 44}, ButtonStyle::Secondary),
      m_btnReadyToPlay("Estou Pronto!", Rectangle{490, 480, 300, 56}, ButtonStyle::Primary) {

    m_btnMenu.setOnClick([this]() {
        if (m_manager) {
            m_manager->changeScene(std::make_unique<MenuScene>());
        }
    });

    setupTurnUI();
}

void MatchScene::onEnter() {
    m_scoreBoard.setPlayerA(m_match->playerA()->name(), m_match->scoreA(), Theme::getSkinColor(m_match->playerA()->skinTone()));
    m_scoreBoard.setPlayerB(m_match->playerB()->name(), m_match->scoreB(), Theme::getSkinColor(m_match->playerB()->skinTone()));
    m_scoreBoard.setRound(m_match->roundNumber(), m_match->targetWins(), m_match->rules()->ruleName());
}

void MatchScene::setPlayer1Move(int move) {
    m_moveA = move;
    m_phase = MatchPhase::PlayerBChoice;
    setupTurnUI();
}

void MatchScene::chooseMove(int moveIndex) {
    if (m_phase == MatchPhase::PlayerAChoice) {
        m_moveA = moveIndex;
        if (m_match->playerB()->isHuman()) {
            // 2-player local: switch to PassDeviceScene to conceal move!
            if (m_manager) {
                m_manager->changeScene(std::make_unique<PassDeviceScene>(m_match, m_moveA), 0.2f);
            }
        } else {
            // 1-player vs CPU: CPU chooses immediately
            m_moveB = m_match->playerB()->chooseMove(*m_match->rules());
            startReveal(m_moveA, m_moveB);
        }
    } else if (m_phase == MatchPhase::PlayerBChoice) {
        // Player B made their choice!
        m_moveB = moveIndex;
        startReveal(m_moveA, m_moveB);
    }
}

void MatchScene::setupTurnUI() {
    m_moveButtons.clear();

    if (m_phase == MatchPhase::PlayerAChoice || m_phase == MatchPhase::PlayerBChoice) {
        int numMoves = m_match->rules()->moveCount();
        float btnW = (numMoves == 6) ? 140.0f : 240.0f;
        float btnH = 56.0f;
        float spacing = 16.0f;
        float totalW = numMoves * btnW + (numMoves - 1) * spacing;
        float startX = 640.0f - totalW * 0.5f;

        for (int i = 0; i < numMoves; ++i) {
            std::string label;
            if (numMoves == 6) {
                label = std::to_string(i) + (i == 1 ? " Dedo" : " Dedos");
            } else {
                label = m_match->rules()->moveName(i);
            }

            Rectangle btnRect = { startX + i * (btnW + spacing), 580.0f, btnW, btnH };
            Button btn(label, btnRect, ButtonStyle::Primary);

            int moveIndex = i;
            btn.setOnClick([this, moveIndex]() {
                chooseMove(moveIndex);
            });

            m_moveButtons.push_back(btn);
        }
    }
}

void MatchScene::startReveal(int moveA, int moveB) {
    m_phase = MatchPhase::RevealAnimation;
    m_revealTimer = 0.0f;
    m_handScalePunch = 1.0f;
    m_handRotationA = 0.0f;
    m_handRotationB = 0.0f;
    m_countdownText = "";
    m_sumText = "";
    m_outcomeText = "";
    m_outcomeAlpha = 0.0f;
    m_pointAwarded = false;

    // Resolve outcome from rules
    m_roundOutcome = m_match->rules()->resolve(moveA, moveB);
}

void MatchScene::update(float dt) {
    m_btnMenu.update(dt);
    m_particles.update(dt);

    if (m_phase == MatchPhase::PlayerAChoice || m_phase == MatchPhase::PlayerBChoice) {
        int numMoves = m_match->rules()->moveCount();
        if (numMoves == 6) {
            // Par ou Ímpar keyboard shortcuts (0..5)
            if (IsKeyPressed(KEY_ZERO) || IsKeyPressed(KEY_KP_0)) chooseMove(0);
            else if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_KP_1)) chooseMove(1);
            else if (IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_KP_2)) chooseMove(2);
            else if (IsKeyPressed(KEY_THREE) || IsKeyPressed(KEY_KP_3)) chooseMove(3);
            else if (IsKeyPressed(KEY_FOUR) || IsKeyPressed(KEY_KP_4)) chooseMove(4);
            else if (IsKeyPressed(KEY_FIVE) || IsKeyPressed(KEY_KP_5)) chooseMove(5);
        } else if (numMoves == 3) {
            // Jokenpô keyboard shortcuts (1: Pedra, 2: Papel, 3: Tesoura / P, A, T)
            if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_KP_1) || IsKeyPressed(KEY_P)) chooseMove(0);
            else if (IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_KP_2) || IsKeyPressed(KEY_A)) chooseMove(1);
            else if (IsKeyPressed(KEY_THREE) || IsKeyPressed(KEY_KP_3) || IsKeyPressed(KEY_T)) chooseMove(2);
        }

        for (auto& btn : m_moveButtons) {
            btn.update(dt);
        }
    } else if (m_phase == MatchPhase::RevealAnimation) {
        m_revealTimer += dt;

        // Quick advance if user presses ENTER, SPACE or clicks once result is displayed
        if (m_revealTimer >= 2.6f && (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER) || IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT))) {
            finishRound();
            return;
        }

        // 0.0 - 0.4s: Hands enter from sides (easeOutBack)
        if (m_revealTimer < 0.4f) {
            float t = m_revealTimer / 0.4f;
            float eased = Easing::easeOutBack(t);
            m_hand1Offset = Easing::lerp(-200.0f, 380.0f, eased);
            m_hand2Offset = Easing::lerp(1480.0f, 900.0f, eased);
            m_handRotationA = 0.0f;
            m_handRotationB = 0.0f;
            m_countdownText = "";
        } else if (m_revealTimer < 1.6f) {
            // 0.4 - 1.6s: Three swings with countdown around wrist pivot
            m_hand1Offset = 380.0f;
            m_hand2Offset = 900.0f;

            float swingT = (m_revealTimer - 0.4f) / 1.2f;
            m_handRotationA = sinf(swingT * 3.0f * 6.28318f) * 16.0f;
            m_handRotationB = -sinf(swingT * 3.0f * 6.28318f) * 16.0f;

            if (swingT < 0.33f) {
                m_countdownText = (m_match->rules()->moveCount() == 6) ? "PAR..." : "JÔ...";
            } else if (swingT < 0.66f) {
                m_countdownText = (m_match->rules()->moveCount() == 6) ? "OU..." : "KÊN...";
            } else {
                m_countdownText = (m_match->rules()->moveCount() == 6) ? "ÍMPAR!" : "PÔ!";
            }
        } else {
            // 1.6s: Reveal of pose with scale punch (1.0 -> 1.25 -> 1.0 in 0.25s)
            m_handRotationA = 0.0f;
            m_handRotationB = 0.0f;
            m_countdownText = "";

            if (m_revealTimer < 1.85f) {
                float punchT = (m_revealTimer - 1.6f) / 0.25f;
                m_handScalePunch = 1.0f + sinf(punchT * 3.14159f) * 0.25f;
            } else {
                m_handScalePunch = 1.0f;
            }

            // 1.9s: Par ou Ímpar sum appears
            if (m_revealTimer >= 1.9f && m_match->rules()->moveCount() == 6) {
                int sum = m_moveA + m_moveB;
                std::string parity = (sum % 2 == 0) ? "PAR" : "ÍMPAR";
                m_sumText = std::to_string(m_moveA) + " + " + std::to_string(m_moveB) + " = " + std::to_string(sum) + " (" + parity + ")";
            }

            // 2.5s: Result text and points awarded
            if (m_revealTimer >= 2.5f) {
                float alphaT = std::min(1.0f, (m_revealTimer - 2.5f) / 0.4f);
                m_outcomeAlpha = alphaT;

                if (!m_pointAwarded) {
                    m_pointAwarded = true;
                    // Play actual round in Match
                    m_match->playRound(m_moveA, m_moveB);
                    m_scoreBoard.setPlayerA(m_match->playerA()->name(), m_match->scoreA(), Theme::getSkinColor(m_match->playerA()->skinTone()));
                    m_scoreBoard.setPlayerB(m_match->playerB()->name(), m_match->scoreB(), Theme::getSkinColor(m_match->playerB()->skinTone()));

                    if (m_roundOutcome == Outcome::WinA) {
                        m_outcomeText = "Vitória de " + m_match->playerA()->name() + "!";
                        m_outcomeColor = Theme::ActionPlayAgain;
                        m_particles.spawnConfetti(380.0f, 380.0f, 60);
                    } else if (m_roundOutcome == Outcome::WinB) {
                        m_outcomeText = "Vitória de " + m_match->playerB()->name() + "!";
                        m_outcomeColor = Theme::ActionPlayAgain;
                        m_particles.spawnConfetti(900.0f, 380.0f, 60);
                    } else {
                        m_outcomeText = "Empate! Rodada repetida!";
                        m_outcomeColor = Theme::NeonPurple;
                    }
                }
            }

            // 4.5s: Complete round or transition to ResultScene
            if (m_revealTimer >= 4.5f) {
                finishRound();
            }
        }
    }
}

void MatchScene::finishRound() {
    if (m_match->isOver()) {
        if (m_manager) {
            m_manager->replaceScene(std::make_unique<ResultScene>(m_match));
        }
    } else {
        // Next round
        m_phase = MatchPhase::PlayerAChoice;
        m_scoreBoard.setRound(m_match->roundNumber(), m_match->targetWins(), m_match->rules()->ruleName());
        setupTurnUI();
    }
}

void MatchScene::draw() {
    Theme::drawBackgroundGradient();

    // ScoreBoard HUD
    m_scoreBoard.draw();
    m_btnMenu.draw();

    auto& atlas = Application::instance().handAtlas();

    // Turn selection prompt
    if (m_phase == MatchPhase::PlayerAChoice || m_phase == MatchPhase::PlayerBChoice) {
        bool isP1 = (m_phase == MatchPhase::PlayerAChoice);
        std::string pName = isP1 ? m_match->playerA()->name() : m_match->playerB()->name();

        Rectangle promptCard = UiLayout::centerCard(480.0f, 74.0f);
        promptCard.y = 485.0f;
        Theme::drawCard(promptCard, 14.0f, true);

        std::string promptText = "Vez de " + pName + ": Faça sua jogada!";
        int tw = Theme::measureText(promptText, 22);
        Theme::drawText(promptText, static_cast<int>(promptCard.x + (promptCard.width - tw) * 0.5f), static_cast<int>(promptCard.y + 24), 22, Theme::NeonPurple);

        for (auto& btn : m_moveButtons) {
            btn.draw();
        }

        // Preview idle hands on sides with floor shadows
        DrawEllipse(250, 445, 60, 16, Theme::ShadowColor);
        DrawEllipse(1030, 445, 60, 16, Theme::ShadowColor);

        atlas.draw(0, m_match->playerA()->skinTone(), 0, false, Vector2{250.0f, 440.0f}, 4.2f, 0.0f, Color{255, 255, 255, 180});
        atlas.draw(0, m_match->playerB()->skinTone(), 1, true, Vector2{1030.0f, 440.0f}, 4.2f, 0.0f, Color{255, 255, 255, 180});
    }

    // Animation Phase Hands and Effects
    if (m_phase == MatchPhase::RevealAnimation) {
        // Floor shadows under wrists
        DrawEllipse(static_cast<int>(m_hand1Offset), 555, 65, 18, Theme::ShadowColor);
        DrawEllipse(static_cast<int>(m_hand2Offset), 555, 65, 18, Theme::ShadowColor);

        // Determine pose to display: closed fist during swing, then chosen pose at 1.6s
        int poseA = 0;
        int poseB = 0;

        if (m_revealTimer >= 1.6f) {
            if (m_match->rules()->moveCount() == 6) {
                poseA = m_moveA;
                poseB = m_moveB;
            } else {
                poseA = RpsRules::toPoseIndex(m_moveA);
                poseB = RpsRules::toPoseIndex(m_moveB);
            }
        }

        // Player A Hand (Left, Blue sleeve, not mirrored)
        atlas.draw(poseA, m_match->playerA()->skinTone(), 0, false, Vector2{m_hand1Offset, 550.0f}, 5.0f * m_handScalePunch, m_handRotationA, WHITE);

        // Player B Hand (Right, Red sleeve, mirrored)
        atlas.draw(poseB, m_match->playerB()->skinTone(), 1, true, Vector2{m_hand2Offset, 550.0f}, 5.0f * m_handScalePunch, m_handRotationB, WHITE);

        // Countdown text ("1... 2... 3..." / "JÔ... KÊN... PÔ!")
        if (!m_countdownText.empty()) {
            int cw = Theme::measureText(m_countdownText, 48);
            Theme::drawText(m_countdownText, 640 - cw / 2 + 2, 220 + 2, 48, Color{0, 0, 0, 180});
            Theme::drawText(m_countdownText, 640 - cw / 2, 220, 48, Theme::NeonPurple);
        }

        // Sum text for Par ou Ímpar
        if (!m_sumText.empty()) {
            Rectangle sumCard = { 440, 200, 400, 64 };
            Theme::drawCard(sumCard, 14.0f, true);
            int sw = Theme::measureText(m_sumText, 24);
            Theme::drawText(m_sumText, static_cast<int>(sumCard.x + (sumCard.width - sw) * 0.5f), static_cast<int>(sumCard.y + 20), 24, Theme::NeonPurple);
        }

        // Outcome Text Banner
        if (!m_outcomeText.empty() && m_outcomeAlpha > 0.01f) {
            Rectangle outCard = { 420, 560, 440, 70 };
            Color cardColor = Theme::CardSurface;
            cardColor.a = static_cast<unsigned char>(220 * m_outcomeAlpha);
            Theme::drawCard(outCard, 16.0f, true, cardColor);

            Color textC = m_outcomeColor;
            textC.a = static_cast<unsigned char>(255 * m_outcomeAlpha);
            int ow = Theme::measureText(m_outcomeText, 24);
            Theme::drawText(m_outcomeText, static_cast<int>(outCard.x + (outCard.width - ow) * 0.5f), static_cast<int>(outCard.y + 22), 24, textC);
        }

        m_particles.draw();
    }
}

} // namespace Starlight
