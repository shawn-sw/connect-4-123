#pragma once
#include <string>

namespace ClassGame {
    extern bool gameOver;
    extern std::string gameWinner;

    void GameStartUp();
    void RenderGame();
    void EndOfTurn();
}
