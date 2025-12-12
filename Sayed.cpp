/**
 * @file Sayed.cpp
 * @brief Implementation of 5x5 Tic-Tac-Toe, Word Tic-Tac-Toe, and Infinity Tic-Tac-Toe.
 * @details Contains the game logic, board updates, win conditions, and UI implementations for Sayed's games.
 */

#include "Sayed.h"
#include "Mohammed.h" // Required for GameAI
#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cctype>
#include <deque>
#include <filesystem> 

using namespace std;

// ==========================================
// 5x5 Tic Tac Toe Implementation
// ==========================================

XO_5x5_Board::XO_5x5_Board() : Board(5, 5) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            board[i][j] = blank_symbol;
        }
    }
}

void XO_5x5_Board::undoLastMove() {
    if (!history.empty()) {
        Point last = history.back();
        history.pop_back();
        board[last.r][last.c] = blank_symbol;
        n_moves--;
    }
}

bool XO_5x5_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char sym = move->get_symbol();

    if (x >= 0 && x < rows && y >= 0 && y < columns && board[x][y] == blank_symbol) {
        n_moves++;
        board[x][y] = toupper(sym);
        history.push_back({x, y});
        delete move;
        return true;
    }
    delete move;
    return false;
}

// Counts how many "3-in-a-rows" exist for a specific symbol
int XO_5x5_Board::count_three_in_row(char symbol) const {
    char sym = toupper(symbol);
    int count = 0;
    
    // Check Rows
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j <= columns - 3; j++) {
            if (board[i][j] == sym && board[i][j+1] == sym && board[i][j+2] == sym) count++;
        }
    }
    // Check Cols
    for (int j = 0; j < columns; j++) {
        for (int i = 0; i <= rows - 3; i++) {
            if (board[i][j] == sym && board[i+1][j] == sym && board[i+2][j] == sym) count++;
        }
    }
    // Check Diagonals
    for (int i = 0; i <= rows - 3; i++) {
        for (int j = 0; j <= columns - 3; j++) {
            if (board[i][j] == sym && board[i+1][j+1] == sym && board[i+2][j+2] == sym) count++;
            if (board[i][j+2] == sym && board[i+1][j+1] == sym && board[i+2][j] == sym) count++;
        }
    }
    return count;
}

bool XO_5x5_Board::is_win(Player<char>* player) {
    if (n_moves < 24) return false; 
    
    char current_symbol = toupper(player->get_symbol());
    char opponent_symbol = (current_symbol == 'X') ? 'O' : 'X';
    
    return count_three_in_row(current_symbol) > count_three_in_row(opponent_symbol);
}

bool XO_5x5_Board::is_draw(Player<char>* player) {
    if (n_moves < 24) return false;
    
    char current_symbol = toupper(player->get_symbol());
    char opponent_symbol = (current_symbol == 'X') ? 'O' : 'X';
    
    return count_three_in_row(current_symbol) == count_three_in_row(opponent_symbol);
}

bool XO_5x5_Board::game_is_over(Player<char>* player) {
    return n_moves >= 24;
}

// --- UI 5x5 ---

XO_5x5_UI::XO_5x5_UI() : UI<char>("Welcome to 5x5 Tic Tac Toe (Most 3-in-a-row wins!)", 3) {}

Player<char>* XO_5x5_UI::create_player(string& name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

Move<char>* XO_5x5_UI::computerMove(Player<char>* aiPlayer) {
    XO_5x5_Board* board = dynamic_cast<XO_5x5_Board*>(aiPlayer->get_board_ptr());
    if (!board) return nullptr;

    char aiSym = aiPlayer->get_symbol();
    char humanSym = (aiSym == 'X') ? 'O' : 'X';
    int rows = board->get_rows();
    int cols = board->get_columns();

    int currentAiScore = board->count_three_in_row(aiSym);
    int currentHumanScore = board->count_three_in_row(humanSym);

    // 1. Try to increase own score
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            Move<char>* testMove = new Move<char>(r, c, aiSym);
            if (board->update_board(testMove)) {
                if (board->count_three_in_row(aiSym) > currentAiScore) {
                    board->undoLastMove();
                    return new Move<char>(r, c, aiSym);
                }
                board->undoLastMove();
            }
        }
    }

    // 2. Block opponent
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            Move<char>* testMove = new Move<char>(r, c, humanSym);
            if (board->update_board(testMove)) {
                if (board->count_three_in_row(humanSym) > currentHumanScore) {
                    board->undoLastMove();
                    return new Move<char>(r, c, aiSym); 
                }
                board->undoLastMove();
            }
        }
    }

    // 3. Random fallback
    return GameAI::getRandomMove<XO_5x5_Board>(aiPlayer);
}

