#pragma once
#include "Game.h"

// Connect Four implementation
class Connect4 : public Game
{
public:
    Connect4();
    ~Connect4();

    void        setUpBoard() override;
    Player*     checkForWinner() override;
    bool        checkForDraw() override;
    std::string initialStateString() override;
    std::string stateString() override;
    void        setStateString(const std::string &s) override;
    bool        actionForEmptyHolder(BitHolder &holder) override;
    bool        canBitMoveFrom(Bit &bit, BitHolder &src) override { return false; }
    bool        canBitMoveFromTo(Bit &bit, BitHolder &src, BitHolder &dst) override { return false; }
    void        stopGame() override;

    bool        gameHasAI() override { return true; }
    void        updateAI() override;
    Grid*       getGrid() override { return _grid; }

private:
    Bit*        PieceForPlayer(int playerNumber);
    bool        dropPieceAtColumn(int column);
    Player*     ownerAt(int x, int y) const;

    Grid*       _grid;
    static const int WIDTH = 7;
    static const int HEIGHT = 6;
};
