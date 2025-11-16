#include "Mohammed.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>

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

        if (r == -1) {
            c = -1;
            TTT_Board* board = dynamic_cast<TTT_Board*>(p->get_board_ptr());
            if (board) {
                if (board->get_history_size() >= 2) {
                    board->undoLastMove();
                    board->undoLastMove();
                    cout << "Undo successful.\n";
                } else if (board->get_history_size() == 1) {
                    board->undoLastMove();
                    cout << "Undo successful.\n";
                } else {
                    cout << "Cannot undo.\n";
                }
                display_board_matrix(board->get_board_matrix());
            }
            return get_move(p); 
        }

        if (!(cin >> c)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Try again.\n";
            return get_move(p);
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

        if (r == -1) {
            c = -1;
            TTT_Board* board = dynamic_cast<TTT_Board*>(p->get_board_ptr());
            if (board) {
                if (board->get_history_size() >= 2) {
                    board->undoLastMove();
                    board->undoLastMove();
                    cout << "Undo successful.\n";
                } else if (board->get_history_size() == 1) {
                    board->undoLastMove();
                    cout << "Undo successful.\n";
                } else {
                    cout << "Cannot undo.\n";
                }
                display_board_matrix(board->get_board_matrix());
            }
            return get_move(p); 
        }
        
        if (!(cin >> c)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Try again.\n";
            return get_move(p);
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

C4_UI::C4_UI() : UI("Welcome to Four in a row", 2) {}

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
        
        if (c == -1) {
            C4_Board* board = dynamic_cast<C4_Board*>(p->get_board_ptr());
            if (board) {
                if (board->get_history_size() >= 2) {
                    board->undoLastMove();
                    board->undoLastMove();
                    cout << "Undo successful.\n";
                } else if (board->get_history_size() == 1) {
                    board->undoLastMove();
                    cout << "Undo successful.\n";
                } else {
                    cout << "Cannot undo.\n";
                }
                display_board_matrix(board->get_board_matrix());
            }
            return get_move(p);
        }

        int r = 0;
        return new Move<char>(r, c, p->get_symbol());

    } else {
        cout << p->get_name() << " (" << p->get_symbol() << ") is thinking...\n";
        return computerMove(p);
    }
}

Numerical_TTT_Board::Numerical_TTT_Board() : Board(3, 3) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            board[i][j] = ' ';
        }
    }
    for (int i = 1; i <= 9; i += 2) odd_nums_used[i] = false;
    for (int i = 2; i <= 8; i += 2) even_nums_used[i] = false;
}

bool Numerical_TTT_Board::isValidMove(int r, int c, int num, bool is_odd_turn) const {
    if (r < 0 || r >= rows || c < 0 || c >= columns || board[r][c] != ' ') {
        return false;
    }
    if (num < 1 || num > 9) {
        return false;
    }

    if (is_odd_turn) {
        if (num % 2 == 0 || odd_nums_used.at(num)) return false;
    } else {
        if (num % 2 != 0 || even_nums_used.at(num)) return false;
    }
    return true;
}

bool Numerical_TTT_Board::public_is_valid(int r, int c, int num, bool is_odd_turn) {
    return isValidMove(r, c, num, is_odd_turn);
}

void Numerical_TTT_Board::undoLastMove() {
    if (!history.empty()) {
        Point last = history.back();
        history.pop_back();
        
        char num_char = board[last.r][last.c];
        int num = num_char - '0';
        
        if (num % 2 != 0) {
            odd_nums_used[num] = false;
        } else {
            even_nums_used[num] = false;
        }
        
        board[last.r][last.c] = ' ';
        n_moves--;
    }
}

bool Numerical_TTT_Board::update_board(Move<char>* move) {
    int r = move->get_x();
    int c = move->get_y();
    char num_char = move->get_symbol();
    int num = num_char - '0';

    bool is_odd_turn = (n_moves % 2 == 0);

    if (!isValidMove(r, c, num, is_odd_turn)) {
        cout << "Invalid move. Try again.\n";
        delete move;
        return false;
    }

    board[r][c] = num_char;
    history.push_back({r, c});
    
    if (is_odd_turn) {
        odd_nums_used[num] = true;
    } else {
        even_nums_used[num] = true;
    }
    
    n_moves++;
    delete move;
    return true;
}

bool Numerical_TTT_Board::checkLine(int r1, int c1, int r2, int c2, int r3, int c3) const {
    if (board[r1][c1] == ' ' || board[r2][c2] == ' ' || board[r3][c3] == ' ') {
        return false;
    }
    int v1 = board[r1][c1] - '0';
    int v2 = board[r2][c2] - '0';
    int v3 = board[r3][c3] - '0';
    return (v1 + v2 + v3) == 15;
}

