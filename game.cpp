#include <iostream>
#include <cctype>
#include <vector>
#include <memory>

class Piece {
public:
    virtual bool isValidMove(int startX, int startY, int endX, int endY, const std::vector<std::vector<std::unique_ptr<Piece>>>& board) const = 0;
    virtual char getSymbol() const = 0;
    virtual ~Piece() = default;
};

class Rook : public Piece {
public:
    char getSymbol() const override { return symbol; }
    bool isValidMove(int startX, int startY, int endX, int endY, const std::vector<std::vector<std::unique_ptr<Piece>>>& board) const override {
        if (startX != endX && startY != endY) return false;
        if (startX == endX) {
            int step = (endY - startY) / std::abs(endY - startY);
            for (int y = startY + step; y != endY; y += step) {
                if (board[startX][y]) return false;
            }
        } else {
            int step = (endX - startX) / std::abs(endX - startX);
            for (int x = startX + step; x != endX; x += step) {
                if (board[x][startY]) return false;
            }
        }
        return true;
    }

    explicit Rook(bool isWhite) : symbol(isWhite ? 'R' : 'r') {}

private:
    char symbol;
};

class Knight : public Piece {
public:
    char getSymbol() const override { return symbol; }
    bool isValidMove(int startX, int startY, int endX, int endY, const std::vector<std::vector<std::unique_ptr<Piece>>>& board) const override {
        int dx = std::abs(endX - startX);
        int dy = std::abs(endY - startY);
        return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
    }

    explicit Knight(bool isWhite) : symbol(isWhite ? 'N' : 'n') {}

private:
    char symbol;
};

class Bishop : public Piece {
public:
    char getSymbol() const override { return symbol; }
    bool isValidMove(int startX, int startY, int endX, int endY, const std::vector<std::vector<std::unique_ptr<Piece>>>& board) const override {
        int dx = std::abs(endX - startX);
        int dy = std::abs(endY - startY);
        if (dx != dy) return false;
        int stepX = (endX - startX) / dx;
        int stepY = (endY - startY) / dy;
        for (int i = 1; i < dx; ++i) {
            if (board[startX + i * stepX][startY + i * stepY]) return false;
        }
        return true;
    }

    explicit Bishop(bool isWhite) : symbol(isWhite ? 'B' : 'b') {}

private:
    char symbol;
};

class Queen : public Piece {
public:
    char getSymbol() const override { return symbol; }
    bool isValidMove(int startX, int startY, int endX, int endY, const std::vector<std::vector<std::unique_ptr<Piece>>>& board) const override {
        int dx = std::abs(endX - startX);
        int dy = std::abs(endY - startY);
        if (dx != dy && startX != endX && startY != endY) return false;
        if (startX == endX) {
            int step = (endY - startY) / std::abs(endY - startY);
            for (int y = startY + step; y != endY; y += step) {
                if (board[startX][y]) return false;
            }
        } else if (startY == endY) {
            int step = (endX - startX) / std::abs(endX - startX);
            for (int x = startX + step; x != endX; x += step) {
                if (board[x][startY]) return false;
            }
        } else {
            int stepX = (endX - startX) / dx;
            int stepY = (endY - startY) / dy;
            for (int i = 1; i < dx; ++i) {
                if (board[startX + i * stepX][startY + i * stepY]) return false;
            }
        }
        return true;
    }

    explicit Queen(bool isWhite) : symbol(isWhite ? 'Q' : 'q') {}

private:
    char symbol;
};

class King : public Piece {
public:
    char getSymbol() const override { return symbol; }
    bool isValidMove(int startX, int startY, int endX, int endY, const std::vector<std::vector<std::unique_ptr<Piece>>>& board) const override {
        return std::abs(endX - startX) <= 1 && std::abs(endY - startY) <= 1;
    }

    explicit King(bool isWhite) : symbol(isWhite ? 'K' : 'k') {}

private:
    char symbol;
};

