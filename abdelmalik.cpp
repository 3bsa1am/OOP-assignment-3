/**
 * @file abdelmalik.cpp
 * @brief Implementation of Pyramid, Diamond, and Memory Tic-Tac-Toe.
 * @details Contains the logic for the specific board geometries and unique game rules.
 */

#include "abdelmalik.h"
#include "Mohammed.h" 
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <set>

using namespace std;

// --- Pyramid Tic-Tac-Toe Implementation ---

Pyramid_X_O_Board::Pyramid_X_O_Board() : Board(3, 5) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            board[i][j] = 0;
        }
    }
}

bool Pyramid_X_O_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char symbol = move->get_symbol();

    if (x < 0 || x >= rows || y < 0 || y >= columns) {
        delete move; 
        return false;
    }

    bool isValidShape = false;
    if (x == 0 && y == 2) isValidShape = true;
    else if (x == 1 && (y >= 1 && y <= 3)) isValidShape = true;
    else if (x == 2 && (y >= 0 && y <= 4)) isValidShape = true;

    if (!isValidShape || board[x][y] != 0) {
        delete move;
        return false;
    }

    board[x][y] = symbol;
    n_moves++;
    history.push_back({x, y}); 
    delete move;
    return true;
}

void Pyramid_X_O_Board::undoLastMove() {
    if (!history.empty()) {
        pair<int, int> last = history.back();
        history.pop_back();
        board[last.first][last.second] = 0;
        n_moves--;
    }
}

bool Pyramid_X_O_Board::is_win(Player<char>* player) {
    char s = player->get_symbol();
    auto check = [&](int r1, int c1, int r2, int c2, int r3, int c3) {
        return (board[r1][c1] == s && board[r2][c2] == s && board[r3][c3] == s);
    };

    if (check(0, 2, 1, 2, 2, 2)) return true;
    if (check(1, 1, 1, 2, 1, 3)) return true;
    if (check(2, 0, 2, 1, 2, 2)) return true;
    if (check(2, 1, 2, 2, 2, 3)) return true;
    if (check(2, 2, 2, 3, 2, 4)) return true;
    if (check(0, 2, 1, 3, 2, 4)) return true;
    if (check(0, 2, 1, 1, 2, 0)) return true;

    return false;
}

bool Pyramid_X_O_Board::is_draw(Player<char>* player) {
    return (n_moves == 9 && !is_win(player));
}

bool Pyramid_X_O_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

Pyramid_XO_UI::Pyramid_XO_UI() : UI<char>("Welcome to Pyramid Tic-Tac-Toe!", 3) {}

Player<char>* Pyramid_XO_UI::create_player(string& name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

// Locally implemented AI for Pyramid to avoid template issues
Move<char>* Pyramid_XO_UI::computerMove(Player<char>* player) {
    Pyramid_X_O_Board* board = dynamic_cast<Pyramid_X_O_Board*>(player->get_board_ptr());
    if (!board) return nullptr;

    char aiSym = player->get_symbol();
    char oppSym = (aiSym == 'X') ? 'O' : 'X';
    
    // 1. Check Win
    for(int r=0; r<3; ++r) {
        for(int c=0; c<5; ++c) {
            // Check manual validity first (pyramid shape)
            bool valid = (r==0 && c==2) || (r==1 && c>=1 && c<=3) || (r==2 && c>=0 && c<=4);
            if(valid && board->get_cell(r,c) == 0) {
                Move<char>* m = new Move<char>(r, c, aiSym);
                if(board->update_board(m)) {
                    if(board->is_win(player)) {
                        board->undoLastMove();
                        return new Move<char>(r, c, aiSym);
                    }
                    board->undoLastMove();
                }
            }
        }
    }

    // 2. Block
    Player<char> opponent("temp", oppSym, PlayerType::HUMAN);
    for(int r=0; r<3; ++r) {
        for(int c=0; c<5; ++c) {
            bool valid = (r==0 && c==2) || (r==1 && c>=1 && c<=3) || (r==2 && c>=0 && c<=4);
            if(valid && board->get_cell(r,c) == 0) {
                Move<char>* m = new Move<char>(r, c, oppSym);
                if(board->update_board(m)) {
                    if(board->is_win(&opponent)) {
                        board->undoLastMove();
                        return new Move<char>(r, c, aiSym);
                    }
                    board->undoLastMove();
                }
            }
        }
    }

    // 3. Random
    vector<pair<int,int>> validMoves;
    for(int r=0; r<3; ++r) {
        for(int c=0; c<5; ++c) {
            bool valid = (r==0 && c==2) || (r==1 && c>=1 && c<=3) || (r==2 && c>=0 && c<=4);
            if(valid && board->get_cell(r,c) == 0) validMoves.push_back({r,c});
        }
    }
    
    if(validMoves.empty()) return nullptr;
    pair<int,int> p = validMoves[rand() % validMoves.size()];
    return new Move<char>(p.first, p.second, aiSym);
}

Move<char>* Pyramid_XO_UI::get_move(Player<char>* player) {
    if (player->get_type() == PlayerType::HUMAN) {
        int x, y;
        cout << "\nEnter move for " << player->get_name() << " (row col): ";
        cin >> x >> y;
        return new Move<char>(x, y, player->get_symbol());
    } else {
        cout << player->get_name() << " is thinking...\n";
        return computerMove(player);
    }
}

void Pyramid_XO_UI::display_board_matrix(const vector<vector<char>>& matrix) const {
    cout << "\n";
    cout << "           "; 
    cout << "| " << (matrix[0][2] == 0 ? '.' : matrix[0][2]) << " |";
    cout << "              (0 , 2)\n";
    cout << "     "; 
    for (int j = 1; j <= 3; ++j) {
        cout << "| " << (matrix[1][j] == 0 ? '.' : matrix[1][j]) << " |";
    }
    cout << "       (1 , 1 - 3)\n";
    for (int j = 0; j <= 4; ++j) {
        cout << "| " << (matrix[2][j] == 0 ? '.' : matrix[2][j]) << " |";
    }
    cout << "  (2 , 0 - 4)\n\n";
}

// --- Diamond Tic-Tac-Toe Implementation ---

Diamond_XO_Board::Diamond_XO_Board() : Board(7, 7) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            board[i][j] = 0;
        }
    }
}

