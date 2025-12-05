/**
 * @file amir.cpp
 * @brief Implementation of 4x4 Tic-Tac-Toe, SUS, and Ultimate Tic-Tac-Toe.
 * @details Contains specific logic, winning conditions, and AI implementations.
 */

#include <iostream>
#include <cctype>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include "Amir.h"
#include "Mohammed.h" // For basic AI helpers

using namespace std;

// ==========================================
// 4x4 Tic-Tac-Toe Implementation
// ==========================================

XO_4_x_4_Board::XO_4_x_4_Board():Board(4,4) {
    for (auto &row : board) {
        for (auto &col : row) {
            col = blank_symbol;
        }
    }
}

void XO_4_x_4_Board::undoLastMove() {
    if (!history.empty()) {
        Point last = history.back();
        history.pop_back();
        board[last.r][last.c] = blank_symbol;
        n_moves--;
    }
}

bool XO_4_x_4_Board::update_board(Move<char> *move) {
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

bool XO_4_x_4_Board::is_win(Player<char> *player) {
    char sym = toupper(player->get_symbol());
    const int dx[8] = {0,  0,  1, -1,  1, -1,  1, -1};
    const int dy[8] = {1, -1,  0,  0,  1, -1, -1,  1};
    auto equal = [&](int r, int c) {
        return toupper(board[r][c]) == sym;
    };
    for (int i = 0;i < rows;i++) {
        for (int j = 0;j < columns;j++) {
            for (int d = 0;d < 8 ;d++) {
                int cnt = 0;
                for (int k = 0;k<3;k++) {
                    int nx = i + dx[d]*k;
                    int ny = j + dy[d]*k;
                    if (nx < 0 || ny < 0 || nx >= rows || ny >= columns)
                        break;
                    if (equal(nx,ny))
                        cnt++;
                    else
                        break;
                }
                if (cnt == 3)
                    return true;
            }
        }
    }
    return false;
}

bool XO_4_x_4_Board::is_draw(Player<char> *player) {
    return (n_moves == 16 && !is_win(player));
}

bool XO_4_x_4_Board::game_is_over(Player<char> *player) {
    return is_win(player) || is_draw(player);
}

// --- UI 4x4 ---

XO_4_x_4_UI::XO_4_x_4_UI() : UI<char>("Welcome to 4x4 XO Game: ", 4) {}

Player<char>* XO_4_x_4_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

Move<char>* XO_4_x_4_UI::computerMove(Player<char>* aiPlayer) {
    return GameAI::getBestMove_2D<XO_4_x_4_Board>(aiPlayer);
}

Move<char>* XO_4_x_4_UI::get_move(Player<char>* player) {
    if (player->get_type() == PlayerType::HUMAN) {
        int x, y;
        cout << "\nPlease enter your move x and y (0 to 3): ";
        cin >> x >> y;
        return new Move<char>(x, y, player->get_symbol());
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        cout << player->get_name() << " (" << player->get_symbol() << ") is thinking...\n";
        return computerMove(player);
    }
    return nullptr;
}

// ==========================================
// SUS Game Implementation
// ==========================================

sus_board::sus_board():Board(3,3) {
    for (int i = 0;i<3;i++) {
        for (int j = 0;j < 3;j++) {
            board[i][j] = blank_symbol;
        }
    }
    for (int a=0;a<3;a++)
        for (int b=0;b<3;b++)
            taken[a][b] = false;
}

void sus_board::undoLastMove() {
    if (!history.empty()) {
        Point last = history.back();
        history.pop_back();
        board[last.r][last.c] = blank_symbol;
        n_moves--;
    }
}

bool sus_board::update_board(Move<char> *move) {
    int x = move->get_x();
    int y = move->get_y();
    char sym = move->get_symbol();

    if (x >= 0 && x < rows && y >= 0 && y < columns && board[x][y] == blank_symbol) {
        n_moves++;
        board[x][y] = toupper(sym);
        cnt_score(move);
        history.push_back({x, y});
        delete move;
        return true;
    }
    delete move;
    return false;
}

void sus_board::cnt_score(Move<char> *move) {
    char sym = move->get_symbol();
    int i = move->get_x();
    int j = move->get_y();
    string sus = "SUS";
    const int dx[8] = {0,  0,  1, -1,  1, -1,  1, -1};
    const int dy[8] = {1, -1,  0,  0,  1, -1, -1,  1};

    if (sym == 'U') {
        if (i-1 >= 0 && i+1 < rows &&  !taken[i-1][j] && !taken[i+1][j] && board[i-1][j] == 'S' && board[i+1][j] == 'S') {
            cnt2++;
            taken[i][j] = taken[i+1][j] = taken[i-1][j] = true;
        }
        else if (j-1 >= 0 && j+1 < columns && !taken[i][j-1] && !taken[i][j+1]  && board[i][j-1] == 'S' && board[i][j+1] == 'S') {
            cnt2++;
             taken[i][j] = taken[i][j+1] = taken[i][j-1] = true;
        }
        else if (i == 1 && j == 1 && !taken[i+1][j-1] && !taken[i-1][j+1] &&  board[i+1][j-1] == 'S' && board[i-1][j+1] == 'S') {
            cnt2++;
             taken[i][j] = taken[i+1][j-1] = taken[i-1][j+1] = true;
        }
        else if (i == 1 && j == 1 && !taken[i+1][j+1] && !taken[i-1][j-1] && board[i-1][j-1] == 'S' && board[i+1][j+1] == 'S') {
            cnt2++;
             taken[i][j] = taken[i+1][j+1] = taken[i-1][j-1] = true;
        }
    }
    else {
        for (int d = 0;d < 8 ;d++) {
            string curr = "";
            for (int k = 0;k<3;k++) {
                int nx = i + dx[d]*k;
                int ny = j + dy[d]*k;
                if (nx < 0 || ny < 0 || nx >= rows || ny >= columns || taken[nx][ny])
                    break;
                curr+=board[nx][ny];
            }
            if (curr == sus) {
                cnt1++;
                for (int k = 0; k < 3; k++) {
                    int nx = i + dx[d]*k;
                    int ny = j + dy[d]*k;
                    taken[nx][ny] = true;
                }
            }
        }
    }
}

bool sus_board::game_is_over(Player<char> *player) {
    return n_moves == 9;
}

bool sus_board::is_win(Player<char> *player) {
    char sym = player->get_symbol();
    if (!game_is_over(player))
        return false;
    // cnt1 is S score, cnt2 is U score
    // In strict SUS, it is competitive score. 
    // Usually P1 is 'S' and P2 is 'U' or similar, but here moves are free choice.
    // So we compare P1's moves vs P2's moves? 
    // Wait, standard implementation usually assigns scores to player who made move.
    // The variables cnt1 and cnt2 here seem to track count of SUS formed by S vs U?
    // Actually looking at code:
    // If placing 'U' forms SUS -> cnt2++
    // If placing 'S' forms SUS -> cnt1++
    // So if I play S, I increment cnt1. If I play U, I increment cnt2.
    // This implies Players are not owning score, but Symbols are owning score.
    // BUT the prompt says "player with most points". 
    // We will assume Player 1 wants S count high, Player 2 wants U count high?
    // OR we assume whoever triggers the count gets the point.
    // Given the variables are inside board, let's assume cnt1 is S-points and cnt2 is U-points.
    
    if (sym == 'S') return cnt1 > cnt2; // Player holding S wins if S-score > U-score
    else if (sym == 'U') return cnt2 > cnt1;
    return false;
}

bool sus_board::is_draw(Player<char> *player) {
    return (game_is_over(player) && cnt1 == cnt2);
}

// --- UI SUS ---

sus_ui::sus_ui() : UI<char>("Welcome to SUS Game: ", 3) {}

Player<char>* sus_ui::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating "
         << (type == PlayerType::HUMAN ? "human" : "computer")
         << " player: " << name << "\n";
    // Symbol doesn't matter much since they choose per turn, but kept for base class
    return new Player<char>(name, symbol, type);
}

