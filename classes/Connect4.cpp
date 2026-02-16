#include "Connect4.h"

Connect4::Connect4() : Game()
{
    _grid = new Grid(WIDTH, HEIGHT);
}

Connect4::~Connect4()
{
    delete _grid;
}

Bit* Connect4::PieceForPlayer(int playerNumber)
{
    Bit *bit = new Bit();
    // use red for player 0, yellow for player 1
    bit->LoadTextureFromFile(playerNumber == 0 ? "red.png" : "yellow.png");
    bit->setOwner(getPlayerAt(playerNumber));
    return bit;
}

void Connect4::setUpBoard()
{
    setNumberOfPlayers(2);

    // use descriptive names for the two players
    getPlayerAt(0)->setName("Red");
    getPlayerAt(1)->setName("Yellow");

    _gameOptions.rowX = WIDTH;
    _gameOptions.rowY = HEIGHT;

    _grid->initializeSquares(80, "square.png");

    if (gameHasAI()) {
        setAIPlayer(AI_PLAYER);
    }

    startGame();
}

bool Connect4::dropPieceAtColumn(int column)
{
    if (column < 0 || column >= WIDTH)
        return false;

    // search from bottom row upward for the first empty spot
    for (int row = HEIGHT - 1; row >= 0; row--) {
        ChessSquare *sq = _grid->getSquare(column, row);
        if (sq && !sq->bit()) {
            Bit *bit = PieceForPlayer(getCurrentPlayer()->playerNumber());
            // start above the board so we can animate dropping
            ImVec2 target = sq->getPosition();
            ImVec2 startPos = target;
            startPos.y -= 200; // arbitrary distance above
            bit->setPosition(startPos);
            sq->setBit(bit);
            // animate into place
            bit->moveTo(target);
            endTurn();
            return true;
        }
    }
    return false;
}

bool Connect4::actionForEmptyHolder(BitHolder &holder)
{
    ChessSquare *square = static_cast<ChessSquare *>(&holder);
    int col = square->getColumn();
    return dropPieceAtColumn(col);
}

Player* Connect4::ownerAt(int x, int y) const
{
    ChessSquare *sq = _grid->getSquare(x, y);
    if (!sq || !sq->bit())
        return nullptr;
    return sq->bit()->getOwner();
}

Player* Connect4::checkForWinner()
{
    // look for four in a row in any direction
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            Player *p = ownerAt(x, y);
            if (!p)
                continue;

            const int dirs[4][2] = {{1,0}, {0,1}, {1,1}, {1,-1}};
            for (int di = 0; di < 4; di++) {
                int dx = dirs[di][0];
                int dy = dirs[di][1];
                int count = 1;
                int nx = x;
                int ny = y;
                for (int k = 1; k < 4; k++) {
                    nx += dx;
                    ny += dy;
                    if (nx < 0 || nx >= WIDTH || ny < 0 || ny >= HEIGHT)
                        break;
                    Player *p2 = ownerAt(nx, ny);
                    if (p2 == p)
                        count++;
                    else
                        break;
                }
                if (count >= 4)
                    return p;
            }
        }
    }
    return nullptr;
}

bool Connect4::checkForDraw()
{
    bool full = true;
    _grid->forEachSquare([&full](ChessSquare *square, int x, int y) {
        if (!square->bit())
            full = false;
    });
    if (full && !checkForWinner())
        return true;
    return false;
}

void Connect4::stopGame()
{
    _grid->forEachSquare([](ChessSquare *square, int x, int y) {
        square->destroyBit();
    });
}

std::string Connect4::initialStateString()
{
    return std::string(WIDTH * HEIGHT, '0');
}

std::string Connect4::stateString()
{
    std::string s;
    s.reserve(WIDTH * HEIGHT);
    _grid->forEachSquare([&s](ChessSquare *square, int x, int y) {
        Bit *bit = square->bit();
        if (!bit)
            s += '0';
        else
            s += std::to_string(bit->getOwner()->playerNumber() + 1)[0];
    });
    return s;
}

void Connect4::setStateString(const std::string &s)
{
    if ((int)s.length() != WIDTH * HEIGHT)
        return;

    int idx = 0;
    _grid->forEachSquare([&](ChessSquare *square, int x, int y) {
        char c = s[idx++];
        square->destroyBit();
        if (c == '1' || c == '2') {
            int player = (c - '1');
            Bit *bit = PieceForPlayer(player);
            bit->setPosition(square->getPosition());
            square->setBit(bit);
        }
    });
}

void Connect4::updateAI()
{
    if (!gameHasAI())
        return;

    // simple strategy: drop in first available column
    for (int col = 0; col < WIDTH; col++) {
        // check top cell of column; if empty, it means column has space
        ChessSquare *top = _grid->getSquare(col, 0);
        if (top && !top->bit()) {
            actionForEmptyHolder(*top);
            return;
        }
    }
}
