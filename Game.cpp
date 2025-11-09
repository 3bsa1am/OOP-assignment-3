#include "Game.h"

Board::Board(int r,int c) : rows(r),cols(c){
    grid.resize(rows,vector<char>(cols,' '));
}

void Board::display() const{
    cout<<"\n  ";
    for(int j = 0;j<cols;++j){
        cout<<"  "<<j<<" ";
    }
    cout<<"\n";

    string sep="  +";
    for(int j=0;j<cols;++j){
        sep+="---+";
    }
    cout<<sep<<"\n";

    for(int i=0;i<rows;++i){
        cout<<i<<" |";
        for(int j=0;j<cols;++j){
            cout<<" "<< grid[i][j]<<" |";
        }
        cout<<"\n"<<sep<<"\n";
    }
    cout<<endl;
}


bool Board::isFull() const{
    return history.size()>=(rows*cols);
}

void Board::undoLastMove(){
    if(!history.empty()){
        Point last=history.back();
        history.pop_back();
        grid[last.r][last.c]=' ';
    }
}

TTTBoard::TTTBoard():Board(3, 3){}

bool TTTBoard::isValidMove(int r, int c) const{
    return r>=0&&r<rows&&c>=0&&c<cols&&grid[r][c]==' ';
}

bool TTTBoard::makeMove(int r,int c,char symbol){
    if(isValidMove(r,c)){
        grid[r][c]=symbol;
        history.push_back({r, c});
        return true;
    }
    return false;
}

bool TTTBoard::checkWin(char s){
    for (int i=0;i<3;i++){
        if(grid[i][0]==s&&grid[i][1]==s&&grid[i][2]==s)return true;
        if(grid[0][i]==s&&grid[1][i]==s&&grid[2][i]==s)return true;
    }
    if(grid[0][0]==s&&grid[1][1]==s&&grid[2][2]==s)return true;
    if(grid[0][2]==s&&grid[1][1]==s&&grid[2][0]==s)return true;
    return false;
}

bool TTTBoard::isDraw(char p1Sym, char p2Sym){
    return isFull()&& !checkWin(p1Sym)&& !checkWin(p2Sym);
}

C4Board::C4Board():Board(6, 7){}

bool C4Board::isValidMove(int ignored_r, int c)const{
    return c>=0&&c<cols&&grid[0][c]==' ';
}

bool C4Board::makeMove(int col,int ignored_r,char symbol){
    if(!isValidMove(0, col))return false;
    
    for(int r=rows-1;r>=0;--r){
        if (grid[r][col]==' '){
            grid[r][col]=symbol;
            history.push_back({r,col});
            return true;
        }
    }
    return false;
}

bool C4Board::checkWin(char s){
    for(int r=0;r<rows;r++){
        for(int c=0;c<cols;c++){
            if(grid[r][c]==s){
                if(c+3<cols&&grid[r][c+1]==s&&grid[r][c+2]==s&&grid[r][c+3]==s)return true;
                if(r+3<rows&&grid[r+1][c]==s&&grid[r+2][c]==s&&grid[r+3][c]==s)return true;
                if(r+3<rows&&c+3<cols&&grid[r+1][c+1]==s&&grid[r+2][c+2]==s&&grid[r+3][c+3]==s)return true;
                if(r-3>=0&&c+3<cols&&grid[r-1][c+1]==s&&grid[r-2][c+2]==s&&grid[r-3][c+3]==s)return true;
            }
        }
    }
    return false;
}

bool C4Board::isDraw(char p1Sym, char p2Sym){
    return isFull()&& !checkWin(p1Sym)&& !checkWin(p2Sym);
}

void computerMove(Board* board, char aiSym, char humanSym, bool isConnect4){
    int rows=board->getRows();
    int cols=board->getCols();

    for(int r=0;r<rows;r++){
        for(int c=0;c<cols;c++){
            if(board->makeMove(isConnect4? c:r, c, aiSym)){
                if (board->checkWin(aiSym))return;
                board->undoLastMove(); 
            }
        }
    }

    for(int r=0;r<rows;r++){
        for(int c=0;c<cols;c++){
            if(board->makeMove(isConnect4? c:r, c, humanSym)){
                if(board->checkWin(humanSym)){
                    board->undoLastMove();
                    board->makeMove(isConnect4? c:r, c, aiSym); 
                    return;
                }
                board->undoLastMove();
            }
        }
    }

    int r, c;
    do{
        r=rand()%rows;
        c=rand()%cols;
    } while(!board->makeMove(isConnect4? c:r, c, aiSym));
}