bool Diamond_XO_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char symbol = move->get_symbol();

    if (x < 0 || x >= rows || y < 0 || y >= columns) {
        delete move;
        return false;
    }

    // Manhattan distance restriction for 7x7 Diamond shape
    // Center is (3,3). Valid if abs(x-3) + abs(y-3) <= 3
    if (abs(x - 3) + abs(y - 3) > 3) {
        delete move;
        return false;
    }

    if (board[x][y] != 0) {
        delete move;
        return false;
    }

    board[x][y] = symbol;
    n_moves++;
    history.push_back({x, y}); 
    delete move;
    return true;
}

void Diamond_XO_Board::undoLastMove() {
    if (!history.empty()) {
        pair<int, int> last = history.back();
        history.pop_back();
        board[last.first][last.second] = 0;
        n_moves--;
    }
}

bool Diamond_XO_Board::is_win(Player<char>* player) {
    char s = player->get_symbol();
    set<int> dirs3, dirs4; 
    
    // Directions: 0:Right, 1:Down, 2:DiagDownRight, 3:DiagDownLeft
    int dr[] = {0, 1, 1, 1};
    int dc[] = {1, 0, 1, -1};

    auto get_len = [&](int r, int c, int d) {
        int len = 0;
        int tr = r, tc = c;
        // Check bounds for 7x7
        while(tr >= 0 && tr < 7 && tc >= 0 && tc < 7 && board[tr][tc] == s) {
            len++;
            tr += dr[d];
            tc += dc[d];
        }
        return len;
    };

    auto is_start_of_line = [&](int r, int c, int d) {
        int pr = r - dr[d];
        int pc = c - dc[d];
        // If previous cell is valid and same symbol, this isn't the start
        if (pr >= 0 && pr < 7 && pc >= 0 && pc < 7 && board[pr][pc] == s) return false;
        return true;
    };

    for(int i=0; i<7; i++) {
        for(int j=0; j<7; j++) {
            if(board[i][j] == s) {
                for(int d=0; d<4; d++) {
                    if(is_start_of_line(i, j, d)) {
                        int len = get_len(i, j, d);
                        // A line of 4 implies a line of 3, so add to both
                        if(len >= 3) dirs3.insert(d);
                        if(len >= 4) dirs4.insert(d);
                    }
                }
            }
        }
    }

    // Win if we have a line of 3 and a line of 4 in DIFFERENT directions
    for(int d3 : dirs3) {
        for(int d4 : dirs4) {
            if(d3 != d4) return true;
        }
    }

    return false;
}

bool Diamond_XO_Board::is_draw(Player<char>* player) {
    // 25 valid cells in 7x7 diamond (radius 3)
    return n_moves == 25 && !is_win(player); 
}

bool Diamond_XO_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

