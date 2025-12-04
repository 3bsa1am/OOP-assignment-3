#pragma once
#include "BoardGame_Classes.h"
using namespace std;

class XO_4_x_4_Board : public Board<char> {
private:
    char blank_symbol = '.';
    vector<Point> history;
public:
    XO_4_x_4_Board();
    void undoLastMove();

    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player)override;
    bool is_lose(Player<char> *) override {return false;}
    bool is_draw(Player<char>* player)override;
    bool game_is_over(Player<char>* player)override;

};

class XO_4_x_4_UI : public UI<char> {
private:
    Move<char>* computerMove(Player<char>* aiPlayer);
public:
    XO_4_x_4_UI();
    ~XO_4_x_4_UI() override {};

    Move<char>* get_move(Player<char>*)override;
    Player<char>* create_player(string& name, char symbol, PlayerType type)override;
};
/////////// sus game
class sus_board: public Board<char> {
    char blank_symbol = '.';
    vector<Point> history;
    int cnt1 = 0;
    int cnt2 = 0;
    bool taken[3][3] = {false};
public:
    sus_board();
    void undoLastMove();

    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player)override;
    bool is_lose(Player<char> *) override {return false;}
    bool is_draw(Player<char>* player)override;
    bool game_is_over(Player<char>* player)override;
    void cnt_score(Move<char>*move);

};

class sus_ui : public UI<char> {
    Move<char>* computerMove(Player<char>* aiPlayer);
public:
    sus_ui();
    ~sus_ui() override {};

    Move<char>* get_move(Player<char>*)override;
    Player<char>** setup_players() override;
    Player<char>* create_player(string& name, char symbol, PlayerType type)override;
};

//////////////////////////////////////////////////////////////

class ultimate_XO_board : public Board<char>{

    int currentBigRow = -1 ;
    int currentBigCol = -1 ;
    int N_moves = 0;
    char globalBoard[3][3];
public:
    ultimate_XO_board();
    ~ultimate_XO_board();

    bool isValidMove(int r, int c) const;
    bool is_win_small(Move<char>* move) ;
    bool update_board(Move<char>* move)override;
    void update_board2(char symbol);
    bool is_win(Player<char>* player)override;
    bool is_lose(Player<char> *) override {return false;}
    bool is_draw(Player<char>* player)override;
    bool game_is_over(Player<char>* player)override;
    void reset_small_board();
    void display_global_board() const;
    bool small_board_is_draw();



    char get_small_cell(int r, int c) ;
    char get_global_cell(int r, int c) ;
    bool is_small_board_available(int br, int bc) ;

    // current big cell setters/getters
    void set_big_cell_Row(int r) { currentBigRow = r; }
    void set_big_cell_Col(int c) { currentBigCol = c; }
    int get_big_cell_Row() const { return currentBigRow; }
    int get_big_cell_Col() const { return currentBigCol; }



};

class ultimate_XO_UI : public UI<char> {
public:
    ultimate_XO_UI();
    ~ultimate_XO_UI() override = default;
    Move<char>* get_move(Player<char>* player)override;
    Player<char>* create_player(string& name, char symbol, PlayerType type)override;
};