void runGame(Board* board, PlayerType p1Type, PlayerType p2Type, string p1Name, string p2Name, bool isConnect4) {
    bool p1Turn=true;
    char p1Sym='X';
    char p2Sym='O';
    board->display();

    while(true){
        PlayerType currentType = p1Turn ? p1Type : p2Type;
        string currentName = p1Turn ? p1Name : p2Name;
        char currentSym = p1Turn ? p1Sym : p2Sym;
        char opponentSym = p1Turn ? p2Sym : p1Sym;

        if(currentType==HUMAN){
            cout<<currentName<<"'s Turn (" <<currentSym<< ").\n";
            if(isConnect4)cout<<"Enter column (0-"<<board->getCols()-1<< ") or -1 to Undo: ";
            else cout<<"Enter row and col or -1 to Undo: ";

            int r=-1,c=-1;
            if(isConnect4){
                if(!(cin>>c)){cin.clear();cin.ignore(1000, '\n');continue; }
                 r=0;
            } else{
                if(!(cin>>r)){ 
                    cin.clear(); cin.ignore(1000, '\n'); continue;}
                if(r!=-1&&!(cin>>c)){ 
                    cin.clear();cin.ignore(1000, '\n');continue;}
            }

            if(r==-1||c==-1){
                int movesToUndo=0;
                if(p1Type==HUMAN&&p2Type==HUMAN) movesToUndo = 1;
                else if(p1Type!=p2Type)movesToUndo=2;

                if(board->getMoveCount()>=movesToUndo&&movesToUndo>0){
                    for(int i=0;i<movesToUndo;++i) board->undoLastMove();
                    cout<<"Undo successful.\n";
                    board->display();
                } else{
                    cout<<"Cannot undo.\n";
                }
                continue; 
            }

            if(!board->makeMove(isConnect4? c:r, c, currentSym)){
                cout<<"Invalid move, try again.\n";
                continue;
            }
        } else{
            cout<<currentName<< " (" << currentSym << ") is thinking...\n";
            computerMove(board, currentSym, opponentSym, isConnect4);
        }

        board->display();

        if(board->checkWin(currentSym)){
            cout<<currentName<<" ("<<currentSym<<") Wins!\n";
            break;
        }
        if(board->isDraw(p1Sym,p2Sym)){
            cout<<"It's a Draw!\n";
            break;
        }
        p1Turn=!p1Turn;
    }
}

void getPlayerConfig(PlayerType& p1Type, string& p1Name, PlayerType& p2Type, string& p2Name){
    int choice;
    cout<<"\nConfigure Player 1 (X):\n  1. Human\n  2. Computer\nChoice: ";
    cin>>choice;
    if(choice==1){
        p1Type=HUMAN;
        cout<<"Enter Player 1's name: ";
        cin.ignore(1000, '\n'); 
        getline(cin, p1Name);
    } else{
        p1Type=COMPUTER;
        p1Name="Computer (X)";
    }

    cout<<"Configure Player 2 (O):\n  1. Human\n  2. Computer\nChoice: ";
    cin>>choice;
    if(choice==1){
        p2Type=HUMAN;
        cout<<"Enter Player 2's name: ";
        cin.ignore(1000, '\n'); 
        getline(cin, p2Name);
    } else{
        p2Type=COMPUTER;
        p2Name="Computer (O)";
    }
    cout<<endl;
}

int main() {
    srand(time(0));
    
    while(true){
        int choice;
        cout<<"\n--- Main Menu ---\n";
        cout<<"1. Tic-Tac-Toe (3x3)\n";
        cout<<"2. Connect 4 (6x7)\n";
        cout<<"3. Exit\n";
        cout<<"Choose game: ";
        
        if(!(cin>>choice)){
            cin.clear();
            cin.ignore(1000, '\n');
            cout<<"Invalid input. Please enter a number.\n";
            continue;
        }

        if(choice==3){
            cout<<"Goodbye!\n";
            break;
        }

        PlayerType p1Type, p2Type;
        string p1Name, p2Name;

        if(choice==1) {
            getPlayerConfig(p1Type, p1Name, p2Type, p2Name);
            TTTBoard ttt;
            runGame(&ttt, p1Type, p2Type, p1Name, p2Name, false);
        } else if(choice==2){
            getPlayerConfig(p1Type, p1Name, p2Type, p2Name);
            C4Board c4;
            runGame(&c4, p1Type, p2Type, p1Name, p2Name, true);
        } else{
            cout<<"Invalid choice. Please select 1, 2, or 3.\n";
        }

        cout<<"\nPress Enter to return to the main menu...";
        cin.ignore(1000, '\n');
        cin.get();
    }
    return 0;
}