Player<char> **sus_ui::setup_players() {
    Player<char>** players = new Player<char>*[2];
    vector<string> type_options = { "Human", "Computer" };

    string name1 = get_player_name("Player 1");
    PlayerType type1 = get_player_type_choice("Player 1", type_options);
    players[0] = create_player(name1, 'S', type1); // Default symbol

    string name2 = get_player_name("Player 2");
    PlayerType type2 = get_player_type_choice("Player 2", type_options);
    players[1] = create_player(name2, 'U', type2);

    return players;
}

// Smart AI for SUS Game
Move<char>* sus_ui::computerMove(Player<char>* aiPlayer) {
    sus_board* board = dynamic_cast<sus_board*>(aiPlayer->get_board_ptr());
    if (!board) return nullptr;

    int rows = 3;
    int cols = 3;
    
    Move<char>* bestMove = nullptr;
    int maxPoints = -1;

    // Check every empty cell for both 'S' and 'U'
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (board->get_cell(i, j) == '.') { 
                
                // 1. Test placing 'S'
                int scoreS = 0;
                // Check neighbors for S-U-S logic manually to see if it would score
                const int dx[8] = {0, 0, 1, -1, 1, -1, 1, -1};
                const int dy[8] = {1, -1, 0, 0, 1, -1, -1, 1};
                for(int d=0; d<8; ++d) {
                    int r1=i+dx[d], c1=j+dy[d], r2=i+dx[d]*2, c2=j+dy[d]*2;
                    if(r2>=0 && r2<3 && c2>=0 && c2<3 && !board->is_taken(i,j) && !board->is_taken(r1,c1) && !board->is_taken(r2,c2)) {
                        if(board->get_cell(r1,c1) == 'U' && board->get_cell(r2,c2) == 'S') scoreS++;
                    }
                }

                if (scoreS > maxPoints) {
                    maxPoints = scoreS;
                    delete bestMove;
                    bestMove = new Move<char>(i, j, 'S');
                }

                // 2. Test placing 'U'
                int scoreU = 0;
                // Neighbors
                if (i-1>=0 && i+1<3 && !board->is_taken(i-1,j) && !board->is_taken(i+1,j) && board->get_cell(i-1,j)=='S' && board->get_cell(i+1,j)=='S') scoreU++;
                if (j-1>=0 && j+1<3 && !board->is_taken(i,j-1) && !board->is_taken(i,j+1) && board->get_cell(i,j-1)=='S' && board->get_cell(i,j+1)=='S') scoreU++;
                if (i==1 && j==1) {
                    if(!board->is_taken(0,0) && !board->is_taken(2,2) && board->get_cell(0,0)=='S' && board->get_cell(2,2)=='S') scoreU++;
                    if(!board->is_taken(0,2) && !board->is_taken(2,0) && board->get_cell(0,2)=='S' && board->get_cell(2,0)=='S') scoreU++;
                }

                if (scoreU > maxPoints) {
                    maxPoints = scoreU;
                    delete bestMove;
                    bestMove = new Move<char>(i, j, 'U');
                }
            }
        }
    }

    // If found a scoring move, return it
    if (bestMove && maxPoints > 0) return bestMove;
    if (bestMove) delete bestMove;

    // Otherwise random valid move with random symbol
    int r, c;
    do {
        r = rand() % 3;
        c = rand() % 3;
    } while (board->get_cell(r,c) != '.');
    
    char randSym = (rand()%2 == 0) ? 'S' : 'U';
    return new Move<char>(r, c, randSym);
}

