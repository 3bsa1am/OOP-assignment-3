#include <iostream>
#include <iomanip>
#include <algorithm> 
#include "Pyramid_XO.h"

using namespace std;


Pyramid_X_O_Board::Pyramid_X_O_Board() : Board(3, 5) {

  
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            board[i][j] = 0;
        }
    }
}

bool Pyramid_X_O_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char symbol = move->get_symbol();


    if (x < 0 || x >= rows || y < 0 || y >= columns) {
        return false;
    }


    bool isValidShape = false;
    if (x == 0 && y == 2) isValidShape = true;
    else if (x == 1 && (y >= 1 && y <= 3)) isValidShape = true;
    else if (x == 2 && (y >= 0 && y <= 4)) isValidShape = true;

    if (!isValidShape) return false;


    if (board[x][y] != 0) {
        return false;
    }


    board[x][y] = symbol;
    n_moves++;
    return true;
}

bool Pyramid_X_O_Board::is_win(Player<char>* player) {
    char s = player->get_symbol();

    auto check = [&](int r1, int c1, int r2, int c2, int r3, int c3) {
        return (board[r1][c1] == s && board[r2][c2] == s && board[r3][c3] == s);
        };


    if (check(0, 2, 1, 2, 2, 2)) return true;


    if (check(1, 1, 1, 2, 1, 3)) return true;


    if (check(2, 0, 2, 1, 2, 2)) return true;
    if (check(2, 1, 2, 2, 2, 3)) return true;
    if (check(2, 2, 2, 3, 2, 4)) return true;


    if (check(0, 2, 1, 3, 2, 4)) return true;


    if (check(0, 2, 1, 1, 2, 0)) return true;

    return false;
}

bool Pyramid_X_O_Board::is_draw(Player<char>* player) {

    return (n_moves == 9 && !is_win(player));
}

bool Pyramid_X_O_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}



Pyramid_XO_UI::Pyramid_XO_UI() : UI<char>("Welcome to Pyramid Tic-Tac-Toe!", 3) {}

Player<char>* Pyramid_XO_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";
    return new Player<char>(name, symbol, type);
}

Move<char>* Pyramid_XO_UI::get_move(Player<char>* player) {
    int x, y;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nEnter move for " << player->get_name() << " (row col): ";
        cin >> x >> y;
    }
    else {
        x = rand() % 3; // 0 - 2
        y = rand() % 5; // 0 - 4
    }
    return new Move<char>(x, y, player->get_symbol());
}

void Pyramid_XO_UI::display_board_matrix(const vector<vector<char>>& matrix) const {


    cout << "\n";


    cout << "          "; 
    cout << "| " << (matrix[0][2] == 0 ? '.' : matrix[0][2]) << " |";
    cout << "              (0 , 2)";
    cout << "\n";


    cout << "     "; 
    for (int j = 1; j <= 3; ++j) {
        cout << "| " << (matrix[1][j] == 0 ? '.' : matrix[1][j]) << " |";
    }
    cout << "       (1 , 1 - 3)";
    cout << "\n";


    for (int j = 0; j <= 4; ++j) {
        cout << "| " << (matrix[2][j] == 0 ? '.' : matrix[2][j]) << " |";
    }
    cout << "  (2 , 0 - 4)";
    cout << "\n\n";
}