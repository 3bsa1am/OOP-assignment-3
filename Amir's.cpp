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
    auto all_equal = [&](char a, char b, char c) {
        return a == b && b == c && a != blank_symbol;
    };

    // Check rows and columns
    for (int i = 0; i < rows; i++) {
        if ((all_equal(board[i][0], board[i][1], board[i][2]) && toupper(board[i][0]) == sym) ||
            (all_equal(board[i][1], board[i][2], board[i][3]) && toupper(board[i][1]) == sym) ||
            (all_equal(board[0][i], board[1][i], board[2][i]) && toupper(board[0][i]) == sym) ||
            (all_equal(board[1][i], board[2][i], board[3][i]) && toupper(board[1][i]) == sym))
            return true;
    }

    // Check main diagonals (top-left to bottom-right)
    // (0,0), (1,1), (2,2)
    if (all_equal(board[0][0], board[1][1], board[2][2]) && toupper(board[0][0]) == sym)
        return true;
    // (1,1), (2,2), (3,3)
    if (all_equal(board[1][1], board[2][2], board[3][3]) && toupper(board[1][1]) == sym)
        return true;
    // (0,1), (1,2), (2,3)
    if (all_equal(board[0][1], board[1][2], board[2][3]) && toupper(board[0][1]) == sym)
        return true;
    // (1,0), (2,1), (3,2)
    if (all_equal(board[1][0], board[2][1], board[3][2]) && toupper(board[1][0]) == sym)
        return true;

    // Check anti-diagonals (top-right to bottom-left)
    // (0,3), (1,2), (2,1)
    if (all_equal(board[0][3], board[1][2], board[2][1]) && toupper(board[0][3]) == sym)
        return true;
    // (1,2), (2,1), (3,0)
    if (all_equal(board[1][2], board[2][1], board[3][0]) && toupper(board[1][2]) == sym)
        return true;
    // (0,2), (1,1), (2,0)
    if (all_equal(board[0][2], board[1][1], board[2][0]) && toupper(board[0][2]) == sym)
        return true;
    if (all_equal(board[1][3], board[2][2], board[3][1]) && toupper(board[1][3]) == sym)
        return true;

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