Move<char>* XO_5x5_UI::get_move(Player<char>* player) {
    if (player->get_type() == PlayerType::HUMAN) {
        int x, y;
        cout << "\n" << player->get_name() << "'s turn (" << player->get_symbol() << ")\n";
        cout << "Enter your move (row and column: 0-4): ";
        cin >> x >> y;
        return new Move<char>(x, y, player->get_symbol());
    }
    else {
        cout << player->get_name() << " (" << player->get_symbol() << ") is thinking...\n";
        return computerMove(player);
    }
}


// ==========================================
// Word Tic-Tac-Toe Implementation
// ==========================================

Word_TTT_Board::Word_TTT_Board() : Board(3, 3) {
    // Attempt to load dictionary from common locations
    vector<string> paths = {
        "dic.txt",
        "../dic.txt",
        "../../dic.txt",
        "../../../dic.txt"
    };

    bool loaded = false;
    for (const auto& path : paths) {
        if (load_dictionary(path)) {
            loaded = true;
            break;
        }
    }

    if (!loaded) {
        cout << "Error: Could not open dictionary file 'dic.txt'.\n";
        cout << "Checked in the following locations:\n";
        for (const auto& path : paths) {
            cout << " - " << path << "\n";
        }
        cout << "Current Working Directory: " << std::filesystem::current_path() << "\n";
    }
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            board[i][j] = blank_symbol;
        }
    }
}

bool Word_TTT_Board::load_dictionary(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    string word;
    while (file >> word) {
        // Sanitize input
        string clean_word = "";
        for (char c : word) {
            if (isalpha(c)) clean_word += toupper(c);
        }
        if (clean_word.length() == 3) dictionary.insert(clean_word);
    }
    
    file.close();
    cout << "Loaded " << dictionary.size() << " valid words from " << filename << ".\n";
    return true;
}

string Word_TTT_Board::get_word(int start_r, int start_c, int dr, int dc) const {
    string word = "";
    for (int k = 0; k < 3; k++) {
        int r = start_r + k * dr;
        int c = start_c + k * dc;
        if (r >= 0 && r < rows && c >= 0 && c < columns) {
            word += board[r][c];
        }
    }
    return word;
}

bool Word_TTT_Board::check_word_formed() const {
    // Check rows
    for (int i = 0; i < 3; i++) {
        string word = get_word(i, 0, 0, 1); 
        if (word.length() == 3 && dictionary.count(word)) return true;
    }
    // Check cols
    for (int j = 0; j < 3; j++) {
        string word = get_word(0, j, 1, 0);
        if (word.length() == 3 && dictionary.count(word)) return true;
    }
    // Check diagonals
    string diag1 = get_word(0, 0, 1, 1);
    if (diag1.length() == 3 && dictionary.count(diag1)) return true;
    
    string diag2 = get_word(0, 2, 1, -1);
    if (diag2.length() == 3 && dictionary.count(diag2)) return true;
    
    return false;
}

void Word_TTT_Board::undoLastMove() {
    if (!history.empty()) {
        Point last = history.back();
        history.pop_back();
        board[last.r][last.c] = blank_symbol;
        n_moves--;
    }
}

bool Word_TTT_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char letter = move->get_symbol();

    if (!isalpha(letter)) {
        cout << "Invalid input. Please enter a letter (A-Z).\n";
        delete move;
        return false;
    }

    if (x >= 0 && x < rows && y >= 0 && y < columns && board[x][y] == blank_symbol) {
        n_moves++;
        board[x][y] = toupper(letter);
        history.push_back({x, y});
        delete move;
        return true;
    }
    
    delete move;
    return false;
}

bool Word_TTT_Board::is_win(Player<char>* player) {
    return check_word_formed();
}

bool Word_TTT_Board::is_draw(Player<char>* player) {
    return (n_moves == 9 && !check_word_formed());
}

bool Word_TTT_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

// --- UI Word TTT ---

Word_TTT_UI::Word_TTT_UI() : UI<char>("Welcome to Word Tic-Tac-Toe! Form 3-letter words to win.", 3) {}

Player<char>* Word_TTT_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (uses letters)\n";
    return new Player<char>(name, symbol, type);
}

Move<char>* Word_TTT_UI::computerMove(Player<char>* aiPlayer) {
    Word_TTT_Board* board = dynamic_cast<Word_TTT_Board*>(aiPlayer->get_board_ptr());
    if (!board) return nullptr;

    int rows = 3;
    int cols = 3;

    // 1. Try A-Z to see if AI can win
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (board->get_cell(r, c) == ' ') {
                for (char l = 'A'; l <= 'Z'; l++) {
                    Move<char>* testMove = new Move<char>(r, c, l);
                    if (board->update_board(testMove)) {
                        if (board->is_win(aiPlayer)) {
                            board->undoLastMove();
                            return new Move<char>(r, c, l);
                        }
                        board->undoLastMove();
                    }
                }
            }
        }
    }

    // 2. Play Random valid move
    vector<Point> emptyCells;
    for(int r=0; r<3; r++)
        for(int c=0; c<3; c++)
            if(board->get_cell(r,c) == ' ') emptyCells.push_back({r,c});

    if(!emptyCells.empty()){
        Point p = emptyCells[rand() % emptyCells.size()];
        char l = 'A' + (rand() % 26);
        return new Move<char>(p.r, p.c, l);
    }

    return new Move<char>(0, 0, 'A');
}