bool Numerical_TTT_Board::checkWin() const {
    if (checkLine(0, 0, 0, 1, 0, 2)) return true;
    if (checkLine(1, 0, 1, 1, 1, 2)) return true;
    if (checkLine(2, 0, 2, 1, 2, 2)) return true;
    if (checkLine(0, 0, 1, 0, 2, 0)) return true;
    if (checkLine(0, 1, 1, 1, 2, 1)) return true;
    if (checkLine(0, 2, 1, 2, 2, 2)) return true;
    if (checkLine(0, 0, 1, 1, 2, 2)) return true;
    if (checkLine(0, 2, 1, 1, 2, 0)) return true;
    
    return false;
}

bool Numerical_TTT_Board::isFull() const {
    return n_moves >= (rows * columns);
}

bool Numerical_TTT_Board::is_win(Player<char>* p) {
    return checkWin();
}

bool Numerical_TTT_Board::is_lose(Player<char>* p) {
    return false;
}

bool Numerical_TTT_Board::is_draw(Player<char>* p) {
    return isFull() && !checkWin();
}

bool Numerical_TTT_Board::game_is_over(Player<char>* p) {
    return is_win(p) || is_draw(p);
}

Obstacles_TTT_Board::Obstacles_TTT_Board() : Board(6, 6) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            board[i][j] = ' ';
        }
    }
}

bool Obstacles_TTT_Board::isValidMove(int r, int c) const {
    return r >= 0 && r < rows && c >= 0 && c < columns && board[r][c] == ' ';
}

bool Obstacles_TTT_Board::public_is_valid(int r, int c) {
    return isValidMove(r, c);
}

bool Obstacles_TTT_Board::isFull() const {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            if (board[i][j] == ' ') return false;
        }
    }
    return true;
}

void Obstacles_TTT_Board::addObstacles() {
    if (isFull()) return;

    int count = 0;
    while (count < 2) {
        if (isFull()) return; 
        int r = rand() % rows;
        int c = rand() % columns;
        if (board[r][c] == ' ') {
            board[r][c] = '#';
            count++;
        }
    }
}

void Obstacles_TTT_Board::undoLastMove() {
    if (!history.empty()) {
        Point last = history.back();
        history.pop_back();
        board[last.r][last.c] = ' ';
        n_moves--;
    }
}

bool Obstacles_TTT_Board::update_board(Move<char>* move) {
    int r = move->get_x();
    int c = move->get_y();
    char symbol = move->get_symbol();

    if (!isValidMove(r, c)) {
        cout << "Invalid move. Try again.\n";
        delete move;
        return false;
    }

    board[r][c] = symbol;
    history.push_back({r, c});
    n_moves++;

    if (n_moves > 0 && n_moves % 2 == 0) {
        cout << "Adding obstacles...\n";
        addObstacles();
    }

    delete move;
    return true;
}

bool Obstacles_TTT_Board::checkWin(char s) {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < columns; c++) {
            if (board[r][c] == s) {
                if (c + 3 < columns && 
                    board[r][c + 1] == s && board[r][c + 2] == s && board[r][c + 3] == s) return true;
                if (r + 3 < rows && 
                    board[r + 1][c] == s && board[r + 2][c] == s && board[r + 3][c] == s) return true;
                if (r + 3 < rows && c + 3 < columns && 
                    board[r + 1][c + 1] == s && board[r + 2][c + 2] == s && board[r + 3][c + 3] == s) return true;
                if (r - 3 >= 0 && c + 3 < columns && 
                    board[r - 1][c + 1] == s && board[r - 2][c + 2] == s && board[r - 3][c + 3] == s) return true;
            }
        }
    }
    return false;
}

bool Obstacles_TTT_Board::is_win(Player<char>* p) {
    return checkWin(p->get_symbol());
}

bool Obstacles_TTT_Board::is_lose(Player<char>* p) {
    return false;
}

bool Obstacles_TTT_Board::is_draw(Player<char>* p) {
    return isFull() && !checkWin('X') && !checkWin('O');
}

bool Obstacles_TTT_Board::game_is_over(Player<char>* p) {
    return is_win(p) || is_draw(p);
}


Numerical_TTT_UI::Numerical_TTT_UI() : UI("Welcome to Numerical X-O (Sum to 15)", 3) {}

