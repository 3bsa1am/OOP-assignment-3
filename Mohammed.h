#ifndef GAME_H
#define GAME_H
#include "BoardGame_Classes.h"
#include <iostream>
#include <vector>
#include <string>
#include <map> 
using namespace std;

struct Point {
    int r, c;
};

class TTT_Board : public Board<char> {
protected:
    vector<Point> history;
    bool isValidMove(int r, int c) const;
    bool checkWin(char s);
    bool isFull() const;
public:
    TTT_Board();
    void undoLastMove();
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* p) override;
    bool is_lose(Player<char>* p) override;
    bool is_draw(Player<char>* p) override;
    bool game_is_over(Player<char>* p) override;
    int get_history_size() const { return history.size(); }
};

class Misere_TTT_Board : public TTT_Board {
public:
    bool is_win(Player<char>* p) override;
    bool is_lose(Player<char>* p) override;
    bool is_draw(Player<char>* p) override;
    bool game_is_over(Player<char>* p) override;
};

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

class Numerical_TTT_Board : public Board<char> {
protected:
    vector<Point> history;
    map<int, bool> odd_nums_used;
    map<int, bool> even_nums_used;

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

class TTT_UI : 
public UI<char> {
    Move<char>* computerMove(Player<char>* aiPlayer);

public:
    TTT_UI();
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* p) override;
};

class Misere_TTT_UI : 
public UI<char> {
    Move<char>* computerMove(Player<char>* aiPlayer);
public:
    Misere_TTT_UI();
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* p) override;
};

class C4_UI : 
public UI<char> {
    Move<char>* computerMove(Player<char>* aiPlayer);

public:
    C4_UI();
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* p) override;
};

class Numerical_TTT_UI :
public UI<char> {
    Move<char>* computerMove(Player<char>* aiPlayer);

public:
    Numerical_TTT_UI();
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* p) override;
};

class Obstacles_TTT_UI :
public UI<char> {
    Move<char>* computerMove(Player<char>* aiPlayer);

public:
    Obstacles_TTT_UI();
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* p) override;
};


#endif