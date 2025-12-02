/**
 * @file Sayed.h
 * @brief Declarations for 5x5 Tic-Tac-Toe, Word Tic-Tac-Toe, and Infinity Tic-Tac-Toe.
 * @details Contains the specific board and UI classes for these three game variants.
 * @author Sayed (Group Member)
 */

#ifndef SAYED_GAMES_H
#define SAYED_GAMES_H

#include "BoardGame_Classes.h"
#include <vector>
#include <string>
#include <set>
#include <deque>

using namespace std;

// --- 5x5 Tic Tac Toe ---

/**
 * @class XO_5x5_Board
 * @brief Represents a 5x5 Tic-Tac-Toe board.
 * @details The game is played on a larger 5x5 grid. The goal is to have the most 
 * three-in-a-row sequences (horizontal, vertical, or diagonal) when the board is full (24 moves).
 */
class XO_5x5_Board : public Board<char> {
private:
    char blank_symbol = '.'; ///< Symbol representing an empty cell.
    vector<Point> history;   ///< History of moves for undo functionality.
    
    /**
     * @brief Helper function to check if a specific symbol has formed a 3-in-a-row.
     * @param symbol The symbol to check.
     * @return True if at least one sequence exists (simplified check).
     */
    bool check_three_in_row(char symbol) const;
    
public:
    XO_5x5_Board();
    void undoLastMove();
    bool update_board(Move<char>* move) override;
    
    /**
     * @brief Checks if the player has won based on the score count.
     * @details Returns true only if the board is full and the player has more sequences than the opponent.
     */
    bool is_win(Player<char>* player) override;
    
    bool is_lose(Player<char>* player) override { return false; }
    
    /**
     * @brief Checks if the game is a draw (equal scores when full).
     */
    bool is_draw(Player<char>* player) override;
    
    bool game_is_over(Player<char>* player) override;
    
    /**
     * @brief Counts the total number of 3-in-a-row sequences for a given symbol.
     * @param symbol The player's symbol ('X' or 'O').
     * @return The integer count of sequences.
     */
    int count_three_in_row(char symbol) const;
};

/**
 * @class XO_5x5_UI
 * @brief User Interface for the 5x5 Tic-Tac-Toe game.
 */
class XO_5x5_UI : public UI<char> {
private:
    Move<char>* computerMove(Player<char>* aiPlayer);
public:
    XO_5x5_UI();
    ~XO_5x5_UI() override {};
    Move<char>* get_move(Player<char>* player) override;
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
};

// --- Word Tic-Tac-Toe ---

/**
 * @class Word_TTT_Board
 * @brief Represents a Word Tic-Tac-Toe board.
 * @details A 3x3 grid where players place letters to form valid 3-letter English words.
 * Relies on a dictionary file (dic.txt) for validation.
 */
class Word_TTT_Board : public Board<char> {
private:
    char blank_symbol = ' ';
    vector<Point> history;
    set<string> dictionary; ///< Stores the valid words loaded from file.
    
    /**
     * @brief Loads the dictionary from a text file.
     * @param filename Name of the file (usually "dic.txt").
     * @return True if loaded successfully.
     */
    bool load_dictionary(const string& filename);
    
    /**
     * @brief Constructs a string from the board in a specific direction.
     * @param start_r Starting row.
     * @param start_c Starting column.
     * @param dr Row increment.
     * @param dc Column increment.
     * @return The 3-character string formed.
     */
    string get_word(int start_r, int start_c, int dr, int dc) const;
    
public:
    Word_TTT_Board();
    void undoLastMove();
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override { return false; }
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;
    
    /**
     * @brief Checks if any valid word is currently formed on the board.
     * @return True if a valid word exists in any row, column, or diagonal.
     */
    bool check_word_formed() const;
};

/**
 * @class Word_TTT_UI
 * @brief User Interface for Word Tic-Tac-Toe.
 */
class Word_TTT_UI : public UI<char> {
private:
    Move<char>* computerMove(Player<char>* aiPlayer);
public:
    Word_TTT_UI();
    ~Word_TTT_UI() override {};
    Move<char>* get_move(Player<char>* player) override;
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
};

// --- Infinity Tic-Tac-Toe ---

/**
 * @struct InfinityMoveInfo
 * @brief Stores metadata about a move in Infinity Tic-Tac-Toe.
 * @details Used to track which piece was placed and, crucially, if an old piece was removed (and what/where it was) to support undoing.
 */
struct InfinityMoveInfo {
    Point placed;        ///< Location where the new mark was placed.
    bool caused_removal; ///< True if this move caused an old mark to disappear.
    Point removed_point; ///< Location of the removed mark (if applicable).
    char removed_symbol; ///< Symbol of the removed mark (if applicable).
};

/**
 * @class Infinity_TTT_Board
 * @brief Represents an Infinity Tic-Tac-Toe board.
 * @details A 3x3 grid where marks disappear over time. Only the last N moves remain on the board.
 */
class Infinity_TTT_Board : public Board<char> {
private:
    char blank_symbol = ' ';
    deque<Point> move_queue;          ///< queue to track the order of active moves for removal.
    vector<InfinityMoveInfo> history; ///< Detailed history for undo operations.
    
    /**
     * @brief Standard 3x3 win check.
     */
    bool check_win(char symbol) const;
    
public:
    Infinity_TTT_Board();
    void undoLastMove();
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override { return false; }
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;
};

/**
 * @class Infinity_TTT_UI
 * @brief User Interface for Infinity Tic-Tac-Toe.
 */
class Infinity_TTT_UI : public UI<char> {
private:
    Move<char>* computerMove(Player<char>* aiPlayer);
public:
    Infinity_TTT_UI();
    ~Infinity_TTT_UI() override {};
    Move<char>* get_move(Player<char>* player) override;
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
};

#endif