#include "Game.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

bool TTT_Board::isValidMove(int r, int c) const {
    return r >= 0 && r < rows && c >= 0 && c < columns && board[r][c] == ' ';
}

bool TTT_Board::checkWin(char s) {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == s && board[i][1] == s && board[i][2] == s) return true;
        if (board[0][i] == s && board[1][i] == s && board[2][i] == s) return true;
    }
    if (board[0][0] == s && board[1][1] == s && board[2][2] == s) return true;
    if (board[0][2] == s && board[1][1] == s && board[2][0] == s) return true;
    return false;
}

bool TTT_Board::isFull() const {
    return n_moves >= (rows * columns);
}

TTT_Board::TTT_Board() : Board(3, 3) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            board[i][j] = ' ';
        }
    }
}

void TTT_Board::undoLastMove() {
    if (!history.empty()) {
        Point last = history.back();
        history.pop_back();
        board[last.r][last.c] = ' ';
        n_moves--;
    }
}

bool TTT_Board::update_board(Move<char>* move) {
    int r = move->get_x();
    int c = move->get_y();
    char symbol = move->get_symbol();

    if (r == -1 || c == -1) {
        if (history.size() >= 2) {
            undoLastMove();
            undoLastMove();
            cout << "Undo successful.\n";
        } else if (history.size() == 1) {
            undoLastMove();
            cout << "Undo successful.\n";
        } else {
            cout << "Cannot undo.\n";
        }
        delete move;
        return false;
    }

    if (!isValidMove(r, c)) {
        cout << "Invalid move. Try again.\n";
        delete move;
        return false;
    }

    board[r][c] = symbol;
    history.push_back({r, c});
    n_moves++;
    delete move;
    return true;
}

bool TTT_Board::is_win(Player<char>* p) {
    return checkWin(p->get_symbol());
}

bool TTT_Board::is_lose(Player<char>* p) {
    return false;
}

bool TTT_Board::is_draw(Player<char>* p) {
    return isFull() && !checkWin('X') && !checkWin('O');
}

bool TTT_Board::game_is_over(Player<char>* p) {
    return is_win(p) || is_draw(p);
}

bool Misere_TTT_Board::is_win(Player<char>* p) {
    return false;
}

bool Misere_TTT_Board::is_lose(Player<char>* p) {
    return checkWin(p->get_symbol());
}

bool Misere_TTT_Board::is_draw(Player<char>* p) {
    return isFull() && !checkWin('X') && !checkWin('O');
}

bool Misere_TTT_Board::game_is_over(Player<char>* p) {
    return is_lose(p) || is_draw(p);
}


bool C4_Board::isValidMove(int c) const {
    return c >= 0 && c < columns && board[0][c] == ' ';
}

bool C4_Board::checkWin(char s) {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < columns; c++) {
            if (board[r][c] == s) {
                if (c + 3 < columns && board[r][c + 1] == s && board[r][c + 2] == s && board[r][c + 3] == s) return true;
                if (r + 3 < rows && board[r + 1][c] == s && board[r + 2][c] == s && board[r + 3][c] == s) return true;
                if (r + 3 < rows && c + 3 < columns && board[r + 1][c + 1] == s && board[r + 2][c + 2] == s && board[r + 3][c + 3] == s) return true;
                if (r - 3 >= 0 && c + 3 < columns && board[r - 1][c + 1] == s && board[r - 2][c + 2] == s && board[r - 3][c + 3] == s) return true;
            }
        }
    }
    return false;
}

bool C4_Board::isFull() const {
    return n_moves >= (rows * columns);
}

C4_Board::C4_Board() : Board(6, 7) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            board[i][j] = ' ';
        }
    }
}

void C4_Board::undoLastMove() {
    if (!history.empty()) {
        Point last = history.back();
        history.pop_back();
        board[last.r][last.c] = ' ';
        n_moves--;
    }
}

bool C4_Board::update_board(Move<char>* move) {
    int col = move->get_y();
    char symbol = move->get_symbol();

    if (move->get_x() == -1 || col == -1) {
         if (history.size() >= 2) {
            undoLastMove();
            undoLastMove();
            cout << "Undo successful.\n";
        } else if (history.size() == 1) {
            undoLastMove();
            cout << "Undo successful.\n";
        } else {
            cout << "Cannot undo.\n";
        }
        delete move;
        return false;
    }

    if (!isValidMove(col)) {
        cout << "Invalid move. Try again.\n";
        delete move;
        return false;
    }

    for (int r = rows - 1; r >= 0; --r) {
        if (board[r][col] == ' ') {
            board[r][col] = symbol;
            history.push_back({r, col});
            n_moves++;
            delete move;
            return true;
        }
    }
    
    delete move;
    return false;
}