Diamond_XO_UI::Diamond_XO_UI() : UI<char>("Welcome to Diamond Tic-Tac-Toe!", 2) {}

Player<char>* Diamond_XO_UI::create_player(string& name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

// Locally implemented AI for Diamond to ensure robustness
Move<char>* Diamond_XO_UI::computerMove(Player<char>* player) {
    Diamond_XO_Board* board = dynamic_cast<Diamond_XO_Board*>(player->get_board_ptr());
    if (!board) return nullptr;

    char aiSym = player->get_symbol();
    char oppSym = (aiSym == 'X') ? 'O' : 'X';
    vector<Move<char>*> validMoves;

    // 1. Gather all valid moves
    for(int i=0; i<7; ++i) {
        for(int j=0; j<7; ++j) {
            // Check manual validity (bounds + diamond shape + empty)
            if (abs(i - 3) + abs(j - 3) <= 3 && board->get_board_matrix()[i][j] == 0) {
                validMoves.push_back(new Move<char>(i, j, aiSym));
            }
        }
    }

    if (validMoves.empty()) return nullptr;

    // 2. Check for Immediate Win
    for (auto move : validMoves) {
        // Use a copy to prevent update_board from deleting the pointer in validMoves
        Move<char>* tempMove = new Move<char>(move->get_x(), move->get_y(), move->get_symbol());
        if (board->update_board(tempMove)) {
            if (board->is_win(player)) {
                board->undoLastMove();
                Move<char>* winningMove = new Move<char>(move->get_x(), move->get_y(), move->get_symbol());
                for(auto m : validMoves) delete m;
                return winningMove; 
            }
            board->undoLastMove();
        }
    }

    // 3. Check for Immediate Block
    Player<char> opponent("temp", oppSym, PlayerType::HUMAN);
    for (auto move : validMoves) {
        Move<char>* oppMove = new Move<char>(move->get_x(), move->get_y(), oppSym);
        if (board->update_board(oppMove)) {
            if (board->is_win(&opponent)) {
                board->undoLastMove();
                Move<char>* blockingMove = new Move<char>(move->get_x(), move->get_y(), aiSym);
                for(auto m : validMoves) delete m;
                return blockingMove; 
            }
            board->undoLastMove();
        }
    }

    // 4. Random fallback
    Move<char>* bestMove = validMoves[rand() % validMoves.size()];
    Move<char>* finalMove = new Move<char>(bestMove->get_x(), bestMove->get_y(), bestMove->get_symbol());
    for(auto m : validMoves) delete m;

    return finalMove;
}

Move<char>* Diamond_XO_UI::get_move(Player<char>* player) {
    if (player->get_type() == PlayerType::HUMAN) {
        int x, y;
        cout << "\nEnter move for " << player->get_name() << " (row col): ";
        cin >> x >> y;
        return new Move<char>(x, y, player->get_symbol());
    } else {
        cout << player->get_name() << " is thinking...\n";
        return computerMove(player);
    }
}

void Diamond_XO_UI::display_board_matrix(const vector<vector<char>>& matrix) const {
    cout << "\n";
    cout << "     0   1   2   3   4   5   6\n";
    cout << "   -----------------------------\n";
    
    for (int i = 0; i < 7; ++i) {
        cout << " " << i << " | ";
        for (int j = 0; j < 7; ++j) {
            // Check if cell is part of the diamond (radius 3 from center 3,3)
            if (abs(i - 3) + abs(j - 3) > 3) {
                cout << "    "; // Empty space for non-diamond cells
            } else {
                cout << (matrix[i][j] == 0 ? '.' : matrix[i][j]) << " | ";
            }
        }
        cout << "\n   -----------------------------\n";
    }
    cout << "\n";
}

// --- Memory Tic-Tac-Toe Implementation ---

Memory_XO_Board::Memory_XO_Board() : Board(3, 3) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            board[i][j] = 0;
        }
    }
}

bool Memory_XO_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char symbol = move->get_symbol();

    if (x < 0 || x >= rows || y < 0 || y >= columns || board[x][y] != 0) {
        delete move;
        return false;
    }

    board[x][y] = symbol;
    n_moves++;
    history.push_back({x, y}); 
    delete move;
    return true;
}

void Memory_XO_Board::undoLastMove() {
    if (!history.empty()) {
        pair<int, int> last = history.back();
        history.pop_back();
        board[last.first][last.second] = 0;
        n_moves--;
    }
}