class Pawn : public Piece {
public:
    char getSymbol() const override { return symbol; }
    bool isValidMove(int startX, int startY, int endX, int endY, const std::vector<std::vector<std::unique_ptr<Piece>>>& board) const override {
        int direction = isWhite ? -1 : 1;
        if (startX + direction == endX && startY == endY && !board[endX][endY]) return true;
        if (startX + direction == endX && std::abs(startY - endY) == 1 && board[endX][endY]) return true;
        if (isFirstMove && startX + 2 * direction == endX && startY == endY && !board[endX][endY] && !board[startX + direction][startY]) return true;
        return false;
    }

    explicit Pawn(bool isWhite) : symbol(isWhite ? 'P' : 'p'), isWhite(isWhite), isFirstMove(true) {}

    void move() { isFirstMove = false; }

private:
    char symbol;
    bool isWhite;
    bool isFirstMove;
};

class Board {
public:
    Board() {
        initializeBoard();
    }

    void print() const {
        std::cout << "    ";
        for (int i = 0; i < 8; ++i) std::cout << i << " ";
        std::cout << std::endl;

        for (int row = 0; row < 8; ++row) {
            std::cout << row << " | ";
            for (int col = 0; col < 8; ++col) {
                if (board[row][col]) {
                    std::cout << board[row][col]->getSymbol() << " ";
                } else {
                    std::cout << "_ ";
                }
            }
            std::cout << "| " << row << std::endl;
        }

        std::cout << "    ";
        for (int i = 0; i < 8; ++i) std::cout << i << " ";
        std::cout << std::endl;
    }

    bool movePiece(int startX, int startY, int endX, int endY, bool isWhiteTurn) {
        if (!isWithinBounds(startX, startY, endX, endY) || !board[startX][startY] ||
            (isWhiteTurn && std::islower(board[startX][startY]->getSymbol())) ||
            (!isWhiteTurn && std::isupper(board[startX][startY]->getSymbol())) ||
            (board[endX][endY] && ((isWhiteTurn && std::isupper(board[endX][endY]->getSymbol())) ||
                                   (!isWhiteTurn && std::islower(board[endX][endY]->getSymbol()))))) {
            return false;
        }

        if (board[startX][startY]->isValidMove(startX, startY, endX, endY, board)) {
            if (auto pawn = dynamic_cast<Pawn*>(board[startX][startY].get())) {
                pawn->move();
            }
            board[endX][endY] = std::move(board[startX][startY]);
            board[startX][startY] = nullptr;
            return true;
        }

        return false;
    }

    bool isCheck(bool isWhite) const {
        int kingX, kingY;
        char kingSymbol = isWhite ? 'K' : 'k';

        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                if (board[row][col] && board[row][col]->getSymbol() == kingSymbol) {
                    kingX = row;
                    kingY = col;
                    break;
                }
            }
        }

        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                if (board[row][col] && ((isWhite && std::islower(board[row][col]->getSymbol())) ||
                                        (!isWhite && std::isupper(board[row][col]->getSymbol()))) &&
                    board[row][col]->isValidMove(row, col, kingX, kingY, board)) {
                    return true;
                }
            }
        }

        return false;
    }

    bool isCheckmate(bool isWhite) {
        if (!isCheck(isWhite)) return false;

        for (int startX = 0; startX < 8; ++startX) {
            for (int startY = 0; startY < 8; ++startY) {
                if (board[startX][startY] && ((isWhite && std::isupper(board[startX][startY]->getSymbol())) ||
                                              (!isWhite && std::islower(board[startX][startY]->getSymbol())))) {
                    for (int endX = 0; endX < 8; ++endX) {
                        for (int endY = 0; endY < 8; ++endY) {
                            std::unique_ptr<Piece> temp = nullptr;
                            if (board[startX][startY]->isValidMove(startX, startY, endX, endY, board)) {
                                temp = std::move(board[endX][endY]);
                                board[endX][endY] = std::move(board[startX][startY]);
                                board[startX][startY] = nullptr;

                                if (!isCheck(isWhite)) {
                                    board[startX][startY] = std::move(board[endX][endY]);
                                    board[endX][endY] = std::move(temp);
                                    return false;
                                }

                                board[startX][startY] = std::move(board[endX][endY]);
                                board[endX][endY] = std::move(temp);
                            }
                        }
                    }
                }
            }
        }

        return true;
    }

    bool isStalemate(bool isWhite) const {
        for (int startX = 0; startX < 8; ++startX) {
            for (int startY = 0; startY < 8; ++startY) {
                if (board[startX][startY] && ((isWhite && std::isupper(board[startX][startY]->getSymbol())) ||
                                              (!isWhite && std::islower(board[startX][startY]->getSymbol())))) {
                    for (int endX = 0; endX < 8; ++endX) {
                        for (int endY = 0; endY < 8; ++endY) {
                            if (board[startX][startY]->isValidMove(startX, startY, endX, endY, board)) {
                                return false;
                            }
                        }
                    }
                }
            }
        }

        return true;
    }

