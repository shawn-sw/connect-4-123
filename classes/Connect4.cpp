#include "Connect4.h"
#include <limits>
#include <vector>

namespace {
int boardIndex(int x, int y, int width)
{
    return y * width + x;
}

int nextPlayerNumber(int playerNumber)
{
    return 1 - playerNumber;
}

bool boardFull(const std::vector<int> &board)
{
    for (int cell : board) {
        if (cell == 0) {
            return false;
        }
    }
    return true;
}

bool hasFourInARow(const std::vector<int> &board, int width, int height, int playerNumber)
{
    const int token = playerNumber + 1;
    const int dirs[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (board[boardIndex(x, y, width)] != token) {
                continue;
            }

            for (int di = 0; di < 4; di++) {
                int nx = x;
                int ny = y;
                int count = 1;
                const int dx = dirs[di][0];
                const int dy = dirs[di][1];
                for (int k = 1; k < 4; k++) {
                    nx += dx;
                    ny += dy;
                    if (nx < 0 || nx >= width || ny < 0 || ny >= height) {
                        break;
                    }
                    if (board[boardIndex(nx, ny, width)] != token) {
                        break;
                    }
                    count++;
                }
                if (count >= 4) {
                    return true;
                }
            }
        }
    }
    return false;
}

struct BoardOutcome {
    int winnerToken; // 0 = none, 1/2 = player token
    bool draw;
};

BoardOutcome evaluateBoardOutcome(const std::vector<int> &board, int width, int height)
{
    bool full = true;
    const int dirs[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            const int token = board[boardIndex(x, y, width)];
            if (token == 0) {
                full = false;
                continue;
            }

            for (int di = 0; di < 4; di++) {
                int nx = x;
                int ny = y;
                int count = 1;
                const int dx = dirs[di][0];
                const int dy = dirs[di][1];
                for (int k = 1; k < 4; k++) {
                    nx += dx;
                    ny += dy;
                    if (nx < 0 || nx >= width || ny < 0 || ny >= height) {
                        break;
                    }
                    if (board[boardIndex(nx, ny, width)] != token) {
                        break;
                    }
                    count++;
                }
                if (count >= 4) {
                    return {token, false};
                }
            }
        }
    }

    return {0, full};
}

int getDropRow(const std::vector<int> &board, int width, int height, int column)
{
    for (int row = height - 1; row >= 0; row--) {
        if (board[boardIndex(column, row, width)] == 0) {
            return row;
        }
    }
    return -1;
}

bool applyMove(std::vector<int> &board, int width, int height, int column, int playerNumber)
{
    const int row = getDropRow(board, width, height, column);
    if (row < 0) {
        return false;
    }
    board[boardIndex(column, row, width)] = playerNumber + 1;
    return true;
}

int evaluateBoardState(const std::vector<int> &board, int width, int height, int aiPlayer)
{
    const int opponent = nextPlayerNumber(aiPlayer);
    if (hasFourInARow(board, width, height, aiPlayer)) {
        return 100000;
    }
    if (hasFourInARow(board, width, height, opponent)) {
        return -100000;
    }

    // Mild center preference so early moves are less random.
    int score = 0;
    const int center = width / 2;
    for (int y = 0; y < height; y++) {
        int cell = board[boardIndex(center, y, width)];
        if (cell == aiPlayer + 1) {
            score += 3;
        } else if (cell == opponent + 1) {
            score -= 3;
        }
    }
    return score;
}
int negamax(const std::vector<int> &board,
            int width, int height,
            int depth,
            int turnPlayer,
            int aiPlayer,
            int alpha, int beta)
{
    const int opponent = nextPlayerNumber(aiPlayer);

    // terminal / cutoff
    if (depth == 0 ||
        hasFourInARow(board, width, height, turnPlayer) ||
        hasFourInARow(board, width, height, opponent) ||
        boardFull(board)) {
        return evaluateBoardState(board, width, height, turnPlayer);
    }

    int best = std::numeric_limits<int>::min();
    bool foundMove = false;

    for (int col = 0; col < width; col++) {
        std::vector<int> nextBoard = board;
        if (!applyMove(nextBoard, width, height, col, turnPlayer)) {
            continue;
        }
        foundMove = true;

        // Negamax: score = - negamax(next, otherTurn, -beta, -alpha)
        int score = -negamax(nextBoard,
                             width, height,
                             depth - 1,
                             nextPlayerNumber(turnPlayer),
                             aiPlayer,
                             -beta, -alpha);

        best = std::max(best, score);
        alpha = std::max(alpha, best);
        if (alpha >= beta) break; // alpha-beta cutoff
    }

    if (!foundMove) {
        return evaluateBoardState(board, width, height, aiPlayer);
    }
    return best;
}

}

Connect4::Connect4(bool enableAI, int aiPlayerNumber)
    : Game(), _enableAI(enableAI), _aiPlayerNumber(aiPlayerNumber)
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
        if (_aiPlayerNumber < 0 || _aiPlayerNumber >= 2) {
            _aiPlayerNumber = AI_PLAYER;
        }
        setAIPlayer(_aiPlayerNumber);
    }

    startGame();
}

bool Connect4::isAITurn()
{
    return gameHasAI() && getCurrentPlayer()->isAIPlayer();
}

int Connect4::currentPlayer()
{
    return _players.at(_gameOptions.currentTurnNo & 1)->playerNumber();
}

std::vector<int> Connect4::snapshotBoard() const
{
    std::vector<int> board(WIDTH * HEIGHT, 0);
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            Player *owner = ownerAt(x, y);
            if (owner) {
                board[boardIndex(x, y, WIDTH)] = owner->playerNumber() + 1;
            }
        }
    }
    return board;
}

int Connect4::findBestMove(int depth, int aiPlayer)
{
    std::vector<int> board = snapshotBoard();

    int bestCol = -1;
    int bestScore = std::numeric_limits<int>::min();
    for (int col = 0; col < WIDTH; col++) {
        std::vector<int> nextBoard = board;
        if (!applyMove(nextBoard, WIDTH, HEIGHT, col, aiPlayer)) {
            continue;
        }
        int score = -negamax(
            nextBoard,
            WIDTH,
            HEIGHT,
            depth - 1,
            nextPlayerNumber(aiPlayer),
            aiPlayer,
            std::numeric_limits<int>::min(),
            std::numeric_limits<int>::max());
        if (score > bestScore) {
            bestScore = score;
            bestCol = col;
        }
    }
    return bestCol;
}

bool Connect4::dropInColumn(int column)
{
    ChessSquare *top = _grid->getSquare(column, 0);
    if (!top) {
        return false;
    }
    return actionForEmptyHolder(*top);
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
    const std::vector<int> board = snapshotBoard();
    const BoardOutcome outcome = evaluateBoardOutcome(board, WIDTH, HEIGHT);
    if (outcome.winnerToken > 0) {
        return getPlayerAt(outcome.winnerToken - 1);
    }
    return nullptr;
}

bool Connect4::checkForDraw()
{
    const std::vector<int> board = snapshotBoard();
    const BoardOutcome outcome = evaluateBoardOutcome(board, WIDTH, HEIGHT);
    return outcome.draw;
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
    if (!isAITurn())
        return;

    int turnPlayer = currentPlayer();
    int bestCol = findBestMove(DEPTH, turnPlayer);
    if (bestCol >= 0) {
        dropInColumn(bestCol);
    }
}