bool C4_Board::is_win(Player<char>* p) {
    return checkWin(p->get_symbol());
}

bool C4_Board::is_lose(Player<char>* p) {
    return false;
}

bool C4_Board::is_draw(Player<char>* p) {
    return isFull() && !checkWin('X') && !checkWin('O');
}

bool C4_Board::game_is_over(Player<char>* p) {
    return is_win(p) || is_draw(p);
}

Move<char>* TTT_UI::computerMove(Player<char>* aiPlayer) {
    TTT_Board* board = dynamic_cast<TTT_Board*>(aiPlayer->get_board_ptr());
    if (!board) return nullptr;

    char aiSym = aiPlayer->get_symbol();
    char humanSym = (aiSym == 'X') ? 'O' : 'X';
    int rows = board->get_rows();
    int cols = board->get_columns();

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            Move<char>* testMove = new Move<char>(r, c, aiSym);
            if (board->update_board(testMove)) {
                if (board->is_win(aiPlayer)) {
                    board->undoLastMove();
                    return new Move<char>(r, c, aiSym);
                }
                board->undoLastMove();
            } else {
            }
        }
    }

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            Move<char>* testMove = new Move<char>(r, c, humanSym);
            if (board->update_board(testMove)) {
                Player<char> tempHuman("temp", humanSym, PlayerType::HUMAN);
                if (board->is_win(&tempHuman)) {
                    board->undoLastMove();
                    return new Move<char>(r, c, aiSym);
                }
                board->undoLastMove();
            } else {
            }
        }
    }

    int r, c;
    Move<char>* randomMove;
    do {
        r = rand() % rows;
        c = rand() % cols;
        randomMove = new Move<char>(r, c, aiSym);
    } while (!board->update_board(randomMove));
    
    board->undoLastMove();
    return new Move<char>(r, c, aiSym);
}

TTT_UI::TTT_UI() : UI("Welcome to X-O", 3) {}

Player<char>* TTT_UI::create_player(string& name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

Move<char>* TTT_UI::get_move(Player<char>* p) {
    if (p->get_type() == PlayerType::HUMAN) {
        cout << p->get_name() << "'s Turn (" << p->get_symbol() << ").\n";
        cout << "Enter row and col (0-2) or -1 to Undo: ";
        
        int r = -1, c = -1;
        if (!(cin >> r)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Try again.\n";
            return get_move(p);
        }
        if (r != -1) {
            if (!(cin >> c)) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid input. Try again.\n";
                return get_move(p);
            }
        } else {
            c = -1;
        }
        return new Move<char>(r, c, p->get_symbol());

    } else {
        cout << p->get_name() << " (" << p->get_symbol() << ") is thinking...\n";
        return computerMove(p);
    }
}

Misere_TTT_UI::Misere_TTT_UI() : UI("Welcome to Misere X-O", 3) {}

Player<char>* Misere_TTT_UI::create_player(string& name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

Move<char>* Misere_TTT_UI::get_move(Player<char>* p) {
    if (p->get_type() == PlayerType::HUMAN) {
        cout << p->get_name() << "'s Turn (" << p->get_symbol() << ").\n";
        cout << "Enter row and col (0-2) or -1 to Undo: ";
        
        int r = -1, c = -1;
        if (!(cin >> r)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Try again.\n";
            return get_move(p);
        }
        if (r != -1) {
            if (!(cin >> c)) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid input. Try again.\n";
                return get_move(p);
            }
        } else {
            c = -1;
        }
        return new Move<char>(r, c, p->get_symbol());

    } else {
        cout << p->get_name() << " (" << p->get_symbol() << ") is thinking...\n";
        return computerMove(p);
    }
}

Move<char>* Misere_TTT_UI::computerMove(Player<char>* aiPlayer) {
    Misere_TTT_Board* board = dynamic_cast<Misere_TTT_Board*>(aiPlayer->get_board_ptr());
    if (!board) return nullptr;

    char aiSym = aiPlayer->get_symbol();
    int rows = board->get_rows();
    int cols = board->get_columns();

    vector<Point> safeMoves;
    vector<Point> allValidMoves;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            Move<char>* testMove = new Move<char>(r, c, aiSym);
            if (board->update_board(testMove)) {
                allValidMoves.push_back({r, c});
                if (!board->is_lose(aiPlayer)) {
                    safeMoves.push_back({r, c});
                }
                board->undoLastMove();
            } else {
            }
        }
    }

    if (!safeMoves.empty()) {
        Point p = safeMoves[rand() % safeMoves.size()];
        return new Move<char>(p.r, p.c, aiSym);
    } 
    
    if (!allValidMoves.empty()) {
        Point p = allValidMoves[rand() % allValidMoves.size()];
        return new Move<char>(p.r, p.c, aiSym);
    }

    return new Move<char>(0, 0, aiSym); 
}