bool Memory_XO_Board::is_win(Player<char>* player) {
    char s = player->get_symbol();
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == s && board[i][1] == s && board[i][2] == s) return true;
        if (board[0][i] == s && board[1][i] == s && board[2][i] == s) return true;
    }
    if (board[0][0] == s && board[1][1] == s && board[2][2] == s) return true;
    if (board[0][2] == s && board[1][1] == s && board[2][0] == s) return true;
    return false;
}

bool Memory_XO_Board::is_draw(Player<char>* player) {
    return n_moves == 9 && !is_win(player);
}

bool Memory_XO_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

Memory_XO_UI::Memory_XO_UI() : UI<char>("Welcome to Memory Tic-Tac-Toe! Marks disappear after placement.", 3) {}

Player<char>* Memory_XO_UI::create_player(string& name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

// Locally implemented AI for Memory
Move<char>* Memory_XO_UI::computerMove(Player<char>* player) {
    Memory_XO_Board* board = dynamic_cast<Memory_XO_Board*>(player->get_board_ptr());
    if (!board) return nullptr;

    char aiSym = player->get_symbol();
    char oppSym = (aiSym == 'X') ? 'O' : 'X';
    
    // 1. Win
    for(int r=0; r<3; ++r) {
        for(int c=0; c<3; ++c) {
            if(board->get_cell(r, c) == 0) {
                Move<char>* m = new Move<char>(r, c, aiSym);
                if(board->update_board(m)) {
                    if(board->is_win(player)) {
                        board->undoLastMove();
                        return new Move<char>(r, c, aiSym);
                    }
                    board->undoLastMove();
                }
            }
        }
    }

    // 2. Block
    Player<char> opponent("temp", oppSym, PlayerType::HUMAN);
    for(int r=0; r<3; ++r) {
        for(int c=0; c<3; ++c) {
            if(board->get_cell(r, c) == 0) {
                Move<char>* m = new Move<char>(r, c, oppSym);
                if(board->update_board(m)) {
                    if(board->is_win(&opponent)) {
                        board->undoLastMove();
                        return new Move<char>(r, c, aiSym);
                    }
                    board->undoLastMove();
                }
            }
        }
    }

    // 3. Random
    vector<pair<int, int>> validMoves;
    for(int r=0; r<3; ++r) {
        for(int c=0; c<3; ++c) {
            if(board->get_cell(r, c) == 0) validMoves.push_back({r, c});
        }
    }
    if(validMoves.empty()) return nullptr;
    pair<int,int> p = validMoves[rand() % validMoves.size()];
    return new Move<char>(p.first, p.second, aiSym);
}

Move<char>* Memory_XO_UI::get_move(Player<char>* player) {
    if (player->get_type() == PlayerType::HUMAN) {
        int x, y;
        cout << "\nEnter move for " << player->get_name() << " (row col): ";
        cin >> x >> y;
        return new Move<char>(x, y, player->get_symbol());
    } else {
        cout << player->get_name() << " is thinking...\n";
        return computerMove(player);
    }
}

void Memory_XO_UI::display_board_matrix(const vector<vector<char>>& matrix) const {
    // Check if the game is over (Win or Draw) to reveal marks
    bool reveal = false;
    
    // Check Rows & Cols for win
    for(int i=0; i<3; ++i) {
        if(matrix[i][0] != 0 && matrix[i][0] == matrix[i][1] && matrix[i][1] == matrix[i][2]) reveal = true;
        if(matrix[0][i] != 0 && matrix[0][i] == matrix[1][i] && matrix[1][i] == matrix[2][i]) reveal = true;
    }
    // Check Diagonals for win
    if(matrix[0][0] != 0 && matrix[0][0] == matrix[1][1] && matrix[1][1] == matrix[2][2]) reveal = true;
    if(matrix[0][2] != 0 && matrix[0][2] == matrix[1][1] && matrix[1][1] == matrix[2][0]) reveal = true;

    // Check for Full Board (Draw)
    if (!reveal) {
        bool full = true;
        for(const auto& r : matrix) {
            for(char c : r) if(c == 0) full = false;
        }
        if(full) reveal = true;
    }

    cout << "\n";
    cout << "     0   1   2\n";
    cout << "   -------------\n";
    
    for (int i = 0; i < 3; ++i) {
        cout << " " << i << " | ";
        for (int j = 0; j < 3; ++j) {
            char cell = matrix[i][j];
            char displayChar;
            
            if (cell == 0) {
                displayChar = '.'; // Empty cell
            } else if (reveal) {
                displayChar = cell; // Game Over: Show actual mark
            } else {
                displayChar = '#';  // Game Active: Show mask
            }
            
            cout << displayChar << " | ";
        }
        cout << "\n   -------------\n";
    }
    cout << "\n";
}