#include "ResultScene.hpp"
#include "MatchScene.hpp"
#include "NameEntryScene.hpp"
#include "ScoreScene.hpp"
#include "MenuScene.hpp"
#include "core/SceneManager.hpp"
#include "core/Application.hpp"
#include "ui/Theme.hpp"

namespace Starlight {

ResultScene::ResultScene(std::shared_ptr<Match> match)
    : m_match(std::move(match)),
      m_trophy(Vector2{640, 245}, 1.15f),
      m_btnRematch("Jogar Novamente", Rectangle{350, 465, 280, 52}, ButtonStyle::Primary),
      m_btnChangePlayers("Trocar Jogadores", Rectangle{650, 465, 280, 52}, ButtonStyle::Secondary),
      m_btnScores("Ver Placar", Rectangle{350, 530, 280, 52}, ButtonStyle::Secondary),
      m_btnMenu("Menu Principal", Rectangle{650, 530, 280, 52}, ButtonStyle::Secondary) {

    m_winnerIndex = m_match->winner();
    if (m_winnerIndex == 0) {
        m_winnerName = m_match->playerA()->name();
    } else {
        m_winnerName = m_match->playerB()->name();
    }

    m_btnRematch.setOnClick([this]() {
        if (m_manager) {
            m_match->reset();
            m_manager->changeScene(std::make_unique<MatchScene>(m_match));
        }
    });

    m_btnChangePlayers.setOnClick([this]() {
        if (m_manager) {
            GameType gType = (m_match->rules()->ruleName() == "Par ou Ímpar") ? GameType::OddEven : GameType::Rps;
            bool is2P = m_match->playerB()->isHuman();
            m_manager->changeScene(std::make_unique<NameEntryScene>(
                gType,
                is2P,
                true,
                m_match->playerA()->name(),
                m_match->playerB()->name(),
                m_match->playerA()->skinTone(),
                m_match->playerB()->skinTone()
            ));
        }
    });

    m_btnScores.setOnClick([this]() {
        if (m_manager) {
            int tab = (m_match->rules()->ruleName() == "Par ou Ímpar") ? 1 : 2;
            m_manager->changeScene(std::make_unique<ScoreScene>(tab, m_winnerName));
        }
    });

    m_btnMenu.setOnClick([this]() {
        if (m_manager) {
            m_manager->changeScene(std::make_unique<MenuScene>());
        }
    });
}

void ResultScene::onEnter() {
    m_particles.spawnConfettiShower(160);

    // Registra o resultado uma única vez ao entrar na ResultScene
    if (!m_alreadyRecorded) {
        m_alreadyRecorded = true;
        auto pA = m_match->playerA();
        auto pB = m_match->playerB();
        int winner = m_match->winner();

        sl::GameId gId = (m_match->rules()->ruleName() == "Par ou Ímpar") ? sl::GameId::OddEven : sl::GameId::Rps;
        auto& store = Application::instance().scoreStore();

        if (pA->isHuman()) {
            store.recordMatch(pA->name(), gId, winner == 0, m_match->scoreA(), m_match->scoreB());
        }
        if (pB->isHuman()) {
            store.recordMatch(pB->name(), gId, winner == 1, m_match->scoreB(), m_match->scoreA());
        }
        store.save();
    }
}

void ResultScene::update(float dt) {
    m_animTimer += dt;
    m_trophy.update(dt);
    m_particles.update(dt);

    m_btnRematch.update(dt);
    m_btnChangePlayers.update(dt);
    m_btnScores.update(dt);
    m_btnMenu.update(dt);
}

void ResultScene::draw() {
    Theme::drawBackgroundGradient();

    // Central Card
    Rectangle card = { 280, 50, 720, 600 };
    Theme::drawCard(card, 22.0f, true);

    // Header Title
    const char* title = "FIM DA PARTIDA!";
    int titleW = Theme::measureText(title, 30);
    Theme::drawText(title, static_cast<int>(card.x + (card.width - titleW) * 0.5f), static_cast<int>(card.y + 20), 30, Theme::ACCENT_GOLD);

    // Winner Announcement
    std::string winText = "★ Grande Campeão: " + m_winnerName + " ★";
    int winW = Theme::measureText(winText, 24);
    Theme::drawText(winText, static_cast<int>(card.x + (card.width - winW) * 0.5f), static_cast<int>(card.y + 60), 24, Theme::TEXT_PRIMARY);

    // Trophy
    m_trophy.draw();

    // Final Scoreline
    std::string scoreLine = "Placar Final: " + std::to_string(m_match->scoreA()) + " x " + std::to_string(m_match->scoreB());
    int scW = Theme::measureText(scoreLine, 22);
    Theme::drawText(scoreLine, static_cast<int>(card.x + (card.width - scW) * 0.5f), static_cast<int>(card.y + 375), 22, Theme::TEXT_SECONDARY);

    std::string ruleLine = "Modalidade: " + m_match->rules()->ruleName() + " (Melhor de 3)";
    int rlW = Theme::measureText(ruleLine, 18);
    Theme::drawText(ruleLine, static_cast<int>(card.x + (card.width - rlW) * 0.5f), static_cast<int>(card.y + 410), 18, Theme::TEXT_SECONDARY);

    // Buttons
    m_btnRematch.draw();
    m_btnChangePlayers.draw();
    m_btnScores.draw();
    m_btnMenu.draw();

    // Confetti
    m_particles.draw();
}

} // namespace Starlight
