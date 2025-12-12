/**
 * @file Mohammed.cpp
 * @brief Implementation details for TTT, Misere, Connect 4, Numerical, and Obstacles games.
 * @details Includes the GameAI logic template specializations and board-specific rule implementations.
 */

#include "Mohammed.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>

using namespace std;

// --- GameAI Implementations ---

template <typename BoardType>
Move<char>* GameAI::getBestMove_2D(Player<char>* player) {
    BoardType* board = dynamic_cast<BoardType*>(player->get_board_ptr());
    if (!board) return nullptr;

    char aiSym = player->get_symbol();
    char humanSym = (aiSym == 'X') ? 'O' : 'X';
    int rows = board->get_rows();
    int cols = board->get_columns();

    // Strategy 1: Check for an immediate win
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            Move<char>* testMove = new Move<char>(r, c, aiSym);
            if (board->update_board(testMove)) {
                if (board->is_win(player)) {
                    board->undoLastMove();
                    return new Move<char>(r, c, aiSym);
                }
                board->undoLastMove();
            }
        }
    }

    // Strategy 2: Check for an immediate block (prevent opponent win)
    Player<char> tempHuman("temp", humanSym, PlayerType::HUMAN);
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            Move<char>* testMove = new Move<char>(r, c, humanSym);
            if (board->update_board(testMove)) {
                if (board->is_win(&tempHuman)) {
                    board->undoLastMove();
                    return new Move<char>(r, c, aiSym);
                }
                board->undoLastMove();
            }
        }
    }

    // Strategy 3: Random move fallback
    return getRandomMove<BoardType>(player);
}

template <typename BoardType>
Move<char>* GameAI::getBestMove_Column(Player<char>* player) {
    BoardType* board = dynamic_cast<BoardType*>(player->get_board_ptr());
    if (!board) return nullptr;

    char aiSym = player->get_symbol();
    char humanSym = (aiSym == 'X') ? 'O' : 'X';
    int cols = board->get_columns();

    // Check Win
    for (int c = 0; c < cols; c++) {
        Move<char>* testMove = new Move<char>(0, c, aiSym);
        if (board->update_board(testMove)) {
            if (board->is_win(player)) {
                board->undoLastMove();
                return new Move<char>(0, c, aiSym);
            }
            board->undoLastMove();
        }
    }

    // Check Block
    Player<char> tempHuman("temp", humanSym, PlayerType::HUMAN);
    for (int c = 0; c < cols; c++) {
        Move<char>* testMove = new Move<char>(0, c, humanSym);
        if (board->update_board(testMove)) {
            if (board->is_win(&tempHuman)) {
                board->undoLastMove();
                return new Move<char>(0, c, aiSym);
            }
            board->undoLastMove();
        }
    }

    // Random
    int c;
    Move<char>* randomMove;
    int attempts = 0;
    do {
        c = rand() % cols;
        randomMove = new Move<char>(0, c, aiSym);
        attempts++;
        if (attempts > 100) return new Move<char>(0, 0, aiSym); // Fail-safe
    } while (!board->update_board(randomMove));
    
    board->undoLastMove();
    return new Move<char>(0, c, aiSym);
}

template <typename BoardType>
Move<char>* GameAI::getMisereMove(Player<char>* player) {
    BoardType* board = dynamic_cast<BoardType*>(player->get_board_ptr());
    if (!board) return nullptr;

    char aiSym = player->get_symbol();
    int rows = board->get_rows();
    int cols = board->get_columns();

    vector<Point> safeMoves;
    vector<Point> allValidMoves;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            Move<char>* testMove = new Move<char>(r, c, aiSym);
            if (board->update_board(testMove)) {
                allValidMoves.push_back({r, c});
                // In Misere, is_lose checks if 3-in-a-row is formed
                if (!board->is_lose(player)) {
                    safeMoves.push_back({r, c});
                }
                board->undoLastMove();
            }
        }
    }

    Point p;
    if (!safeMoves.empty()) {
        p = safeMoves[rand() % safeMoves.size()];
    } else if (!allValidMoves.empty()) {
        p = allValidMoves[rand() % allValidMoves.size()];
    } else {
        return getRandomMove<BoardType>(player);
    }
    return new Move<char>(p.r, p.c, aiSym);
}

