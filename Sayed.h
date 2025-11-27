#ifndef SAYED_GAMES_H
#define SAYED_GAMES_H

#include "BoardGame_Classes.h"
#include <vector>
#include <string>
#include <set>
#include <queue>

using namespace std;

// 5x5 Tic Tac Toe

class XO_5x5_Board : public Board<char> {
private:
    char blank_symbol = '.';
    vector<Point> history;
    
    bool check_three_in_row(char symbol) const;
    
public:
    XO_5x5_Board();
    void undoLastMove();
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override { return false; }
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;
    
    int count_three_in_row(char symbol) const;
};

class XO_5x5_UI : public UI<char> {
private:
    Move<char>* computerMove(Player<char>* aiPlayer);
public:
    XO_5x5_UI();
    ~XO_5x5_UI() override {};
    Move<char>* get_move(Player<char>* player) override;
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
};

//  Word Tic-tac-toe

class Word_TTT_Board : public Board<char> {
private:
    char blank_symbol = ' ';
    vector<Point> history;
    set<string> dictionary;
    
    bool load_dictionary(const string& filename);
    string get_word(int start_r, int start_c, int dr, int dc) const;
    
public:
    Word_TTT_Board();
    void undoLastMove();
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override { return false; }
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;
    
    bool check_word_formed() const;
};

class Word_TTT_UI : public UI<char> {
private:
    Move<char>* computerMove(Player<char>* aiPlayer);
public:
    Word_TTT_UI();
    ~Word_TTT_UI() override {};
    Move<char>* get_move(Player<char>* player) override;
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
};

//  Infinity Tic-Tac-Toe

class Infinity_TTT_Board : public Board<char> {
private:
    char blank_symbol = ' ';
    queue<Point> move_history;
    
    bool check_win(char symbol) const;
    
public:
    Infinity_TTT_Board();
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override { return false; }
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;
    
    void remove_oldest_move();
};

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