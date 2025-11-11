#include <iostream>
#include <cctype>
#include "XO_4_x_4.h"

using namespace std;

XO_4_x_4_Board::XO_4_x_4_Board():Board(4,4) {
    for (auto &row : board) {
        for (auto &col : row) {
            col = blank_symbol;
        }
    }
}

bool XO_4_x_4_Board::update_board(Move<char> *move) {
    int x = move->get_x();
    int y = move->get_y();
    char sym = move->get_symbol();

    if (x >= 0 && x < rows && y >= 0 && y < columns && (board[x][y] == blank_symbol || sym == 0)) {
        if (sym == 0) {
            n_moves--;
            board[x][y] = blank_symbol;
        }
        else {
            n_moves++;
            board[x][y] = toupper(sym);
        }
        return true;
    }
    return false;
}

bool XO_4_x_4_Board::is_win(Player<char> *player) {
    char sym = player->get_symbol();
    auto all_equal = [&](char a, char b, char c) {
        return a == b && b == c && a != blank_symbol;
    };

    // Check rows and columns
    for (int i = 0; i < rows; i++) {
        if ((all_equal(board[i][0], board[i][1], board[i][2]) && board[i][0] == sym) ||
            all_equal(board[i][1], board[i][2], board[i][3]) && board[i][1] == sym||
            (all_equal(board[0][i], board[1][i], board[2][i]) && board[0][i] == sym) ||
            all_equal(board[1][i], board[2][i], board[3][i]) && board[1][i] == sym)
            return true;

    }

    // Check diagonals
    for (int i = 0;i<rows;i++) {
        for (int j = 0 ;j<columns;j++) {
            if (i - 2 >= 0 && j + 2 < columns) {
                if (all_equal(board[i][j],board[i-1][j+1],board[i-2][j+2])&&board[i][j] == sym)
                    return true;
            }
            if (i + 2 < rows && j - 2 >= 0) {
                if (all_equal(board[i][j],board[i+1][j-1],board[i+2][j-2])&&board[i][j] == sym)
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
    // Create player based on type
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

Move<char>* XO_4_x_4_UI::get_move(Player<char>* player) {
    int x, y;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nPlease enter your move x and y (0 to 3): ";
        cin >> x >> y;
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        x = rand() % player->get_board_ptr()->get_rows();
        y = rand() % player->get_board_ptr()->get_columns();
    }
    return new Move<char>(x, y, player->get_symbol());
}


