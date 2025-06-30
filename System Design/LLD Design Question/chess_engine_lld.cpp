#include <iostream>
#include <vector>
#include <memory>
using namespace std;

enum class Color { WHITE, BLACK };

/// Abstract Movement Strategy
class MoveStrategy {
public:
    virtual bool isValid(int sx, int sy, int dx, int dy) = 0;
    virtual ~MoveStrategy() = default;
};

/// Strategy for Rook
class RookMove : public MoveStrategy {
public:
    bool isValid(int sx, int sy, int dx, int dy) override {
        return sx == dx || sy == dy;
    }
};

/// Strategy for Bishop
class BishopMove : public MoveStrategy {
public:
    bool isValid(int sx, int sy, int dx, int dy) override {
        return abs(sx - dx) == abs(sy - dy);
    }
};

/// Strategy for King
class KingMove : public MoveStrategy {
public:
    bool isValid(int sx, int sy, int dx, int dy) override {
        return max(abs(sx - dx), abs(sy - dy)) == 1;
    }
};

/// Cell on Board
class Piece; // forward declaration

class Cell {
public:
    int x, y;
    Piece* piece = nullptr;

    Cell(int x, int y) : x(x), y(y) {}
};

/// Abstract Piece class
class Piece {
protected:
    Color color;
    unique_ptr<MoveStrategy> strategy;

public:
    Piece(Color color) : color(color) {}
    virtual ~Piece() = default;

    Color getColor() const { return color; }

    virtual bool canMove(int sx, int sy, int dx, int dy, vector<vector<Cell>>& board) {
        return strategy->isValid(sx, sy, dx, dy);
    }

    virtual char getSymbol() const = 0;
};

/// Concrete Pieces
class Rook : public Piece {
public:
    Rook(Color color) : Piece(color) {
        strategy = make_unique<RookMove>();
    }
    char getSymbol() const override { return color == Color::WHITE ? 'R' : 'r'; }
};

class Bishop : public Piece {
public:
    Bishop(Color color) : Piece(color) {
        strategy = make_unique<BishopMove>();
    }
    char getSymbol() const override { return color == Color::WHITE ? 'B' : 'b'; }
};

class King : public Piece {
public:
    King(Color color) : Piece(color) {
        strategy = make_unique<KingMove>();
    }
    char getSymbol() const override { return color == Color::WHITE ? 'K' : 'k'; }
};

/// Board class
class Board {
public:
    vector<vector<Cell>> grid;

    Board() {
        for (int i = 0; i < 8; ++i) {
            vector<Cell> row;
            for (int j = 0; j < 8; ++j)
                row.emplace_back(i, j);
            grid.push_back(row);
        }
    }

    void placePiece(Piece* piece, int x, int y) {
        grid[x][y].piece = piece;
    }

    bool movePiece(int sx, int sy, int dx, int dy) {
        auto& src = grid[sx][sy];
        auto& dest = grid[dx][dy];

        if (!src.piece) return false;

        if (src.piece->canMove(sx, sy, dx, dy, grid)) {
            dest.piece = src.piece;
            src.piece = nullptr;
            return true;
        }

        return false;
    }

    void printBoard() {
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (grid[i][j].piece)
                    cout << grid[i][j].piece->getSymbol() << " ";
                else
                    cout << ". ";
            }
            cout << "\n";
        }
    }
};

/// Game Controller
class ChessGame {
    Board board;
    Color turn = Color::WHITE;

public:
    void start() {
        // Place few pieces
        board.placePiece(new Rook(Color::WHITE), 0, 0);
        board.placePiece(new Bishop(Color::WHITE), 0, 2);
        board.placePiece(new King(Color::WHITE), 0, 4);
        board.placePiece(new King(Color::BLACK), 7, 4);

        board.printBoard();
    }

    void move(int sx, int sy, int dx, int dy) {
        if (board.movePiece(sx, sy, dx, dy)) {
            cout << "Move successful\n";
            turn = (turn == Color::WHITE) ? Color::BLACK : Color::WHITE;
        } else {
            cout << "Invalid move!\n";
        }

        board.printBoard();
    }
};

int main() {
    ChessGame game;
    game.start();

    // Sample moves
    game.move(0, 0, 3, 0); // Rook moves
    game.move(0, 2, 2, 4); // Bishop moves
    game.move(0, 4, 1, 4); // King moves
    return 0;
}