template <typename BoardType>
Move<char>* GameAI::getRandomMove(Player<char>* player) {
    BoardType* board = dynamic_cast<BoardType*>(player->get_board_ptr());
    char aiSym = player->get_symbol();
    
    // Improved Random Strategy: Find ALL valid moves first, then pick one.
    // This prevents infinite loops on sparse or nearly full boards.
    vector<Point> validMoves;
    int rows = board->get_rows();
    int cols = board->get_columns();

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            Move<char>* test = new Move<char>(r, c, aiSym);
            if (board->update_board(test)) {
                board->undoLastMove();
                validMoves.push_back({r, c});
            }
            // Note: update_board creates history, undo removes it.
        }
    }

    if (validMoves.empty()) return nullptr; // No moves available

    Point p = validMoves[rand() % validMoves.size()];
    return new Move<char>(p.r, p.c, aiSym);
}

// --- TTT_Board Implementations ---

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

// --- Misere_TTT_Board Implementations ---

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

// --- C4_Board Implementations ---

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

// --- TTT_UI Implementations ---

Move<char>* TTT_UI::computerMove(Player<char>* aiPlayer) {
    // CHANGED: Use random move instead of smart AI
    return GameAI::getRandomMove<TTT_Board>(aiPlayer);
}

TTT_UI::TTT_UI() : UI("Welcome to X-O", 3) {}