private:
    std::vector<std::vector<std::unique_ptr<Piece>>> board;

    void initializeBoard() {
        board.resize(8, std::vector<std::unique_ptr<Piece>>(8, nullptr));

        // Initialize pieces
        for (int i = 0; i < 8; ++i) {
            board[1][i] = std::make_unique<Pawn>(false);
            board[6][i] = std::make_unique<Pawn>(true);
        }

        board[0][0] = std::make_unique<Rook>(false);
        board[0][7] = std::make_unique<Rook>(false);
        board[7][0] = std::make_unique<Rook>(true);
        board[7][7] = std::make_unique<Rook>(true);

        board[0][1] = std::make_unique<Knight>(false);
        board[0][6] = std::make_unique<Knight>(false);
        board[7][1] = std::make_unique<Knight>(true);
        board[7][6] = std::make_unique<Knight>(true);

        board[0][2] = std::make_unique<Bishop>(false);
        board[0][5] = std::make_unique<Bishop>(false);
        board[7][2] = std::make_unique<Bishop>(true);
        board[7][5] = std::make_unique<Bishop>(true);

        board[0][3] = std::make_unique<Queen>(false);
        board[7][3] = std::make_unique<Queen>(true);

        board[0][4] = std::make_unique<King>(false);
        board[7][4] = std::make_unique<King>(true);
    }

    bool isWithinBounds(int startX, int startY, int endX, int endY) const {
        return startX >= 0 && startX < 8 && startY >= 0 && startY < 8 && endX >= 0 && endX < 8 && endY >= 0 && endY < 8;
    }
};

class ChessGame {
public:
    void play() {
        bool isWhiteTurn = true;
        while (true) {
            board.print();
            if (board.isCheckmate(isWhiteTurn)) {
                std::cout << (isWhiteTurn ? "Black" : "White") << " wins by checkmate!\n";
                break;
            }

            if (board.isStalemate(isWhiteTurn)) {
                std::cout << "Stalemate! It's a draw.\n";
                break;
            }

            std::cout << (isWhiteTurn ? "White's" : "Black's") << " turn\n";
            int startX, startY, endX, endY;
            std::cout << "Enter the position of the piece you want to move (row and column): ";
            std::cin >> startX >> startY;
            std::cout << "Enter the position you want to move the piece to (row and column): ";
            std::cin >> endX >> endY;

            if (board.movePiece(startX, startY, endX, endY, isWhiteTurn)) {
                isWhiteTurn = !isWhiteTurn;
            } else {
                std::cout << "Invalid move. Try again.\n";
            }
        }
    }

private:
    Board board;
};

int main() {
    ChessGame game;
    game.play();
    return 0;
}
