#ifndef PYRAMID_XO_H
#define PYRAMID_XO_H

#include "BoardGame_Classes.h"
#include <vector>
#include <string>

using namespace std;

class Pyramid_X_O_Board : public Board<char> {
public:
    Pyramid_X_O_Board();

    bool update_board(Move<char>* move) override;

    bool is_win(Player<char>* player) override;

    bool is_lose(Player<char>* player) override { return false; }

    bool is_draw(Player<char>* player) override;

    bool game_is_over(Player<char>* player) override;
};

class Pyramid_XO_UI : public UI<char> {
public:
    Pyramid_XO_UI();

    Player<char>* create_player(string& name, char symbol, PlayerType type) override;


    void display_board_matrix(const vector<vector<char>>& matrix) const override;

    Move<char>* get_move(Player<char>* player) override;
};

#endif