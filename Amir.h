#ifndef XO_4_X_4_H
#define XO_4_X_4_H
#include "BoardGame_Classes.h"
#include "Mohammed.h"
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
    ~XO_4_x_4_UI(){};

    Move<char>* get_move(Player<char>*)override;
    Player<char>* create_player(string& name, char symbol, PlayerType type)override;
};


#endif //XO_4_X_4_H