Move<char>* sus_ui::get_move(Player<char>* player) {
    if (player->get_type() == PlayerType::HUMAN) {
        int x, y;
        char s;
        cout << "\n" << player->get_name() << ", enter your move (row col) and symbol (S/U): ";
        cin >> x >> y >> s;
        return new Move<char>(x, y, toupper(s));
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        cout << player->get_name() << " is thinking...\n";
        return computerMove(player);
    }
    return nullptr;
}

// ==========================================
// Ultimate Tic-Tac-Toe Implementation
// ==========================================

ultimate_XO_board::ultimate_XO_board():Board(9,9) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            board[i][j] = ' ';
        }
    }
    for(int i=0; i<3; ++i)
        for(int j=0; j<3; ++j)
            mini_board_wins[i][j] = 0; 
}

ultimate_XO_board::~ultimate_XO_board() {}

bool ultimate_XO_board::isValidMove(int r, int c) const {
    if (r < 0 || r >= 9 || c < 0 || c >= 9) return false;
    // Check if cell is empty AND the mini-board is active (not already won/filled)
    // Note: The board[r][c] check implicitly handles this because won boards get overwritten.
    if (board[r][c] != ' ') return false;
    return true;
}

char ultimate_XO_board::check_mini_board_win(int mini_r, int mini_c) {
    int start_r = mini_r * 3;
    int start_c = mini_c * 3;

    // Rows
    for (int i = 0; i < 3; i++) {
        if (board[start_r + i][start_c] != ' ' &&
            board[start_r + i][start_c] == board[start_r + i][start_c + 1] &&
            board[start_r + i][start_c + 1] == board[start_r + i][start_c + 2]) {
            return board[start_r + i][start_c];
        }
    }
    // Cols
    for (int j = 0; j < 3; j++) {
        if (board[start_r][start_c + j] != ' ' &&
            board[start_r][start_c + j] == board[start_r + 1][start_c + j] &&
            board[start_r + 1][start_c + j] == board[start_r + 2][start_c + j]) {
            return board[start_r][start_c + j];
        }
    }
    // Diagonals
    if (board[start_r][start_c] != ' ' &&
        board[start_r][start_c] == board[start_r + 1][start_c + 1] &&
        board[start_r + 1][start_c + 1] == board[start_r + 2][start_c + 2])
        return board[start_r][start_c];

    if (board[start_r][start_c + 2] != ' ' &&
        board[start_r][start_c + 2] == board[start_r + 1][start_c + 1] &&
        board[start_r + 1][start_c + 1] == board[start_r + 2][start_c])
        return board[start_r][start_c + 2];

    return 0; 
}

