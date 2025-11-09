#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

enum PlayerType{HUMAN, COMPUTER};

struct Point{int r, c;};

class Board{
protected:
    int rows,cols;
    vector<vector<char>> grid;
    vector<Point> history; 

public:
    Board(int r,int c);
    virtual ~Board() {}

    void display() const;
    bool isFull() const;
    void undoLastMove();
    int getMoveCount() const{ return history.size();}

    virtual bool makeMove(int col_or_row, int col_if_needed, char symbol)=0;
    virtual bool checkWin(char symbol)=0;
    virtual bool isDraw(char p1Sym, char p2Sym)=0;
    virtual bool isValidMove(int r, int c) const=0;
    int getRows() const {return rows;}
    int getCols() const {return cols;}
    char getCell(int r, int c) const{ return grid[r][c];}
};

class TTTBoard:public Board{
public:
    TTTBoard();
    bool makeMove(int r, int c, char symbol) override;
    bool checkWin(char symbol) override;
    bool isDraw(char p1Sym, char p2Sym) override;
    bool isValidMove(int r, int c) const override;
};

class C4Board:public Board {
public:
    C4Board();
    bool makeMove(int col, int ignored_row, char symbol) override;
    bool checkWin(char symbol) override;
    bool isDraw(char p1Sym, char p2Sym) override;
    bool isValidMove(int r, int c) const override; 
};
#endif