Player<char>* Numerical_TTT_UI::create_player(string& name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

Move<char>* Numerical_TTT_UI::get_move(Player<char>* p) {
    if (p->get_type() == PlayerType::HUMAN) {
        bool is_odd_player = (p->get_symbol() == 'X');
        
        Numerical_TTT_Board* board = dynamic_cast<Numerical_TTT_Board*>(p->get_board_ptr());
        string available_nums_str;
        
        if (is_odd_player) {
            available_nums_str = "Odd: ";
            map<int, bool> used = board->get_odd_nums_used();
            for (map<int, bool>::iterator it = used.begin(); it != used.end(); ++it) {
                if (!it->second) { 
                    available_nums_str += to_string(it->first) + ",";
                }
            }
        } else {
            available_nums_str = "Even: ";
            map<int, bool> used = board->get_even_nums_used();
            for (map<int, bool>::iterator it = used.begin(); it != used.end(); ++it) {
                if (!it->second) { 
                    available_nums_str += to_string(it->first) + ",";
                }
            }
        }
        if (available_nums_str.length() > 0 && available_nums_str.back() == ',') {
            available_nums_str.pop_back(); 
        }

        cout << p->get_name() << "'s Turn (" << available_nums_str << ").\n";
        cout << "Enter row (0-2), col (0-2), and number, or -1 to Undo: ";
        
        int r = -1, c = -1, num = -1;
        if (!(cin >> r)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Try again.\n";
            return get_move(p);
        }

        if (r == -1) {
            c = -1;
            num = -1;
            if (board) {
                if (board->get_history_size() >= 2) {
                    board->undoLastMove();
                    board->undoLastMove();
                    cout << "Undo successful.\n";
                } else if (board->get_history_size() == 1) {
                    board->undoLastMove();
                    cout << "Undo successful.\n";
                } else {
                    cout << "Cannot undo.\n";
                }
                display_board_matrix(board->get_board_matrix());
            }
            return get_move(p);
        }

        if (!(cin >> c >> num)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Try again.\n";
            return get_move(p);
        } 

        char num_char = (num == -1) ? ' ' : (num + '0');
        return new Move<char>(r, c, num_char);

    } else {
        cout << p->get_name() << " (" << p->get_symbol() << ") is thinking...\n";
        return computerMove(p);
    }
}

Move<char>* Numerical_TTT_UI::computerMove(Player<char>* aiPlayer) {
    Numerical_TTT_Board* board = dynamic_cast<Numerical_TTT_Board*>(aiPlayer->get_board_ptr());
    if (!board) return nullptr;

    bool is_odd = (aiPlayer->get_symbol() == 'X');
    vector<int> available_nums;
    if (is_odd) {
        for (int i = 1; i <= 9; i += 2) available_nums.push_back(i);
    } else {
        for (int i = 2; i <= 8; i += 2) available_nums.push_back(i);
    }

    int r, c, num;
    char num_char;
    Move<char>* testMove;

    do {
        r = rand() % 3;
        c = rand() % 3;
        num = available_nums[rand() % available_nums.size()];
        num_char = num + '0';
        
    } while (!board->public_is_valid(r, c, num, is_odd));

    return new Move<char>(r, c, num_char);
}

Obstacles_TTT_UI::Obstacles_TTT_UI() : UI("Welcome to Obstacles X-O (4-in-a-row)", 2) {}

Player<char>* Obstacles_TTT_UI::create_player(string& name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

Move<char>* Obstacles_TTT_UI::get_move(Player<char>* p) {
    if (p->get_type() == PlayerType::HUMAN) {
        cout << p->get_name() << "'s Turn (" << p->get_symbol() << ").\n";
        cout << "Enter row and col (0-5) or -1 to Undo: ";
        
        int r = -1, c = -1;
        if (!(cin >> r)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Try again.\n";
            return get_move(p);
        }

        if (r == -1) {
            c = -1;
            Obstacles_TTT_Board* board = dynamic_cast<Obstacles_TTT_Board*>(p->get_board_ptr());
            if (board) {
                if (board->get_history_size() >= 2) {
                    board->undoLastMove();
                    board->undoLastMove();
                    cout << "Undo successful.\n";
                } else if (board->get_history_size() == 1) {
                    board->undoLastMove();
                    cout << "Undo successful.\n";
                } else {
                    cout << "Cannot undo.\n";
                }
                display_board_matrix(board->get_board_matrix());
            }
            return get_move(p);
        }

        if (!(cin >> c)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Try again.\n";
            return get_move(p);
        }
        
        return new Move<char>(r, c, p->get_symbol());

    } else {
        cout << p->get_name() << " (" << p->get_symbol() << ") is thinking...\n";
        return computerMove(p);
    }
}

Move<char>* Obstacles_TTT_UI::computerMove(Player<char>* aiPlayer) {
    Obstacles_TTT_Board* board = dynamic_cast<Obstacles_TTT_Board*>(aiPlayer->get_board_ptr());
    if (!board) return nullptr;

    char aiSym = aiPlayer->get_symbol();

    int r, c;
    do {
        r = rand() % 6;
        c = rand() % 6;
    } while (!board->public_is_valid(r, c));
    
    return new Move<char>(r, c, aiSym);
}