#pragma once
#include "Game.h"

// Connect Four implementation
class Connect4 : public Game
{
public:
    explicit Connect4(bool enableAI = true, int aiPlayerNumber = AI_PLAYER);
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

    bool        gameHasAI() override { return _enableAI; }
    void        updateAI() override;
    Grid*       getGrid() override { return _grid; }

private:
    bool        isAITurn();
    int         currentPlayer();
    int         findBestMove(int depth, int aiPlayer);
    bool        dropInColumn(int column);

    Bit*        PieceForPlayer(int playerNumber);
    bool        dropPieceAtColumn(int column);
    Player*     ownerAt(int x, int y) const;

    Grid*       _grid;
    bool        _enableAI;
    int         _aiPlayerNumber;
    static const int DEPTH = 5;
    static const int WIDTH = 7;
    static const int HEIGHT = 6;
};
