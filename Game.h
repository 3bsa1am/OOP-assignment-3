#ifndef GAME_H
#define GAME_H
#include "BoardGame_Classes.h"
#include <iostream>
#include <vector>
#include <string>
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

#endif