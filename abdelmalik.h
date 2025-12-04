/**
 * @file abdelmalik.h
 * @brief Declarations for Pyramid, Diamond, and Memory Tic-Tac-Toe games.
 * @details Contains the game logic boards and UI classes for the specified variants.
 * @author Abdelmalik (Group Member)
 */

#ifndef ABDELMALIK_H
#define ABDELMALIK_H

#include "BoardGame_Classes.h"
#include <vector>
#include <string>
#include <utility> // for std::pair

using namespace std;

/**
 * @class Pyramid_X_O_Board
 * @brief Represents a Tic-Tac-Toe variant played on a pyramid-shaped board.
 * @details The board consists of 3 rows: 1 cell on top, 3 in the middle, and 5 at the bottom.
 * Winning requires 3 marks in a line (horizontal, vertical, or diagonal).
 */
class Pyramid_X_O_Board : public Board<char> {
    vector<pair<int, int>> history;
public:
    Pyramid_X_O_Board();
    bool update_board(Move<char>* move) override;
    void undoLastMove(); // Added required function
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override { return false; }
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;
};

class Pyramid_XO_UI : public UI<char> {
    Move<char>* computerMove(Player<char>* player);
public:
    Pyramid_XO_UI();
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    void display_board_matrix(const vector<vector<char>>& matrix) const override;
    Move<char>* get_move(Player<char>* player) override;
};

/**
 * @class Diamond_XO_Board
 * @brief Represents a Tic-Tac-Toe game on a 7x7 diamond-shaped grid.
 * @details Winning condition: Complete a line of 3 marks AND a line of 4 marks simultaneously.
 */
class Diamond_XO_Board : public Board<char> {
    vector<pair<int, int>> history;
public:
    Diamond_XO_Board();
    bool update_board(Move<char>* move) override;
    void undoLastMove(); // Added required function
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override { return false; }
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;
};

class Diamond_XO_UI : public UI<char> {
    Move<char>* computerMove(Player<char>* player);
public:
    Diamond_XO_UI();
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    void display_board_matrix(const vector<vector<char>>& matrix) const override;
    Move<char>* get_move(Player<char>* player) override;
};

/**
 * @class Memory_XO_Board
 * @brief Represents a 3x3 Memory Tic-Tac-Toe board.
 * @details Moves are hidden from view. Players must remember board state.
 * Winning requires 3 marks in a row.
 */
class Memory_XO_Board : public Board<char> {
    vector<pair<int, int>> history;
public:
    Memory_XO_Board();
    bool update_board(Move<char>* move) override;
    void undoLastMove(); // Added required function
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override { return false; }
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;
};

class Memory_XO_UI : public UI<char> {
    Move<char>* computerMove(Player<char>* player);
public:
    Memory_XO_UI();
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    void display_board_matrix(const vector<vector<char>>& matrix) const override;
    Move<char>* get_move(Player<char>* player) override;
};

#endif