Move<char>* C4_UI::computerMove(Player<char>* aiPlayer) {
    C4_Board* board = dynamic_cast<C4_Board*>(aiPlayer->get_board_ptr());
    if (!board) return nullptr;

    char aiSym = aiPlayer->get_symbol();
    char humanSym = (aiSym == 'X') ? 'O' : 'X';
    int cols = board->get_columns();

    for (int c = 0; c < cols; c++) {
        Move<char>* testMove = new Move<char>(0, c, aiSym);
        if (board->update_board(testMove)) {
            if (board->is_win(aiPlayer)) {
                board->undoLastMove();
                return new Move<char>(0, c, aiSym);
            }
            board->undoLastMove();
        } else {
        }
    }

    for (int c = 0; c < cols; c++) {
        Move<char>* testMove = new Move<char>(0, c, humanSym);
        if (board->update_board(testMove)) {
            Player<char> tempHuman("temp", humanSym, PlayerType::HUMAN);
            if (board->is_win(&tempHuman)) {
                board->undoLastMove();
                return new Move<char>(0, c, aiSym);
            }
            board->undoLastMove();
        } else {
        }
    }

    int c;
    Move<char>* randomMove;
    do {
        c = rand() % cols;
        randomMove = new Move<char>(0, c, aiSym);
    } while (!board->update_board(randomMove));
    
    board->undoLastMove();
    return new Move<char>(0, c, aiSym);
}

C4_UI::C4_UI() : UI("Welcome to Four in a row", 3) {}

Player<char>* C4_UI::create_player(string& name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

Move<char>* C4_UI::get_move(Player<char>* p) {
    if (p->get_type() == PlayerType::HUMAN) {
        cout << p->get_name() << "'s Turn (" << p->get_symbol() << ").\n";
        cout << "Enter column (0-" << p->get_board_ptr()->get_columns() - 1 << ") or -1 to Undo: ";
        
        int c = -1;
        if (!(cin >> c)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Try again.\n";
            return get_move(p);
        }
        
        int r = (c == -1) ? -1 : 0;
        return new Move<char>(r, c, p->get_symbol());

    } else {
        cout << p->get_name() << " (" << p->get_symbol() << ") is thinking...\n";
        return computerMove(p);
    }
}

int main() {
    srand(time(0));
    
    while (true) {
        int choice;
        cout << "\n Games menu\n";
        cout << "1. X-O (3x3)\n";
        cout << "2. Fourin a row (6x7)\n";
        cout << "3. Misere X-O\n";
        cout << "4. Exit\n";
        cout << "Choose game: ";
        
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        if (choice == 4) {
            cout << "kfaya l3b ro7 zaker\n";
            break;
        }

        if (choice == 1) {
            TTT_Board* board = new TTT_Board();
            UI<char>* ui = new TTT_UI();
            Player<char>** players = ui->setup_players();
            
            GameManager<char> game(board, players, ui);
            game.run();

            delete board;
            delete ui;
            delete players[0];
            delete players[1];
            delete[] players;

        } else if (choice == 2) {
            C4_Board* board = new C4_Board();
            UI<char>* ui = new C4_UI();
            Player<char>** players = ui->setup_players();

            GameManager<char> game(board, players, ui);
            game.run();

            delete board;
            delete ui;
            delete players[0];
            delete players[1];
            delete[] players;

        } else if (choice == 3) {
            Misere_TTT_Board* board = new Misere_TTT_Board();
            UI<char>* ui = new Misere_TTT_UI();
            Player<char>** players = ui->setup_players();
            
            GameManager<char> game(board, players, ui);
            game.run();

            delete board;
            delete ui;
            delete players[0];
            delete players[1];
            delete[] players;

        } else {
            cout << "Invalid choice. Please select 1, 2, 3, or 4.\n";
        }

        cout << "\nPress Enter to return to the main menu...";
        cin.ignore(1000, '\n');
        cin.get();
    }
    return 0;
}