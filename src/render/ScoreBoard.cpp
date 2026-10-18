#include "ScoreBoard.hpp"
#include "ui/Theme.hpp"

namespace Starlight {

ScoreBoard::ScoreBoard()
    : m_nameA("Jogador 1"), m_nameB("Jogador 2") {}

void ScoreBoard::setPlayerA(const std::string& name, int score, Color skinColor) {
    m_nameA = name;
    m_scoreA = score;
    m_skinColorA = skinColor;
}

void ScoreBoard::setPlayerB(const std::string& name, int score, Color skinColor) {
    m_nameB = name;
    m_scoreB = score;
    m_skinColorB = skinColor;
}

void ScoreBoard::setRound(int roundNumber, int targetWins, const std::string& gameTitle) {
    m_roundNumber = roundNumber;
    m_targetWins = targetWins;
    m_gameTitle = gameTitle;
}

void ScoreBoard::draw() const {
    // Left card (Player 1)
    Rectangle cardA = {40, 24, 290, 84};
    Theme::drawCard(cardA, 14.0f, true);

    // Sleeve indicator bar & Skin indicator dot
    DrawRectangleRounded(Rectangle{cardA.x + 8, cardA.y + 16, 6, 52}, 0.5f, 4, Color{52, 112, 215, 255}); // Blue sleeve indicator
    DrawCircle(static_cast<int>(cardA.x + 32), static_cast<int>(cardA.y + 42), 12, m_skinColorA);
    DrawCircleLines(static_cast<int>(cardA.x + 32), static_cast<int>(cardA.y + 42), 12, Color{255, 255, 255, 80});

    // Player A Name
    Theme::drawText(m_nameA, static_cast<int>(cardA.x + 52), static_cast<int>(cardA.y + 20), 20, Theme::TEXT_PRIMARY);

    // Round win circles (● / ○)
    for (int i = 0; i < m_targetWins; ++i) {
        int cx = static_cast<int>(cardA.x + 56 + i * 22);
        int cy = static_cast<int>(cardA.y + 54);
        if (i < m_scoreA) {
            DrawCircle(cx, cy, 7, Theme::ACCENT_GOLD);
        } else {
            DrawCircleLines(cx, cy, 7, Color{255, 255, 255, 90});
        }
    }

    // Score display
    std::string scoreStrA = std::to_string(m_scoreA);
    Theme::drawText(scoreStrA, static_cast<int>(cardA.x + cardA.width - 40), static_cast<int>(cardA.y + 24), 36, Theme::ACCENT_GOLD);

    // Center header (Round & Game title)
    Rectangle cardCenter = {480, 24, 320, 84};
    Theme::drawCard(cardCenter, 14.0f, true);

    int titleW = Theme::measureText(m_gameTitle, 20);
    Theme::drawText(m_gameTitle, static_cast<int>(cardCenter.x + (cardCenter.width - titleW) * 0.5f), static_cast<int>(cardCenter.y + 16), 20, Theme::ACCENT_GOLD);

    std::string roundStr = "Rodada " + std::to_string(m_roundNumber) + " (Primeiro a " + std::to_string(m_targetWins) + ")";
    int roundW = Theme::measureText(roundStr, 16);
    Theme::drawText(roundStr, static_cast<int>(cardCenter.x + (cardCenter.width - roundW) * 0.5f), static_cast<int>(cardCenter.y + 48), 16, Theme::TEXT_SECONDARY);

    // Right card (Player 2)
    Rectangle cardB = {1280 - 40 - 290, 24, 290, 84};
    Theme::drawCard(cardB, 14.0f, true);

    // Sleeve indicator bar & Skin indicator dot
    DrawRectangleRounded(Rectangle{cardB.x + cardB.width - 14, cardB.y + 16, 6, 52}, 0.5f, 4, Color{210, 48, 62, 255}); // Red sleeve indicator
    DrawCircle(static_cast<int>(cardB.x + cardB.width - 32), static_cast<int>(cardB.y + 42), 12, m_skinColorB);
    DrawCircleLines(static_cast<int>(cardB.x + cardB.width - 32), static_cast<int>(cardB.y + 42), 12, Color{255, 255, 255, 80});

    // Score display
    std::string scoreStrB = std::to_string(m_scoreB);
    Theme::drawText(scoreStrB, static_cast<int>(cardB.x + 20), static_cast<int>(cardB.y + 24), 36, Theme::ACCENT_GOLD);

    // Player B Name
    int nameBW = Theme::measureText(m_nameB, 20);
    Theme::drawText(m_nameB, static_cast<int>(cardB.x + cardB.width - 52 - nameBW), static_cast<int>(cardB.y + 20), 20, Theme::TEXT_PRIMARY);

    // Round win circles for Player B
    for (int i = 0; i < m_targetWins; ++i) {
        int cx = static_cast<int>(cardB.x + cardB.width - 56 - i * 22);
        int cy = static_cast<int>(cardB.y + 54);
        if (i < m_scoreB) {
            DrawCircle(cx, cy, 7, Theme::ACCENT_GOLD);
        } else {
            DrawCircleLines(cx, cy, 7, Color{255, 255, 255, 90});
        }
    }
}

} // namespace Starlight
