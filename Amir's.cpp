#include <iostream>
#include <cctype>
#include <vector>
#include <cstdlib>
#include "Amir's.h"

using namespace std;

XO_4_x_4_Board::XO_4_x_4_Board():Board(4,4) {
    for (auto &row : board) {
        for (auto &col : row) {
            col = blank_symbol;
        }
    }
}

void XO_4_x_4_Board::undoLastMove() {
    if (!history.empty()) {
        Point last = history.back();
        history.pop_back();
        board[last.r][last.c] = blank_symbol;
        n_moves--;
    }
}

bool XO_4_x_4_Board::update_board(Move<char> *move) {
    int x = move->get_x();
    int y = move->get_y();
    char sym = move->get_symbol();

    if (x >= 0 && x < rows && y >= 0 && y < columns && board[x][y] == blank_symbol) {
        n_moves++;
        board[x][y] = toupper(sym);
        history.push_back({x, y});
        return true;
    }
    return false;
}

bool XO_4_x_4_Board::is_win(Player<char> *player) {
    char sym = toupper(player->get_symbol());
    const int dx[8] = {0,  0,  1, -1,  1, -1,  1, -1};
    const int dy[8] = {1, -1,  0,  0,  1, -1, -1,  1};
    auto equal = [&](int r, int c) {
        return toupper(board[r][c]) == sym;
    };
    for (int i = 0;i < rows;i++) {
        for (int j = 0;j < columns;j++) {
            for (int d = 0;d < 8 ;d++) {
                int cnt = 0;
                for (int k = 0;k<3;k++) {
                    int nx = i + dx[d]*k;
                    int ny = j + dy[d]*k;
                    if (nx < 0 || ny < 0 || nx >= rows || ny >= columns)
                        break;
                    if (equal(nx,ny))
                        cnt++;
                    else
                        break;
                }
                if (cnt == 3)
                    return true;
            }
        }
    }
    return false;
}

bool XO_4_x_4_Board::is_draw(Player<char> *player) {
    return (n_moves == 16 && !is_win(player));
}

bool XO_4_x_4_Board::game_is_over(Player<char> *player) {
    return is_win(player) || is_draw(player);
}

XO_4_x_4_UI::XO_4_x_4_UI() : UI<char>("Welcome to 4x4 XO Game: ", 4) {}

Player<char>* XO_4_x_4_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

Move<char>* XO_4_x_4_UI::computerMove(Player<char>* aiPlayer) {
    XO_4_x_4_Board* board = dynamic_cast<XO_4_x_4_Board*>(aiPlayer->get_board_ptr());
    if (!board) return nullptr;

    char aiSym = aiPlayer->get_symbol();
    char humanSym = (aiSym == 'X') ? 'O' : 'X';
    int rows = board->get_rows();
    int cols = board->get_columns();

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            Move<char>* testMove = new Move<char>(r, c, aiSym);
            if (board->update_board(testMove)) {
                if (board->is_win(aiPlayer)) {
                    board->undoLastMove();
                    return new Move<char>(r, c, aiSym);
                }
                board->undoLastMove();
            } else {
                delete testMove;
            }
        }
    }

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            Move<char>* testMove = new Move<char>(r, c, humanSym);
            if (board->update_board(testMove)) {
                Player<char> tempHuman("temp", humanSym, PlayerType::HUMAN);
                if (board->is_win(&tempHuman)) {
                    board->undoLastMove();
                    delete testMove;
                    return new Move<char>(r, c, aiSym);
                }
                board->undoLastMove();
            } else {
                delete testMove;
            }
        }
    }

    int r, c;
    Move<char>* randomMove;
    do {
        r = rand() % rows;
        c = rand() % cols;
        randomMove = new Move<char>(r, c, aiSym);
    } while (!board->update_board(randomMove));
    
    board->undoLastMove();
    delete randomMove;
    return new Move<char>(r, c, aiSym);
}

Move<char>* XO_4_x_4_UI::get_move(Player<char>* player) {
    if (player->get_type() == PlayerType::HUMAN) {
        int x, y;
        cout << "\nPlease enter your move x and y (0 to 3): ";
        cin >> x >> y;
        return new Move<char>(x, y, player->get_symbol());
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        cout << player->get_name() << " (" << player->get_symbol() << ") is thinking...\n";
        return computerMove(player);
    }
    return nullptr;
}