Move<char>* Word_TTT_UI::get_move(Player<char>* player) {
    if (player->get_type() == PlayerType::HUMAN) {
        int x, y;
        char letter;
        cout << "\n" << player->get_name() << "'s turn";
        cout << " | Enter row (0-2), column (0-2), and a letter A-Z: ";
        cin >> x >> y >> letter;
        return new Move<char>(x, y, letter);
    } else {
        cout << player->get_name() << " is thinking...\n";
        return computerMove(player);
    }
}


// ==========================================
// Infinity Tic-Tac-Toe Implementation
// ==========================================

Infinity_TTT_Board::Infinity_TTT_Board() : Board(3, 3) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            board[i][j] = blank_symbol;
        }
    }
}

bool Infinity_TTT_Board::check_win(char symbol) const {
    char sym = toupper(symbol);
    // Rows
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == sym && board[i][1] == sym && board[i][2] == sym) return true;
    }
    // Cols
    for (int j = 0; j < 3; j++) {
        if (board[0][j] == sym && board[1][j] == sym && board[2][j] == sym) return true;
    }
    // Diagonals
    if (board[0][0] == sym && board[1][1] == sym && board[2][2] == sym) return true;
    if (board[0][2] == sym && board[1][1] == sym && board[2][0] == sym) return true;
    
    return false;
}

bool Infinity_TTT_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char sym = move->get_symbol();

    if (x >= 0 && x < rows && y >= 0 && y < columns && board[x][y] == blank_symbol) {
        n_moves++;
        board[x][y] = toupper(sym);
        move_queue.push_back({x, y});

        InfinityMoveInfo info;
        info.placed = {x, y};
        info.caused_removal = false;

        if (n_moves >= 4 && (n_moves - 1) % 3 == 0) {
            if (!move_queue.empty()) {
                Point oldest = move_queue.front();
                move_queue.pop_front();
                
                info.caused_removal = true;
                info.removed_point = oldest;
                info.removed_symbol = board[oldest.r][oldest.c];
                
                board[oldest.r][oldest.c] = blank_symbol;
            }
        }
        
        history.push_back(info);
        delete move;
        return true;
    }
    
    delete move;
    return false;
}

void Infinity_TTT_Board::undoLastMove() {
    if (!history.empty()) {
        InfinityMoveInfo last = history.back();
        history.pop_back();

        // 1. Remove the placed piece
        board[last.placed.r][last.placed.c] = blank_symbol;
        move_queue.pop_back(); 

        // 2. Restore removed piece
        if (last.caused_removal) {
            board[last.removed_point.r][last.removed_point.c] = last.removed_symbol;
            move_queue.push_front(last.removed_point); 
        }

        n_moves--;
    }
}

bool Infinity_TTT_Board::is_win(Player<char>* player) {
    return check_win(player->get_symbol());
}

bool Infinity_TTT_Board::is_draw(Player<char>* player) {
    // Force draw after 50 moves to prevent infinite loops in AI vs AI
    if (n_moves > 50) return true;
    
    // Standard Draw if board full (shouldn't happen often in Infinity)
    for(int i=0; i<rows; i++) {
        for(int j=0; j<columns; j++) {
            if (board[i][j] == blank_symbol) return false;
        }
    }
    return true;
}

bool Infinity_TTT_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

// --- UI Infinity ---

Infinity_TTT_UI::Infinity_TTT_UI() : UI<char>("Welcome to Infinity Tic-Tac-Toe! Oldest moves disappear every 3 moves.", 3) {}

Player<char>* Infinity_TTT_UI::create_player(string& name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

Move<char>* Infinity_TTT_UI::computerMove(Player<char>* aiPlayer) {
    return GameAI::getBestMove_2D<Infinity_TTT_Board>(aiPlayer);
}

Move<char>* Infinity_TTT_UI::get_move(Player<char>* player) {
    if (player->get_type() == PlayerType::HUMAN) {
        int x, y;
        cout << "\n" << player->get_name() << "'s turn (" << player->get_symbol() << ")\n";
        cout << "Enter your move (row and column: 0-2): ";
        cin >> x >> y;
        return new Move<char>(x, y, player->get_symbol());
    }
    else {
        cout << player->get_name() << " (" << player->get_symbol() << ") is thinking...\n";
        return computerMove(player);
    }
}