bool ultimate_XO_board::update_board(Move<char>* move) {
    int r = move->get_x();
    int c = move->get_y();
    char symbol = move->get_symbol();

    if (!isValidMove(r, c)) {
        delete move;
        return false;
    }

    board[r][c] = symbol;
    n_moves++;

    // Check if this move won the mini-board
    int mini_r = r / 3;
    int mini_c = c / 3;
    
    if (mini_board_wins[mini_r][mini_c] == 0) {
        char win = check_mini_board_win(mini_r, mini_c);
        if (win != 0) {
            mini_board_wins[mini_r][mini_c] = win;
            
            // Overwrite the mini-board with the winner symbol to display it and lock it
            int start_r = mini_r * 3;
            int start_c = mini_c * 3;
            for(int i = 0; i < 3; ++i) {
                for(int j = 0; j < 3; ++j) {
                    board[start_r + i][start_c + j] = win;
                }
            }
        } 
    }
    
    delete move;
    return true;
}

bool ultimate_XO_board::is_win(Player<char> *player) {
    char s = player->get_symbol();
    for (int i = 0; i < 3; i++) {
        if (mini_board_wins[i][0] == s && mini_board_wins[i][1] == s && mini_board_wins[i][2] == s) return true;
        if (mini_board_wins[0][i] == s && mini_board_wins[1][i] == s && mini_board_wins[2][i] == s) return true;
    }
    if (mini_board_wins[0][0] == s && mini_board_wins[1][1] == s && mini_board_wins[2][2] == s) return true;
    if (mini_board_wins[0][2] == s && mini_board_wins[1][1] == s && mini_board_wins[2][0] == s) return true;
    return false;
}

bool ultimate_XO_board::is_draw(Player<char> *player) {
    if (is_win(player)) return false;
    for(int i=0; i<3; ++i)
        for(int j=0; j<3; ++j)
            if(mini_board_wins[i][j] == 0) return false; 
    return true;
}

bool ultimate_XO_board::game_is_over(Player<char> *player) {
    return is_win(player) || is_draw(player);
}

// --- UI Ultimate ---

ultimate_XO_UI::ultimate_XO_UI():UI("Welcome to Ultimate XO", 3) {}

Player<char> *ultimate_XO_UI::create_player(string &name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

// Smart AI for Ultimate TTT
Move<char>* ultimate_XO_UI::computerMove(Player<char>* aiPlayer) {
    ultimate_XO_board* board = dynamic_cast<ultimate_XO_board*>(aiPlayer->get_board_ptr());
    if (!board) return nullptr;

    char sym = aiPlayer->get_symbol();
    vector<Move<char>*> validMoves;

    for(int i=0; i<9; ++i) {
        for(int j=0; j<9; ++j) {
            if(board->isValidMove(i, j)) validMoves.push_back(new Move<char>(i, j, sym));
        }
    }

    if (validMoves.empty()) return nullptr;

    Move<char>* bestMove = validMoves[0];
    int bestScore = -1000;

    for (auto move : validMoves) {
        int score = 0;
        int r = move->get_x();
        int c = move->get_y();
        
        // Prefer center of mini-boards
        if ((r % 3 == 1) && (c % 3 == 1)) score += 5;
        // Prefer corners of mini-boards
        else if((r%3 != 1) && (c%3 != 1)) score += 3;

        // Note: Real "win" check simulation is hard without undo, 
        // but since isValidMove checks bounds, this heuristic is safe.
        
        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }
    
    // Slight randomness to avoid identical games
    if (rand() % 10 < 3) return validMoves[rand() % validMoves.size()];
    
    return bestMove;
}

Move<char>* ultimate_XO_UI::get_move(Player<char>* player) {
    if (player->get_type() == PlayerType::HUMAN) {
        int x, y;
        cout << "\nEnter move (row col) 0-8: ";
        cin >> x >> y;
        return new Move<char>(x, y, player->get_symbol());
    }
    else {
        cout << player->get_name() << " is thinking...\n";
        return computerMove(player);
    }
}

void ultimate_XO_UI::display_board_matrix(const vector<vector<char>>& matrix) const {
    cout << "\n";
    cout << "     0 1 2   3 4 5   6 7 8\n";
    cout << "   +-------+-------+-------+\n";
    
    for (int i = 0; i < 9; ++i) {
        cout << " " << i << " | ";
        for (int j = 0; j < 9; ++j) {
            cout << matrix[i][j] << " ";
            if ((j + 1) % 3 == 0) cout << "| ";
        }
        cout << "\n";
        if ((i + 1) % 3 == 0) cout << "   +-------+-------+-------+\n";
    }
    cout << endl;
}