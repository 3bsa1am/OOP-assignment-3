/**
 * @file Mohammed.h
 * @brief Declarations for standard TTT, Misere, Connect 4, Numerical, and Obstacles games.
 * @details Contains the game logic boards and UI classes for the specified variants.
 * @author Mohammed (Group Member)
 */

#ifndef MOHAMMED_H
#define MOHAMMED_H
#include "BoardGame_Classes.h"
#include <iostream>
#include <vector>
#include <string>
#include <map> 
using namespace std;

/**
 * @struct Point
 * @brief Represents a simple 2D coordinate (row, column).
 */
struct Point {
    int r; ///< Row index
    int c; ///< Column index
};

/**
 * @class GameAI
 * @brief Static helper class containing generic AI logic strategies.
 * @details Implements strategies for 2D grid games, column-based games, and Misere variants.
 */
class GameAI {
public:
    /**
     * @brief Determines the best move for 2D grid games using a Win -> Block -> Random strategy.
     * @tparam BoardType The specific board class (must support undoLastMove).
     * @param player Pointer to the AI player.
     * @return A pointer to the selected optimal Move.
     */
    template <typename BoardType>
    static Move<char>* getBestMove_2D(Player<char>* player);

    /**
     * @brief Determines the best move for column-based games (like Connect 4).
     * @tparam BoardType The specific board class.
     * @param player Pointer to the AI player.
     * @return A pointer to the selected optimal Move.
     */
    template <typename BoardType>
    static Move<char>* getBestMove_Column(Player<char>* player);

    /**
     * @brief AI logic for Misere games where the goal is to avoid winning.
     * @tparam BoardType The specific board class.
     * @param player Pointer to the AI player.
     * @return A pointer to a safe Move.
     */
    template <typename BoardType>
    static Move<char>* getMisereMove(Player<char>* player);

    /**
     * @brief Generates a purely random valid move.
     * @tparam BoardType The specific board class.
     * @param player Pointer to the AI player.
     * @return A pointer to a random valid Move.
     */
    template <typename BoardType>
    static Move<char>* getRandomMove(Player<char>* player);
};

// --- Tic-Tac-Toe Classes ---

/**
 * @class TTT_Board
 * @brief Represents a standard 3x3 Tic-Tac-Toe Board.
 */
class TTT_Board : public Board<char> {
protected:
    vector<Point> history; ///< Stores history of moves to support undo operations for AI.
    
    /** @brief Checks if a specific cell is valid for a move. */
    bool isValidMove(int r, int c) const;
    /** @brief Checks if the given symbol has formed a winning line. */
    bool checkWin(char s);
    /** @brief Checks if the board is completely full. */
    bool isFull() const;
public:
    TTT_Board();
    /** @brief Undoes the last move played on the board. */
    void undoLastMove();
    
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* p) override;
    bool is_lose(Player<char>* p) override;
    bool is_draw(Player<char>* p) override;
    bool game_is_over(Player<char>* p) override;
    int get_history_size() const { return history.size(); }
};

/**
 * @class Misere_TTT_Board
 * @brief Variant of Tic-Tac-Toe where the player who completes a line LOSES.
 */
class Misere_TTT_Board : public TTT_Board {
public:
    /** * @brief In Misere, completing a line is NOT a win. 
     * @return Always false.
     */
    bool is_win(Player<char>* p) override;
    
    /** * @brief In Misere, completing a line causes a loss.
     * @return True if the player has 3 in a row.
     */
    bool is_lose(Player<char>* p) override;
    
    bool is_draw(Player<char>* p) override;
    bool game_is_over(Player<char>* p) override;
};

// --- Connect 4 Classes ---

/**
 * @class C4_Board
 * @brief Represents a 6x7 Connect 4 Board.
 * @details Gravity applies; tokens fall to the lowest available row in a column.
 */
class C4_Board : public Board<char> {
    vector<Point> history;
    bool isValidMove(int c) const;
    bool checkWin(char s);
    bool isFull() const;

public:
    C4_Board();
    void undoLastMove();
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* p) override;
    bool is_lose(Player<char>* p) override;
    bool is_draw(Player<char>* p) override;
    bool game_is_over(Player<char>* p) override;
    int get_history_size() const { return history.size(); }
};

// --- Numerical Tic-Tac-Toe Classes ---

/**
 * @class Numerical_TTT_Board
 * @brief A 3x3 Tic-Tac-Toe variant using numbers.
 * @details Player 1 uses odd numbers (1,3,5,7,9), Player 2 uses even numbers (2,4,6,8).
 * Winning condition: Sum of 3 numbers in a line equals 15.
 */
class Numerical_TTT_Board : public Board<char> {
protected:
    vector<Point> history;
    map<int, bool> odd_nums_used;  ///< Tracks which odd numbers have been played.
    map<int, bool> even_nums_used; ///< Tracks which even numbers have been played.

    bool isValidMove(int r, int c, int num, bool is_odd_turn) const;
    bool checkWin() const;
    bool checkLine(int r1, int c1, int r2, int c2, int r3, int c3) const;
    bool isFull() const;

public:
    Numerical_TTT_Board();
    void undoLastMove();
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* p) override;
    bool is_lose(Player<char>* p) override;
    bool is_draw(Player<char>* p) override;
    bool game_is_over(Player<char>* p) override;
    bool public_is_valid(int r, int c, int num, bool is_odd_turn);
    map<int, bool> get_odd_nums_used() const { return odd_nums_used; }
    map<int, bool> get_even_nums_used() const { return even_nums_used; }
    int get_history_size() const { return history.size(); }
};

// --- Obstacles Tic-Tac-Toe Classes ---

/**
 * @class Obstacles_TTT_Board
 * @brief A 6x6 board where random obstacles appear.
 * @details The goal is to get 4 in a row. Some cells are blocked ('#').
 */
class Obstacles_TTT_Board : public Board<char> {
protected:
    vector<Point> history;

    bool isValidMove(int r, int c) const;
    bool checkWin(char s);
    bool isFull() const;
    void addObstacles();

public:
    Obstacles_TTT_Board();
    void undoLastMove();
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* p) override;
    bool is_lose(Player<char>* p) override;
    bool is_draw(Player<char>* p) override;
    bool game_is_over(Player<char>* p) override;
    bool public_is_valid(int r, int c);
    int get_history_size() const { return history.size(); }
};

// --- UI Classes ---

class TTT_UI : public UI<char> {
    Move<char>* computerMove(Player<char>* aiPlayer);
public:
    TTT_UI();
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* p) override;
};

class Misere_TTT_UI : public UI<char> {
    Move<char>* computerMove(Player<char>* aiPlayer);
public:
    Misere_TTT_UI();
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* p) override;
};

class C4_UI : public UI<char> {
    Move<char>* computerMove(Player<char>* aiPlayer);
public:
    C4_UI();
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* p) override;
};

class Numerical_TTT_UI : public UI<char> {
    Move<char>* computerMove(Player<char>* aiPlayer);
public:
    Numerical_TTT_UI();
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* p) override;
};

class Obstacles_TTT_UI : public UI<char> {
    Move<char>* computerMove(Player<char>* aiPlayer);
public:
    Obstacles_TTT_UI();
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* p) override;
};

#endif