Player<char>* TTT_UI::create_player(string& name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

Move<char>* TTT_UI::get_move(Player<char>* p) {
    if (p->get_type() == PlayerType::HUMAN) {
        int r, c;
        cout << p->get_name() << ", enter row and col (0-2): ";
        cin >> r >> c;
        return new Move<char>(r, c, p->get_symbol());
    } else {
        cout << p->get_name() << " is thinking...\n";
        return computerMove(p);
    }
}

// --- Misere_TTT_UI Implementations ---

Misere_TTT_UI::Misere_TTT_UI() : UI("Welcome to Misere X-O", 3) {}

Player<char>* Misere_TTT_UI::create_player(string& name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

Move<char>* Misere_TTT_UI::get_move(Player<char>* p) {
    if (p->get_type() == PlayerType::HUMAN) {
        int r, c;
        cout << p->get_name() << ", enter row and col (0-2): ";
        cin >> r >> c;
        return new Move<char>(r, c, p->get_symbol());
    } else {
        cout << p->get_name() << " is thinking...\n";
        return computerMove(p);
    }
}

Move<char>* Misere_TTT_UI::computerMove(Player<char>* aiPlayer) {
    // CHANGED: Use random move instead of smart AI
    return GameAI::getRandomMove<Misere_TTT_Board>(aiPlayer);
}

// --- C4_UI Implementations ---

Move<char>* C4_UI::computerMove(Player<char>* aiPlayer) {
    // CHANGED: Use random move instead of smart AI
    return GameAI::getRandomMove<C4_Board>(aiPlayer);
}

C4_UI::C4_UI() : UI("Welcome to Four in a row", 2) {}

Player<char>* C4_UI::create_player(string& name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

Move<char>* C4_UI::get_move(Player<char>* p) {
    if (p->get_type() == PlayerType::HUMAN) {
        int c;
        cout << p->get_name() << ", enter column (0-6): ";
        cin >> c;
        return new Move<char>(0, c, p->get_symbol());
    } else {
        cout << p->get_name() << " is thinking...\n";
        return computerMove(p);
    }
}

// --- Numerical_TTT_Board Implementations ---

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

// --- Obstacles_TTT_Board Implementations ---

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
    // Add exactly 2 obstacles, if possible
    for (int k = 0; k < 2; ++k) {
        if (isFull()) return;
        
        int attempts = 0;
        // Try up to 100 times to find an empty spot for this obstacle
        while (attempts < 100) {
            int r = rand() % 6;
            int c = rand() % 6;
            if (board[r][c] == ' ') {
                board[r][c] = '#';
                break; // Obstacle placed, move to next k
            }
            attempts++;
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
        delete move;
        return false;
    }

    board[r][c] = symbol;
    history.push_back({r, c});
    n_moves++;

    // Add 2 obstacles only after every full round (Player 2's turn)
    // Round 1: P1 (move 1), P2 (move 2) -> add obstacles
    // Round 2: P1 (move 3), P2 (move 4) -> add obstacles
    // Condition: n_moves is even (2, 4, 6...)
    if (n_moves > 0 && n_moves % 2 == 0) {
        if (!isFull()) {
            addObstacles();
        }
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

// --- Numerical_TTT_UI Implementations ---

Numerical_TTT_UI::Numerical_TTT_UI() : UI("Welcome to Numerical X-O (Sum to 15)", 3) {}

Player<char>* Numerical_TTT_UI::create_player(string& name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

Move<char>* Numerical_TTT_UI::get_move(Player<char>* p) {
    if (p->get_type() == PlayerType::HUMAN) {
        bool is_odd_player = (p->get_symbol() == 'X');
        
        Numerical_TTT_Board* board = dynamic_cast<Numerical_TTT_Board*>(p->get_board_ptr());
        
        // Show available numbers
        if (is_odd_player) cout << "Available (Odd): "; else cout << "Available (Even): ";
        map<int, bool> used = is_odd_player ? board->get_odd_nums_used() : board->get_even_nums_used();
        for (auto const& [num, is_used] : used) {
            if (!is_used) cout << num << " ";
        }
        cout << endl;

        int r, c, num;
        cout << p->get_name() << ", enter row (0-2), col (0-2), and number: ";
        cin >> r >> c >> num;
        
        char num_char = num + '0';
        return new Move<char>(r, c, num_char);

    } else {
        cout << p->get_name() << " is thinking...\n";
        return computerMove(p);
    }
}

Move<char>* Numerical_TTT_UI::computerMove(Player<char>* aiPlayer) {
    Numerical_TTT_Board* board = dynamic_cast<Numerical_TTT_Board*>(aiPlayer->get_board_ptr());
    if (!board) return nullptr;

    bool is_odd = (aiPlayer->get_symbol() == 'X');
    vector<int> available_nums;
    map<int, bool> used = is_odd ? board->get_odd_nums_used() : board->get_even_nums_used();
    
    for (auto const& [num, is_used] : used) {
        if (!is_used) available_nums.push_back(num);
    }

    if (available_nums.empty()) return nullptr;

    // Robust Random Strategy: Find all valid (pos, num) pairs and pick one
    vector<Move<char>*> validMoves;
    for(int r=0; r<3; ++r) {
        for(int c=0; c<3; ++c) {
            for(int num : available_nums) {
                if(board->public_is_valid(r, c, num, is_odd)) {
                    validMoves.push_back(new Move<char>(r, c, num + '0'));
                }
            }
        }
    }

    if (validMoves.empty()) return nullptr;

    Move<char>* selected = validMoves[rand() % validMoves.size()];
    
    Move<char>* retMove = new Move<char>(selected->get_x(), selected->get_y(), selected->get_symbol());
    for(auto m : validMoves) delete m;
    
    return retMove;
}

// --- Obstacles_TTT_UI Implementations ---

Obstacles_TTT_UI::Obstacles_TTT_UI() : UI("Welcome to Obstacles X-O (4-in-a-row)", 2) {}

Player<char>* Obstacles_TTT_UI::create_player(string& name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

Move<char>* Obstacles_TTT_UI::get_move(Player<char>* p) {
    if (p->get_type() == PlayerType::HUMAN) {
        int r, c;
        cout << p->get_name() << ", enter row and col (0-5): ";
        cin >> r >> c;
        return new Move<char>(r, c, p->get_symbol());
    } else {
        cout << p->get_name() << " is thinking...\n";
        return computerMove(p);
    }
}

Move<char>* Obstacles_TTT_UI::computerMove(Player<char>* aiPlayer) {
    return GameAI::getRandomMove<Obstacles_TTT_Board>(aiPlayer);
}