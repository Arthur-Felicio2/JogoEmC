#include "ResultScene.hpp"
#include "MatchScene.hpp"
#include "NameEntryScene.hpp"
#include "ScoreScene.hpp"
#include "MenuScene.hpp"
#include "core/SceneManager.hpp"
#include "core/Application.hpp"
#include "ui/Theme.hpp"
#include "ui/UiLayout.h"

namespace Starlight {

ResultScene::ResultScene(std::shared_ptr<Match> match)
    : m_match(std::move(match)),
      m_store(Application::instance().scoreStore()),
      m_trophy(Vector2{640, 215}, 0.95f),
      m_btnPlayAgain("SIM - JOGAR NOVAMENTE [ENTER]", Rectangle{430, 420, 420, 52}, ButtonStyle::Success),
      m_btnBackToMenu("NÃO - VOLTAR AO MENU [ESC]", Rectangle{430, 482, 420, 52}, ButtonStyle::Danger),
      m_btnScores("🏆 Ver Placar", Rectangle{430, 546, 205, 44}, ButtonStyle::Secondary),
      m_btnChangePlayers("👥 Trocar Nomes", Rectangle{645, 546, 205, 44}, ButtonStyle::Secondary) {

    m_winnerIndex = m_match->winner();
    if (m_winnerIndex == 0) {
        m_winnerName = m_match->playerA()->name();
    } else {
        m_winnerName = m_match->playerB()->name();
    }
    m_scoreP1 = m_match->scoreA();
    m_scoreP2 = m_match->scoreB();

    m_context.gameId = (m_match->rules()->ruleName() == "Par ou Ímpar") ? sl::GameId::OddEven : sl::GameId::Rps;
    m_context.mode = m_match->playerB()->isHuman() ? sl::GameMode::TwoPlayers : sl::GameMode::SinglePlayer;
    m_context.player1Name = m_match->playerA()->name();
    m_context.player2Name = m_match->playerB()->name();
    m_context.skinP1 = m_match->playerA()->skinTone();
    m_context.skinP2 = m_match->playerB()->skinTone();

    setupButtonCallbacks();
}

ResultScene::ResultScene(sl::GameSessionContext ctx, const std::string& winner, int scoreP1, int scoreP2, sl::ScoreStore& store)
    : m_context(ctx),
      m_winnerName(winner),
      m_scoreP1(scoreP1),
      m_scoreP2(scoreP2),
      m_store(store),
      m_trophy(Vector2{640, 215}, 0.95f),
      m_btnPlayAgain("SIM - JOGAR NOVAMENTE [ENTER]", Rectangle{430, 420, 420, 52}, ButtonStyle::Success),
      m_btnBackToMenu("NÃO - VOLTAR AO MENU [ESC]", Rectangle{430, 482, 420, 52}, ButtonStyle::Danger),
      m_btnScores("🏆 Ver Placar", Rectangle{430, 546, 205, 44}, ButtonStyle::Secondary),
      m_btnChangePlayers("👥 Trocar Nomes", Rectangle{645, 546, 205, 44}, ButtonStyle::Secondary) {

    m_winnerIndex = (m_winnerName == m_context.player1Name) ? 0 : 1;
    setupButtonCallbacks();
}

void ResultScene::setupButtonCallbacks() {
    m_btnPlayAgain.setOnClick([this]() {
        onPlayAgain();
    });

    m_btnBackToMenu.setOnClick([this]() {
        onBackToMenu();
    });

    m_btnScores.setOnClick([this]() {
        if (m_manager) {
            int tab = (m_context.gameId == sl::GameId::OddEven) ? 1 : 2;
            m_manager->changeScene(std::make_unique<ScoreScene>(tab, m_winnerName));
        }
    });

    m_btnChangePlayers.setOnClick([this]() {
        if (m_manager) {
            GameType gType = (m_context.gameId == sl::GameId::OddEven) ? GameType::OddEven : GameType::Rps;
            bool is2P = (m_context.mode == sl::GameMode::TwoPlayers);
            m_manager->changeScene(std::make_unique<NameEntryScene>(
                gType,
                is2P,
                m_context.p1IsEven,
                m_context.player1Name,
                m_context.player2Name,
                m_context.skinP1,
                m_context.skinP2
            ));
        }
    });
}

void ResultScene::onEnter() {
    m_particles.spawnConfettiShower(160);

    // Gravação idempotente: só registra uma única vez por partida terminada
    if (!m_scoreRecorded) {
        bool p1Won = (m_winnerName == m_context.player1Name);
        if (p1Won) {
            m_context.sessionWinsP1++;
        } else {
            m_context.sessionWinsP2++;
        }

        m_store.recordMatch(m_context.player1Name, m_context.gameId, p1Won, m_scoreP1, m_scoreP2);

        if (m_context.mode == sl::GameMode::TwoPlayers) {
            m_store.recordMatch(m_context.player2Name, m_context.gameId, !p1Won, m_scoreP2, m_scoreP1);
        }
        m_store.save();
        m_scoreRecorded = true;
    }
}

void ResultScene::onPlayAgain() {
    // [SIM] -> Reinicia partida direta preservando os mesmos jogadores e regras
    if (m_match) {
        m_match->reset();
        auto nextMatch = std::make_unique<MatchScene>(m_match);
        nextMatch->setSessionContext(m_context);
        if (m_manager) {
            m_manager->replaceScene(std::move(nextMatch));
        } else {
            SceneManager::get().replaceScene(std::move(nextMatch));
        }
    } else {
        if (m_manager) {
            m_manager->replaceScene(std::make_unique<MatchScene>(m_context, m_store));
        } else {
            SceneManager::get().replaceScene(std::make_unique<MatchScene>(m_context, m_store));
        }
    }
}

void ResultScene::onBackToMenu() {
    // [NÃO] -> Retorna ao Menu Principal
    m_context.resetSession();
    if (m_manager) {
        m_manager->changeScene(std::make_unique<MenuScene>());
    } else {
        SceneManager::get().switchScene(std::make_unique<MenuScene>());
    }
}

void ResultScene::update(float dt) {
    m_animTimer += dt;
    m_trophy.update(dt);
    m_particles.update(dt);

    // Responsive button bounds update
    Rectangle card = UiLayout::centerCard(680.0f, 600.0f);
    float btnW = 460.0f;
    float btnX = card.x + (card.width - btnW) * 0.5f;

    m_btnPlayAgain.setBounds(Rectangle{btnX, card.y + 382.0f, btnW, 52.0f});
    m_btnBackToMenu.setBounds(Rectangle{btnX, card.y + 444.0f, btnW, 52.0f});

    float subW = (btnW - 16.0f) * 0.5f;
    m_btnScores.setBounds(Rectangle{btnX, card.y + 508.0f, subW, 46.0f});
    m_btnChangePlayers.setBounds(Rectangle{btnX + subW + 16.0f, card.y + 508.0f, subW, 46.0f});

    // Atalhos de Teclado Canônicos
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) {
        onPlayAgain();
        return;
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        onBackToMenu();
        return;
    }

