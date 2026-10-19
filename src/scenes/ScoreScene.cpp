#include "ScoreScene.hpp"
#include "MenuScene.hpp"
#include "core/SceneManager.hpp"
#include "core/Application.hpp"
#include "ui/Theme.hpp"
#include "ui/UiLayout.h"
#include <iomanip>

namespace Starlight {

ScoreScene::ScoreScene(int initialTab, std::string highlightedPlayer)
    : m_currentTab(initialTab),
      m_highlightedPlayer(std::move(highlightedPlayer)),
      m_btnTabGeral("Geral", Rectangle{280, 125, 170, 44}, ButtonStyle::Primary),
      m_btnTabOddEven("Par ou Ímpar", Rectangle{460, 125, 170, 44}, ButtonStyle::Secondary),
      m_btnTabRps("Jokenpô", Rectangle{640, 125, 170, 44}, ButtonStyle::Secondary),
      m_btnTabFlappy("Starlight Bird", Rectangle{820, 125, 170, 44}, ButtonStyle::Secondary),
      m_btnBack("<- Voltar ao Menu", Rectangle{490, 615, 300, 48}, ButtonStyle::Secondary) {

    m_btnTabGeral.setOnClick([this]() { setTab(0); });
    m_btnTabOddEven.setOnClick([this]() { setTab(1); });
    m_btnTabRps.setOnClick([this]() { setTab(2); });
    m_btnTabFlappy.setOnClick([this]() { setTab(3); });

    m_btnBack.setOnClick([this]() {
        if (m_manager) {
            m_manager->changeScene(std::make_unique<MenuScene>());
        }
    });

    setTab(initialTab);
}

void ScoreScene::setTab(int tab) {
    m_currentTab = tab;
    m_btnTabGeral.setStyle(tab == 0 ? ButtonStyle::Primary : ButtonStyle::Secondary);
    m_btnTabOddEven.setStyle(tab == 1 ? ButtonStyle::Primary : ButtonStyle::Secondary);
    m_btnTabRps.setStyle(tab == 2 ? ButtonStyle::Primary : ButtonStyle::Secondary);
    m_btnTabFlappy.setStyle(tab == 3 ? ButtonStyle::Primary : ButtonStyle::Secondary);
}

void ScoreScene::update(float dt) {
    m_animTimer += dt;
    m_btnTabGeral.update(dt);
    m_btnTabOddEven.update(dt);
    m_btnTabRps.update(dt);
    m_btnTabFlappy.update(dt);
    m_btnBack.update(dt);

    if (IsKeyPressed(KEY_ESCAPE)) {
        if (m_manager) {
            m_manager->changeScene(std::make_unique<MenuScene>());
        }
    }

    // Number keys 1..4 switch tabs
    if (IsKeyPressed(KEY_ONE)) setTab(0);
    if (IsKeyPressed(KEY_TWO)) setTab(1);
    if (IsKeyPressed(KEY_THREE)) setTab(2);
    if (IsKeyPressed(KEY_FOUR)) setTab(3);
}

void ScoreScene::draw() {
    Theme::drawBackgroundGradient();

    // Central Card
    Rectangle card = UiLayout::centerCard(960.0f, 640.0f);
    card.y = 35.0f;
    Theme::drawCard(card, 22.0f, true);

    // Title
    const char* title = "🏆 PLACAR & RANKING DOS JOGOS 🏆";
    int tw = Theme::measureText(title, 26);
    Theme::drawText(title, static_cast<int>(card.x + (card.width - tw) * 0.5f), static_cast<int>(card.y + 18), 26, Theme::NeonPurple);

    const char* sub = "Pontuação Geral: 10 pts por vitória em partida + Recorde do Flappy";
    int sw = Theme::measureText(sub, 16);
    Theme::drawText(sub, static_cast<int>(card.x + (card.width - sw) * 0.5f), static_cast<int>(card.y + 54), 16, Theme::TextMuted);

    // Draw Tab Buttons
    m_btnTabGeral.draw();
    m_btnTabOddEven.draw();
    m_btnTabRps.draw();
    m_btnTabFlappy.draw();

    // Table Container Box
    Rectangle tableBox = { card.x + 30, 180, card.width - 60, 410 };
    Theme::drawCard(tableBox, 14.0f, false, Theme::CardSurfaceDark);

    // Table Header
    int headerY = static_cast<int>(tableBox.y + 14);
    Theme::drawText("POS", static_cast<int>(tableBox.x + 20), headerY, 16, Theme::NeonPurple);
    Theme::drawText("JOGADOR", static_cast<int>(tableBox.x + 90), headerY, 16, Theme::NeonPurple);

    if (m_currentTab == 0) {
        Theme::drawText("VITÓRIAS", static_cast<int>(tableBox.x + 360), headerY, 16, Theme::NeonPurple);
        Theme::drawText("RECORDE BIRD", static_cast<int>(tableBox.x + 540), headerY, 16, Theme::NeonPurple);
        Theme::drawText("PONTOS TOTAIS", static_cast<int>(tableBox.x + 720), headerY, 16, Theme::NeonPurple);
    } else if (m_currentTab == 1 || m_currentTab == 2) {
        Theme::drawText("PARTIDAS (V / D)", static_cast<int>(tableBox.x + 360), headerY, 16, Theme::NeonPurple);
        Theme::drawText("RODADAS (V - D)", static_cast<int>(tableBox.x + 560), headerY, 16, Theme::NeonPurple);
        Theme::drawText("APROVEITAMENTO", static_cast<int>(tableBox.x + 720), headerY, 16, Theme::NeonPurple);
    } else if (m_currentTab == 3) {
        Theme::drawText("MELHOR RECORDE", static_cast<int>(tableBox.x + 400), headerY, 16, Theme::NeonPurple);
        Theme::drawText("TENTATIVAS", static_cast<int>(tableBox.x + 680), headerY, 16, Theme::NeonPurple);
    }

    // Divider
    DrawLine(static_cast<int>(tableBox.x + 10), headerY + 28, static_cast<int>(tableBox.x + tableBox.width - 10), headerY + 28, Theme::BorderSubtle);

    // Fetch Ranking data from ScoreStore
    const auto& store = Application::instance().scoreStore();
    std::vector<sl::PlayerProfile> data;
    if (m_currentTab == 0) {
        data = store.overallRanking();
    } else if (m_currentTab == 1) {
        data = store.ranking(sl::GameId::OddEven);
    } else if (m_currentTab == 2) {
        data = store.ranking(sl::GameId::Rps);
    } else {
        data = store.ranking(sl::GameId::Flappy);
    }

    if (data.empty()) {
        const char* emptyMsg = "Sem registros ainda nesta categoria.";
        int ew = Theme::measureText(emptyMsg, 20);
        Theme::drawText(emptyMsg, static_cast<int>(tableBox.x + (tableBox.width - ew) * 0.5f), static_cast<int>(tableBox.y + 160), 20, Theme::TextNormal);

        const char* emptySub = "Jogue uma partida para inaugurar o placar!";
        int esw = Theme::measureText(emptySub, 16);
        Theme::drawText(emptySub, static_cast<int>(tableBox.x + (tableBox.width - esw) * 0.5f), static_cast<int>(tableBox.y + 195), 16, Theme::TextMuted);
    } else {
        int maxRows = std::min(static_cast<int>(data.size()), 9); // Top 9 to fit comfortably
        int rowStartY = headerY + 38;
        int rowHeight = 36;

        for (int i = 0; i < maxRows; ++i) {
            const auto& p = data[i];
            int rowY = rowStartY + i * rowHeight;
            bool isHighlighted = (!m_highlightedPlayer.empty() && sl::ScoreStore::iequals(p.name, m_highlightedPlayer));

            // Row background highlight
            if (isHighlighted) {
                Rectangle rowRec = { tableBox.x + 8, static_cast<float>(rowY - 4), tableBox.width - 16, static_cast<float>(rowHeight - 2) };
                DrawRectangleRounded(rowRec, 0.2f, 4, Color{108, 43, 217, 60});
                DrawRectangleRoundedLines(rowRec, 0.2f, 4, 1.5f, Theme::BorderGlow);
            } else if (i % 2 == 1) {
                Rectangle rowRec = { tableBox.x + 8, static_cast<float>(rowY - 4), tableBox.width - 16, static_cast<float>(rowHeight - 2) };
                DrawRectangleRounded(rowRec, 0.2f, 4, Color{37, 28, 69, 70});
            }

            // Rank medal / number
            Color rankColor = Theme::TextBright;
            std::string rankStr = std::to_string(i + 1) + "º";
            if (i == 0) rankColor = Theme::TrophyGold;
            else if (i == 1) rankColor = Color{215, 225, 245, 255}; // Silver
            else if (i == 2) rankColor = Color{205, 145, 90, 255};  // Bronze

            Theme::drawText(rankStr, static_cast<int>(tableBox.x + 22), rowY, 18, rankColor);

            // Name
            Color nameColor = isHighlighted ? Theme::NeonPurple : Theme::TextBright;
            Theme::drawText(p.name, static_cast<int>(tableBox.x + 90), rowY, 18, nameColor);

            // Tab specific stats
            if (m_currentTab == 0) {
                int totalWins = p.totalMatchesWon();
                Theme::drawText(std::to_string(totalWins) + " vitórias", static_cast<int>(tableBox.x + 360), rowY, 18, Theme::TextNormal);
                Theme::drawText(std::to_string(p.flappyHighScore) + " canos", static_cast<int>(tableBox.x + 540), rowY, 18, Theme::TextNormal);
                Theme::drawText(std::to_string(p.totalPoints()) + " pts", static_cast<int>(tableBox.x + 720), rowY, 18, Theme::TrophyGold);
            } else if (m_currentTab == 1 || m_currentTab == 2) {
                int wins = (m_currentTab == 1) ? p.oddEvenWins : p.rpsWins;
                int losses = (m_currentTab == 1) ? p.oddEvenLosses : p.rpsLosses;
                int rWon = (m_currentTab == 1) ? p.oddEvenRoundsWon : p.rpsRoundsWon;
                int rLost = (m_currentTab == 1) ? p.oddEvenRoundsLost : p.rpsRoundsLost;

                std::string matchStr = std::to_string(wins) + "V  -  " + std::to_string(losses) + "D";
                Theme::drawText(matchStr, static_cast<int>(tableBox.x + 360), rowY, 18, Theme::TextNormal);

                std::string roundStr = std::to_string(rWon) + " x " + std::to_string(rLost);
                Theme::drawText(roundStr, static_cast<int>(tableBox.x + 560), rowY, 18, Theme::TextNormal);

                int totalMatches = wins + losses;
                int pct = (totalMatches > 0) ? (wins * 100 / totalMatches) : 0;
                Theme::drawText(std::to_string(pct) + "%", static_cast<int>(tableBox.x + 720), rowY, 18, Theme::ActionPlayAgain);
            } else if (m_currentTab == 3) {
                Theme::drawText(std::to_string(p.flappyHighScore) + " canos", static_cast<int>(tableBox.x + 400), rowY, 18, Theme::TrophyGold);
                Theme::drawText(std::to_string(p.flappyAttempts) + " voos", static_cast<int>(tableBox.x + 680), rowY, 18, Theme::TextNormal);
            }
        }
    }

    m_btnBack.draw();
}

} // namespace Starlight
