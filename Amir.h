/**
 * @file Amir.h
 * @brief Declarations for 4x4 Tic-Tac-Toe, SUS, and Ultimate Tic-Tac-Toe games.
 * @details Contains the game logic boards and UI classes for these specific variants.
 * @author Amir (Group Member)
 */

#pragma once
#include "BoardGame_Classes.h"
using namespace std;

// -----------------------------------------------------------------------------
// 4x4 Tic-Tac-Toe
// -----------------------------------------------------------------------------

/**
 * @class XO_4_x_4_Board
 * @brief Represents a 4x4 Tic-Tac-Toe board.
 * @details The game is played on a 4x4 grid. The goal is to get 3 marks in a row (horizontal, vertical, or diagonal).
 */
class XO_4_x_4_Board : public Board<char> {
private:
    char blank_symbol = '.'; ///< Symbol representing an empty cell.
    vector<Point> history;   ///< History of moves for undo functionality.
public:
    XO_4_x_4_Board();
    void undoLastMove();

    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char> *) override {return false;}
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;
};

/**
 * @class XO_4_x_4_UI
 * @brief User Interface for the 4x4 Tic-Tac-Toe game.
 */
class XO_4_x_4_UI : public UI<char> {
private:
    Move<char>* computerMove(Player<char>* aiPlayer);
public:
    XO_4_x_4_UI();
    ~XO_4_x_4_UI() override {};

    Move<char>* get_move(Player<char>*) override;
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
};

// -----------------------------------------------------------------------------
// SUS Game
// -----------------------------------------------------------------------------

/**
 * @class sus_board
 * @brief Represents the board for the SUS game (3x3 grid).
 * @details Players compete to form the sequence "S-U-S" horizontally, vertically, or diagonally.
 * Points are awarded for each sequence formed. The player with the most points wins.
 */
class sus_board: public Board<char> {
    char blank_symbol = '.';
    vector<Point> history;
    int cnt1 = 0; ///< Score counter for Player 1.
    int cnt2 = 0; ///< Score counter for Player 2.
    bool taken[3][3] = {false}; ///< Tracks which sequences have already been counted.
public:
    sus_board();
    void undoLastMove();

    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char> *) override {return false;}
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;
    
    /**
     * @brief Calculates score based on the last move.
     * @param move The move just played.
     */
    void cnt_score(Move<char>* move);
    
    // Getters for AI logic
    char get_cell(int r, int c) const { return board[r][c]; }
    bool is_taken(int r, int c) const { return taken[r][c]; }
    void set_taken(int r, int c, bool val) { taken[r][c] = val; }
};

/**
 * @class sus_ui
 * @brief User Interface for the SUS game.
 */
class sus_ui : public UI<char> {
    Move<char>* computerMove(Player<char>* aiPlayer);
public:
    sus_ui();
    ~sus_ui() override {};

    Move<char>* get_move(Player<char>*) override;
    Player<char>** setup_players() override;
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
};

// -----------------------------------------------------------------------------
// Ultimate Tic-Tac-Toe
// -----------------------------------------------------------------------------

/**
 * @class ultimate_XO_board
 * @brief Represents the Ultimate Tic-Tac-Toe board (9x9 grid).
 * @details The board is a 9x9 grid divided into nine 3x3 "mini-boards".
 * Winning a mini-board marks that board for the player on the global 3x3 scale.
 */
class ultimate_XO_board : public Board<char> {
    char mini_board_wins[3][3]; ///< Tracks the winner ('X', 'O') or draw ('D') status of each mini-board.

public:
    /**
     * @brief Constructs a 9x9 board and initializes mini-board statuses.
     */
    ultimate_XO_board();
    ~ultimate_XO_board();

    /**
     * @brief Validates if a move is legal.
     * @param r Row index (0-8).
     * @param c Column index (0-8).
     * @return True if the move is allowed.
     */
    bool isValidMove(int r, int c) const;

    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char> *) override {return false;}
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

    /**
     * @brief Checks if a specific 3x3 mini-board has been won.
     * @param mini_r The row of the mini-board (0-2).
     * @param mini_c The col of the mini-board (0-2).
     * @return The symbol of the winner ('X', 'O') or 0 if no winner yet.
     */
    char check_mini_board_win(int mini_r, int mini_c);
    
    char get_mini_status(int r, int c) const { return mini_board_wins[r][c]; }
};

/**
 * @class ultimate_XO_UI
 * @brief User Interface for Ultimate Tic-Tac-Toe.
 */
class ultimate_XO_UI : public UI<char> {
private:
    /**
     * @brief Smart AI logic for Ultimate Tic-Tac-Toe.
     * @details Prioritizes winning mini-boards.
     */
    Move<char>* computerMove(Player<char>* aiPlayer);
public:
    ultimate_XO_UI();
    ~ultimate_XO_UI() override = default;
    Move<char>* get_move(Player<char>* player) override;
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    
    /**
     * @brief Displays the 9x9 grid with separators for mini-boards.
     */
    void display_board_matrix(const vector<vector<char>>& matrix) const override;
};