#pragma once
#include <string>
#include "save/ScoreStore.h"

namespace sl {

enum class GameMode {
    SinglePlayer,
    TwoPlayers
};

struct GameSessionContext {
    GameId gameId = GameId::Rps;
    GameMode mode = GameMode::SinglePlayer;
    std::string player1Name = "Jogador 1";
    std::string player2Name = "Jogador 2";
    int skinP1 = 0;
    int skinP2 = 1;
    bool p1IsEven = true;

    // Placar da série atual (quantas partidas inteiras cada um venceu na sessão)
    int sessionWinsP1 = 0;
    int sessionWinsP2 = 0;

    void resetSession() {
        sessionWinsP1 = 0;
        sessionWinsP2 = 0;
    }
};

} // namespace sl

namespace Starlight {
    using GameMode = sl::GameMode;
    using GameSessionContext = sl::GameSessionContext;
}