    m_btnPlayAgain.update(dt);
    m_btnBackToMenu.update(dt);
    m_btnScores.update(dt);
    m_btnChangePlayers.update(dt);
}

void ResultScene::draw() {
    Theme::drawBackgroundGradient();

    // Card Modal Centralizado Responsivo (Blueprint 4.5)
    Rectangle card = UiLayout::centerCard(680.0f, 600.0f);
    Theme::drawCard(card, 22.0f, true);

    // Header Title
    const char* title = "🎉 FIM DA PARTIDA! 🎉";
    int titleW = Theme::measureText(title, 28);
    Theme::drawText(title, static_cast<int>(card.x + (card.width - titleW) * 0.5f), static_cast<int>(card.y + 20), 28, Theme::NeonPurple);

    // Winner Announcement
    std::string winText = m_winnerName + " VENCEU O JOGO!";
    int winW = Theme::measureText(winText, 24);
    Theme::drawText(winText, static_cast<int>(card.x + (card.width - winW) * 0.5f), static_cast<int>(card.y + 56), 24, Theme::TextBright);

    // Trophy Centerpiece
    m_trophy.setPosition(Vector2{640.0f, card.y + 165.0f});
    m_trophy.draw();

    // Final Scoreline
    std::string scoreLine = "Placar Final: " + std::to_string(m_scoreP1) + " x " + std::to_string(m_scoreP2);
    int scW = Theme::measureText(scoreLine, 22);
    Theme::drawText(scoreLine, static_cast<int>(card.x + (card.width - scW) * 0.5f), static_cast<int>(card.y + 266), 22, Theme::TextNormal);

    std::string gameName = m_match ? m_match->rules()->ruleName() : ((m_context.gameId == sl::GameId::OddEven) ? "Par ou Ímpar" : "Jokenpô");
    std::string ruleLine = "Modalidade: " + gameName + " (Melhor de 3)";
    int rlW = Theme::measureText(ruleLine, 17);
    Theme::drawText(ruleLine, static_cast<int>(card.x + (card.width - rlW) * 0.5f), static_cast<int>(card.y + 294), 17, Theme::TextMuted);

    // Série na sessão (quando jogou mais de uma partida seguida)
    if (m_context.sessionWinsP1 + m_context.sessionWinsP2 > 1) {
        std::string seriesLine = "Série na sessão: " + m_context.player1Name + " " +
                                 std::to_string(m_context.sessionWinsP1) + " x " +
                                 std::to_string(m_context.sessionWinsP2) + " " +
                                 m_context.player2Name;
        int slW = Theme::measureText(seriesLine, 16);
        Theme::drawText(seriesLine, static_cast<int>(card.x + (card.width - slW) * 0.5f), static_cast<int>(card.y + 316), 16, Theme::TrophyGold);
    }

    // Divider Line
    DrawLine(static_cast<int>(card.x + 40), static_cast<int>(card.y + 338),
             static_cast<int>(card.x + card.width - 40), static_cast<int>(card.y + 338),
             Theme::BorderSubtle);

    // Pergunta Central de Pós-jogo
    const char* question = "QUER JOGAR NOVAMENTE?";
    int qW = Theme::measureText(question, 22);
    Theme::drawText(question, static_cast<int>(card.x + (card.width - qW) * 0.5f), static_cast<int>(card.y + 350), 22, Theme::NeonPurple);

    // Responsive button bounds
    float btnW = 460.0f;
    float btnX = card.x + (card.width - btnW) * 0.5f;

    m_btnPlayAgain.setBounds(Rectangle{btnX, card.y + 382.0f, btnW, 52.0f});
    m_btnBackToMenu.setBounds(Rectangle{btnX, card.y + 444.0f, btnW, 52.0f});

    float subW = (btnW - 16.0f) * 0.5f;
    m_btnScores.setBounds(Rectangle{btnX, card.y + 508.0f, subW, 46.0f});
    m_btnChangePlayers.setBounds(Rectangle{btnX + subW + 16.0f, card.y + 508.0f, subW, 46.0f});

    // Action Buttons
    m_btnPlayAgain.draw();
    m_btnBackToMenu.draw();
    m_btnScores.draw();
    m_btnChangePlayers.draw();

    // Confetti particles
    m_particles.draw();
}

} // namespace Starlight

