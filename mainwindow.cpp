#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QTimer>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QRandomGenerator>
#include <QDateTime>
#include <QColorDialog>
#include <fstream>
#include <sstream>
#include <random>
#include <cmath>
#include <unordered_set>
#include <deque>

// ==========================================
// Game Board Implementations
// ==========================================

TTT_Board::TTT_Board(int r, int c, int win) : GameBoard(r, c, ' '), winCondition(win) {}

bool TTT_Board::update_board(int r, int c, char sym) {
    if(r < 0 || r >= rows || c < 0 || c >= columns || board[r][c] != blank_symbol)
        return false;
    board[r][c] = sym;
    history.push_back({r, c});
    n_moves++;
    return true;
}

bool TTT_Board::is_win(char sym) const {
    // Check rows
    for(int i = 0; i < rows; ++i) {
        for(int j = 0; j <= columns - winCondition; ++j) {
            bool win = true;
            for(int k = 0; k < winCondition; ++k) {
                if(board[i][j+k] != sym) { win = false; break; }
            }
            if(win) return true;
        }
    }

    // Check columns
    for(int i = 0; i <= rows - winCondition; ++i) {
        for(int j = 0; j < columns; ++j) {
            bool win = true;
            for(int k = 0; k < winCondition; ++k) {
                if(board[i+k][j] != sym) { win = false; break; }
            }
            if(win) return true;
        }
    }

    // Check diagonals
    for(int i = 0; i <= rows - winCondition; ++i) {
        for(int j = 0; j <= columns - winCondition; ++j) {
            bool win = true;
            for(int k = 0; k < winCondition; ++k) {
                if(board[i+k][j+k] != sym) { win = false; break; }
            }
            if(win) return true;
        }
    }

    // Check anti-diagonals
    for(int i = 0; i <= rows - winCondition; ++i) {
        for(int j = winCondition - 1; j < columns; ++j) {
            bool win = true;
            for(int k = 0; k < winCondition; ++k) {
                if(board[i+k][j-k] != sym) { win = false; break; }
            }
            if(win) return true;
        }
    }

    return false;
}

bool TTT_Board::is_draw() const {
    if(is_win('X') || is_win('O')) return false;
    return n_moves == rows * columns;
}

Misere_TTT_Board::Misere_TTT_Board() : TTT_Board(3, 3, 3) {}

bool Misere_TTT_Board::is_win(char sym) const {
    char opponent = (sym == 'X') ? 'O' : 'X';
    return TTT_Board::is_win(opponent);
}

C4_Board::C4_Board() : GameBoard(6, 7, ' ') {}

bool C4_Board::update_board(int r, int c, char sym) {
    int row = get_drop_row(c);
    if(row == -1) return false;
    board[row][c] = sym;
    history.push_back({row, c});
    n_moves++;
    return true;
}

int C4_Board::get_drop_row(int col) const {
    if(col < 0 || col >= columns) return -1;
    for(int r = rows - 1; r >= 0; --r) {
        if(board[r][col] == blank_symbol) return r;
    }
    return -1;
}

bool C4_Board::is_win(char sym) const {
    // Check horizontal
    for(int r = 0; r < rows; ++r) {
        for(int c = 0; c <= columns - 4; ++c) {
            if(board[r][c] == sym && board[r][c+1] == sym &&
                board[r][c+2] == sym && board[r][c+3] == sym)
                return true;
        }
    }

    // Check vertical
    for(int r = 0; r <= rows - 4; ++r) {
        for(int c = 0; c < columns; ++c) {
            if(board[r][c] == sym && board[r+1][c] == sym &&
                board[r+2][c] == sym && board[r+3][c] == sym)
                return true;
        }
    }

    // Check diagonals
    for(int r = 0; r <= rows - 4; ++r) {
        for(int c = 0; c <= columns - 4; ++c) {
            if(board[r][c] == sym && board[r+1][c+1] == sym &&
                board[r+2][c+2] == sym && board[r+3][c+3] == sym)
                return true;
        }
    }

    // Check anti-diagonals
    for(int r = 3; r < rows; ++r) {
        for(int c = 0; c <= columns - 4; ++c) {
            if(board[r][c] == sym && board[r-1][c+1] == sym &&
                board[r-2][c+2] == sym && board[r-3][c+3] == sym)
                return true;
        }
    }

    return false;
}

bool C4_Board::is_draw() const {
    for(int c = 0; c < columns; ++c) {
        if(board[0][c] == blank_symbol) return false;
    }
    return true;
}

Numerical_TTT_Board::Numerical_TTT_Board() : GameBoard(3, 3, ' ') {
    for(int i = 1; i <= 9; i += 2) odd_used.insert(i);
    for(int i = 2; i <= 8; i += 2) even_used.insert(i);
}

bool Numerical_TTT_Board::is_valid_number(char num, bool is_odd) const {
    int val = num - '0';
    if(val < 1 || val > 9) return false;
    if(is_odd) return (val % 2 == 1) && (odd_used.find(val) != odd_used.end());
    else return (val % 2 == 0) && (even_used.find(val) != even_used.end());
}

bool Numerical_TTT_Board::update_board(int r, int c, char sym) {
    if(r < 0 || r >= rows || c < 0 || c >= columns || board[r][c] != blank_symbol)
        return false;

    int num = sym - '0';
    if(is_odd_turn) {
        if(num % 2 == 0 || odd_used.find(num) == odd_used.end()) return false;
        odd_used.erase(num);
    } else {
        if(num % 2 == 1 || even_used.find(num) == even_used.end()) return false;
        even_used.erase(num);
    }

    board[r][c] = sym;
    history.push_back({r, c});
    n_moves++;
    is_odd_turn = !is_odd_turn;
    return true;
}

bool Numerical_TTT_Board::is_win(char sym) const {
    // Check rows
    for(int i = 0; i < 3; ++i) {
        if(board[i][0] != ' ' && board[i][1] != ' ' && board[i][2] != ' ') {
            int sum = (board[i][0] - '0') + (board[i][1] - '0') + (board[i][2] - '0');
            if(sum == 15) return true;
        }
    }

    // Check columns
    for(int j = 0; j < 3; ++j) {
        if(board[0][j] != ' ' && board[1][j] != ' ' && board[2][j] != ' ') {
            int sum = (board[0][j] - '0') + (board[1][j] - '0') + (board[2][j] - '0');
            if(sum == 15) return true;
        }
    }

    // Check diagonals
    if(board[0][0] != ' ' && board[1][1] != ' ' && board[2][2] != ' ') {
        int sum = (board[0][0] - '0') + (board[1][1] - '0') + (board[2][2] - '0');
        if(sum == 15) return true;
    }

    if(board[0][2] != ' ' && board[1][1] != ' ' && board[2][0] != ' ') {
        int sum = (board[0][2] - '0') + (board[1][1] - '0') + (board[2][0] - '0');
        if(sum == 15) return true;
    }

    return false;
}

bool Numerical_TTT_Board::is_draw() const {
    return n_moves == 9 && !is_win('X') && !is_win('O');
}

Obstacles_TTT_Board::Obstacles_TTT_Board() : GameBoard(6, 6, ' ') {
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, 5);

    for(int i = 0; i < 4; ++i) {
        int r, c;
        do {
            r = dist(rng);
            c = dist(rng);
        } while(board[r][c] != ' ');

        obstacles.push_back({r, c});
        board[r][c] = '#';
    }
}

bool Obstacles_TTT_Board::is_obstacle(int r, int c) const {
    for(const auto& obs : obstacles) {
        if(obs.r == r && obs.c == c) return true;
    }
    return false;
}

bool Obstacles_TTT_Board::update_board(int r, int c, char sym) {
    if(r < 0 || r >= rows || c < 0 || c >= columns ||
        board[r][c] != blank_symbol || is_obstacle(r, c))
        return false;

    board[r][c] = sym;
    history.push_back({r, c});
    n_moves++;
    return true;
}

bool Obstacles_TTT_Board::is_win(char sym) const {
    for(int r = 0; r < rows; ++r) {
        for(int c = 0; c <= columns - 4; ++c) {
            bool win = true;
            for(int k = 0; k < 4; ++k) {
                if(board[r][c+k] != sym) { win = false; break; }
            }
            if(win) return true;
        }
    }

    for(int r = 0; r <= rows - 4; ++r) {
        for(int c = 0; c < columns; ++c) {
            bool win = true;
            for(int k = 0; k < 4; ++k) {
                if(board[r+k][c] != sym) { win = false; break; }
            }
            if(win) return true;
        }
    }

    for(int r = 0; r <= rows - 4; ++r) {
        for(int c = 0; c <= columns - 4; ++c) {
            bool win = true;
            for(int k = 0; k < 4; ++k) {
                if(board[r+k][c+k] != sym) { win = false; break; }
            }
            if(win) return true;
        }
    }

    for(int r = 3; r < rows; ++r) {
        for(int c = 0; c <= columns - 4; ++c) {
            bool win = true;
            for(int k = 0; k < 4; ++k) {
                if(board[r-k][c+k] != sym) { win = false; break; }
            }
            if(win) return true;
        }
    }

    return false;
}

bool Obstacles_TTT_Board::is_draw() const {
    if(is_win('X') || is_win('O')) return false;
    for(int r = 0; r < rows; ++r) {
        for(int c = 0; c < columns; ++c) {
            if(board[r][c] == blank_symbol) return false;
        }
    }
    return true;
}

XO_4_x_4_Board::XO_4_x_4_Board() : GameBoard(4, 4, ' ') {}

bool XO_4_x_4_Board::update_board(int r, int c, char sym) {
    if(r < 0 || r >= rows || c < 0 || c >= columns || board[r][c] != blank_symbol)
        return false;
    board[r][c] = sym;
    history.push_back({r, c});
    n_moves++;
    return true;
}

bool XO_4_x_4_Board::is_win(char sym) const {
    for(int i = 0; i < rows; ++i) {
        for(int j = 0; j <= columns - 3; ++j) {
            if(board[i][j] == sym && board[i][j+1] == sym && board[i][j+2] == sym)
                return true;
        }
    }

    for(int i = 0; i <= rows - 3; ++i) {
        for(int j = 0; j < columns; ++j) {
            if(board[i][j] == sym && board[i+1][j] == sym && board[i+2][j] == sym)
                return true;
        }
    }

    for(int i = 0; i <= rows - 3; ++i) {
        for(int j = 0; j <= columns - 3; ++j) {
            if(board[i][j] == sym && board[i+1][j+1] == sym && board[i+2][j+2] == sym)
                return true;
            if(board[i][j+2] == sym && board[i+1][j+1] == sym && board[i+2][j] == sym)
                return true;
        }
    }

    return false;
}

bool XO_4_x_4_Board::is_draw() const {
    return n_moves == 16 && !is_win('X') && !is_win('O');
}

SUS_Board::SUS_Board() : GameBoard(3, 3, ' ') {}

bool SUS_Board::update_board(int r, int c, char sym) {
    if(r < 0 || r >= rows || c < 0 || c >= columns || board[r][c] != blank_symbol)
        return false;
    if(sym != 'S' && sym != 'U') return false;

    board[r][c] = sym;
    history.push_back({r, c});
    n_moves++;

    // Calculate scores
    s_score = u_score = 0;
    std::string sus = "SUS";

    // Check rows
    for(int i = 0; i < 3; ++i) {
        std::string row = "";
        for(int j = 0; j < 3; ++j) row += board[i][j];
        if(row == sus) {
            if(board[i][0] == 'S') s_score++;
            if(board[i][1] == 'U') u_score++;
            if(board[i][2] == 'S') s_score++;
        }
    }

    // Check columns
    for(int j = 0; j < 3; ++j) {
        std::string col = "";
        for(int i = 0; i < 3; ++i) col += board[i][j];
        if(col == sus) {
            if(board[0][j] == 'S') s_score++;
            if(board[1][j] == 'U') u_score++;
            if(board[2][j] == 'S') s_score++;
        }
    }

    // Check diagonals
    std::string diag1 = "", diag2 = "";
    for(int i = 0; i < 3; ++i) {
        diag1 += board[i][i];
        diag2 += board[i][2-i];
    }

    if(diag1 == sus) {
        if(board[0][0] == 'S') s_score++;
        if(board[1][1] == 'U') u_score++;
        if(board[2][2] == 'S') s_score++;
    }

    if(diag2 == sus) {
        if(board[0][2] == 'S') s_score++;
        if(board[1][1] == 'U') u_score++;
        if(board[2][0] == 'S') s_score++;
    }

    return true;
}

bool SUS_Board::is_win(char sym) const {
    if(sym == 'S') return s_score > u_score;
    if(sym == 'U') return u_score > s_score;
    return false;
}

bool SUS_Board::is_draw() const {
    return n_moves == 9 && s_score == u_score;
}

XO_5x5_Board::XO_5x5_Board(int win) : GameBoard(5, 5, ' '), winCondition(win) {}

bool XO_5x5_Board::update_board(int r, int c, char sym) {
    if(r < 0 || r >= rows || c < 0 || c >= columns || board[r][c] != blank_symbol)
        return false;
    board[r][c] = sym;
    history.push_back({r, c});
    n_moves++;
    return true;
}

bool XO_5x5_Board::is_win(char sym) const {
    for(int i = 0; i < rows; ++i) {
        for(int j = 0; j <= columns - winCondition; ++j) {
            bool win = true;
            for(int k = 0; k < winCondition; ++k) {
                if(board[i][j+k] != sym) { win = false; break; }
            }
            if(win) return true;
        }
    }

    for(int i = 0; i <= rows - winCondition; ++i) {
        for(int j = 0; j < columns; ++j) {
            bool win = true;
            for(int k = 0; k < winCondition; ++k) {
                if(board[i+k][j] != sym) { win = false; break; }
            }
            if(win) return true;
        }
    }

    for(int i = 0; i <= rows - winCondition; ++i) {
        for(int j = 0; j <= columns - winCondition; ++j) {
            bool win = true;
            for(int k = 0; k < winCondition; ++k) {
                if(board[i+k][j+k] != sym) { win = false; break; }
            }
            if(win) return true;
        }
    }

    for(int i = 0; i <= rows - winCondition; ++i) {
        for(int j = winCondition - 1; j < columns; ++j) {
            bool win = true;
            for(int k = 0; k < winCondition; ++k) {
                if(board[i+k][j-k] != sym) { win = false; break; }
            }
            if(win) return true;
        }
    }

    return false;
}

bool XO_5x5_Board::is_draw() const {
    return n_moves == 25 && !is_win('X') && !is_win('O');
}

Word_TTT_Board::Word_TTT_Board() : GameBoard(3, 3, ' ') {
    const char* words[] = {
        "CAT", "DOG", "BAT", "RAT", "MAT", "HAT", "SAT", "PAT", "FAT",
        "BET", "GET", "SET", "NET", "WET", "LET", "MET", "PET",
        "BIT", "FIT", "HIT", "SIT", "LIT", "PIT", "WIT",
        "COT", "DOT", "GOT", "HOT", "LOT", "NOT", "POT", "ROT", "SOT",
        "CUT", "GUT", "HUT", "NUT", "PUT", "RUT", "TUT",
        "ACE", "ICE", "AGE", "AWE", "DUE", "DYE", "EVE", "EYE", "FOE",
        "HUE", "LIE", "PIE", "RUE", "SEE", "TIE", "TOE", "VUE", "WOE",
        "ACT", "ART", "APT", "ANT", "AND", "ANY", "AIR", "ARM", "ASH",
        "ASK", "ASS", "ATE", "AIM", "ALE", "ALL", "AMP", "ANA", "ARC"
    };

    for(int i = 0; i < sizeof(words)/sizeof(words[0]); ++i) {
        dictionary.insert(words[i]);
    }
}

bool Word_TTT_Board::is_valid_letter(char c) const {
    return c >= 'A' && c <= 'Z';
}

bool Word_TTT_Board::update_board(int r, int c, char sym) {
    if(r < 0 || r >= rows || c < 0 || c >= columns || board[r][c] != blank_symbol)
        return false;
    if(!is_valid_letter(sym)) return false;

    board[r][c] = sym;
    history.push_back({r, c});
    n_moves++;
    return true;
}

bool Word_TTT_Board::is_win(char sym) const {
    // Check rows
    for(int i = 0; i < 3; ++i) {
        std::string word = "";
        for(int j = 0; j < 3; ++j) word += board[i][j];
        if(dictionary.find(word) != dictionary.end()) return true;
    }

    // Check columns
    for(int j = 0; j < 3; ++j) {
        std::string word = "";
        for(int i = 0; i < 3; ++i) word += board[i][j];
        if(dictionary.find(word) != dictionary.end()) return true;
    }

    // Check diagonals
    std::string diag1 = "", diag2 = "";
    for(int i = 0; i < 3; ++i) {
        diag1 += board[i][i];
        diag2 += board[i][2-i];
    }

    if(dictionary.find(diag1) != dictionary.end()) return true;
    if(dictionary.find(diag2) != dictionary.end()) return true;

    return false;
}

bool Word_TTT_Board::is_draw() const {
    return n_moves == 9 && !is_win('X') && !is_win('O');
}

Ultimate_XO_Board::Ultimate_XO_Board() : GameBoard(9, 9, ' ') {
    for(auto& row : board) std::fill(row.begin(), row.end(), ' ');
}

bool Ultimate_XO_Board::update_board(int r, int c, char sym) {
    if(r < 0 || r >= rows || c < 0 || c >= columns || board[r][c] != ' ')
        return false;

    int mini_r = r / 3;
    int mini_c = c / 3;
    if(mini_wins[mini_r][mini_c] != 0) return false;

    board[r][c] = sym;
    history.push_back({r, c});
    n_moves++;

    return true;
}

bool Ultimate_XO_Board::is_win(char sym) const {
    for(int i = 0; i < 3; ++i) {
        if(mini_wins[i][0] == sym && mini_wins[i][1] == sym && mini_wins[i][2] == sym)
            return true;
    }

    for(int j = 0; j < 3; ++j) {
        if(mini_wins[0][j] == sym && mini_wins[1][j] == sym && mini_wins[2][j] == sym)
            return true;
    }

    if(mini_wins[0][0] == sym && mini_wins[1][1] == sym && mini_wins[2][2] == sym)
        return true;

    if(mini_wins[0][2] == sym && mini_wins[1][1] == sym && mini_wins[2][0] == sym)
        return true;

    return false;
}

bool Ultimate_XO_Board::is_draw() const {
    if(is_win('X') || is_win('O')) return false;
    for(int i = 0; i < 3; ++i) {
        for(int j = 0; j < 3; ++j) {
            if(mini_wins[i][j] == 0) return false;
        }
    }
    return true;
}

Infinity_TTT_Board::Infinity_TTT_Board() : GameBoard(3, 3, ' ') {}

bool Infinity_TTT_Board::update_board(int r, int c, char sym) {
    if(r < 0 || r >= rows || c < 0 || c >= columns || board[r][c] != blank_symbol)
        return false;

    MoveInfo info;
    info.placed = {r, c};
    info.removed = false;

    board[r][c] = sym;
    move_queue.push_back({r, c});
    history.push_back({r, c});
    history_info.push_back(info);
    n_moves++;

    if(n_moves >= 4 && (n_moves - 1) % 3 == 0) {
        if(!move_queue.empty()) {
            Point oldest = move_queue.front();
            move_queue.pop_front();

            history_info.back().removed = true;
            history_info.back().removed_pos = oldest;
            history_info.back().removed_sym = board[oldest.r][oldest.c];

            board[oldest.r][oldest.c] = blank_symbol;
        }
    }

    return true;
}

void Infinity_TTT_Board::undoLastMove() {
    if(history.empty()) return;

    Point last = history.back();
    history.pop_back();
    MoveInfo info = history_info.back();
    history_info.pop_back();

    board[last.r][last.c] = blank_symbol;

    if(!move_queue.empty() && move_queue.back().r == last.r && move_queue.back().c == last.c) {
        move_queue.pop_back();
    }

    if(info.removed) {
        board[info.removed_pos.r][info.removed_pos.c] = info.removed_sym;
        move_queue.push_front(info.removed_pos);
    }

    n_moves--;
}

bool Infinity_TTT_Board::is_win(char sym) const {
    for(int i = 0; i < 3; ++i) {
        if(board[i][0] == sym && board[i][1] == sym && board[i][2] == sym)
            return true;
    }

    for(int j = 0; j < 3; ++j) {
        if(board[0][j] == sym && board[1][j] == sym && board[2][j] == sym)
            return true;
    }

    if(board[0][0] == sym && board[1][1] == sym && board[2][2] == sym)
        return true;

    if(board[0][2] == sym && board[1][1] == sym && board[2][0] == sym)
        return true;

    return false;
}

bool Infinity_TTT_Board::is_draw() const {
    return false;
}

Pyramid_X_O_Board::Pyramid_X_O_Board() : GameBoard(3, 5, ' ') {}

bool Pyramid_X_O_Board::is_valid_position(int r, int c) const {
    if(r == 0) return c == 2;
    if(r == 1) return c >= 1 && c <= 3;
    if(r == 2) return c >= 0 && c <= 4;
    return false;
}

bool Pyramid_X_O_Board::update_board(int r, int c, char sym) {
    if(!is_valid_position(r, c) || board[r][c] != blank_symbol)
        return false;

    board[r][c] = sym;
    history.push_back({r, c});
    n_moves++;
    return true;
}

bool Pyramid_X_O_Board::is_win(char sym) const {
    if(board[0][2] == sym && board[1][2] == sym && board[2][2] == sym)
        return true;

    if(board[1][1] == sym && board[1][2] == sym && board[1][3] == sym)
        return true;

    if(board[2][0] == sym && board[2][1] == sym && board[2][2] == sym)
        return true;

    if(board[2][1] == sym && board[2][2] == sym && board[2][3] == sym)
        return true;

    if(board[2][2] == sym && board[2][3] == sym && board[2][4] == sym)
        return true;

    if(board[0][2] == sym && board[1][1] == sym && board[2][0] == sym)
        return true;

    if(board[0][2] == sym && board[1][3] == sym && board[2][4] == sym)
        return true;

    return false;
}

bool Pyramid_X_O_Board::is_draw() const {
    int valid_positions = 0;
    int filled = 0;

    for(int r = 0; r < 3; ++r) {
        for(int c = 0; c < 5; ++c) {
            if(is_valid_position(r, c)) {
                valid_positions++;
                if(board[r][c] != blank_symbol) filled++;
            }
        }
    }

    return filled == valid_positions && !is_win('X') && !is_win('O');
}

Diamond_XO_Board::Diamond_XO_Board() : GameBoard(7, 7, ' ') {}

bool Diamond_XO_Board::is_valid_position(int r, int c) const {
    return std::abs(r - 3) + std::abs(c - 3) <= 3;
}

bool Diamond_XO_Board::update_board(int r, int c, char sym) {
    if(!is_valid_position(r, c) || board[r][c] != blank_symbol)
        return false;

    board[r][c] = sym;
    history.push_back({r, c});
    n_moves++;
    return true;
}

bool Diamond_XO_Board::is_win(char sym) const {
    // Check for 3 in a row
    for(int r = 0; r < rows; ++r) {
        for(int c = 0; c <= columns - 3; ++c) {
            bool valid = true;
            for(int k = 0; k < 3; ++k) {
                if(!is_valid_position(r, c+k) || board[r][c+k] != sym) {
                    valid = false;
                    break;
                }
            }
            if(valid) return true;
        }
    }

    // Check for 4 in a row
    for(int r = 0; r <= rows - 4; ++r) {
        for(int c = 0; c < columns; ++c) {
            bool valid = true;
            for(int k = 0; k < 4; ++k) {
                if(!is_valid_position(r+k, c) || board[r+k][c] != sym) {
                    valid = false;
                    break;
                }
            }
            if(valid) return true;
        }
    }

    return false;
}

bool Diamond_XO_Board::is_draw() const {
    int valid_positions = 0;
    int filled = 0;

    for(int r = 0; r < rows; ++r) {
        for(int c = 0; c < columns; ++c) {
            if(is_valid_position(r, c)) {
                valid_positions++;
                if(board[r][c] != blank_symbol) filled++;
            }
        }
    }

    return filled == valid_positions && !is_win('X') && !is_win('O');
}

Memory_XO_Board::Memory_XO_Board() : GameBoard(3, 3, ' ') {
    revealed = std::vector<std::vector<bool>>(3, std::vector<bool>(3, false));
}

bool Memory_XO_Board::update_board(int r, int c, char sym) {
    if(r < 0 || r >= rows || c < 0 || c >= columns || board[r][c] != blank_symbol)
        return false;

    board[r][c] = sym;
    revealed[r][c] = true;
    history.push_back({r, c});
    n_moves++;
    return true;
}

bool Memory_XO_Board::is_revealed(int r, int c) const {
    return revealed[r][c];
}

void Memory_XO_Board::toggle_reveal(int r, int c) {
    if(r >= 0 && r < 3 && c >= 0 && c < 3)
        revealed[r][c] = !revealed[r][c];
}

bool Memory_XO_Board::is_win(char sym) const {
    for(int i = 0; i < 3; ++i) {
        if(board[i][0] == sym && board[i][1] == sym && board[i][2] == sym)
            return true;
    }

    for(int j = 0; j < 3; ++j) {
        if(board[0][j] == sym && board[1][j] == sym && board[2][j] == sym)
            return true;
    }

    if(board[0][0] == sym && board[1][1] == sym && board[2][2] == sym)
        return true;

    if(board[0][2] == sym && board[1][1] == sym && board[2][0] == sym)
        return true;

    return false;
}

bool Memory_XO_Board::is_draw() const {
    return n_moves == 9 && !is_win('X') && !is_win('O');
}

// ==========================================
// AI Implementation
// ==========================================

std::mt19937 GameAI::rng(std::random_device{}());

template<typename BoardType>
Point GameAI::get_best_move(const BoardType* board, char ai_sym, char human_sym) {
    auto moves = board->get_valid_moves();
    if(moves.empty()) return {-1, -1};

    std::vector<Point> win_moves;
    std::vector<Point> block_moves;
    std::vector<Point> center_moves;
    std::vector<Point> corner_moves;

    for(const auto& move : moves) {
        // Try win
        BoardType temp_board = *board;
        temp_board.update_board(move.r, move.c, ai_sym);
        if(temp_board.is_win(ai_sym)) {
            win_moves.push_back(move);
        }

        // Try block
        temp_board = *board;
        temp_board.update_board(move.r, move.c, human_sym);
        if(temp_board.is_win(human_sym)) {
            block_moves.push_back(move);
        }

        // Categorize by position
        int rows = board->get_rows();
        int cols = board->get_columns();

        if(rows % 2 == 1 && cols % 2 == 1) {
            if(move.r == rows/2 && move.c == cols/2) {
                center_moves.push_back(move);
            }
        }

        if((move.r == 0 || move.r == rows-1) && (move.c == 0 || move.c == cols-1)) {
            corner_moves.push_back(move);
        }
    }

    if(!win_moves.empty()) return win_moves[0];
    if(!block_moves.empty()) return block_moves[0];
    if(!center_moves.empty()) return center_moves[0];
    if(!corner_moves.empty()) return corner_moves[0];

    std::uniform_int_distribution<int> dist(0, moves.size()-1);
    return moves[dist(rng)];
}

template<typename BoardType>
Point GameAI::get_random_move(const BoardType* board) {
    auto moves = board->get_valid_moves();
    if(moves.empty()) return {-1, -1};

    std::uniform_int_distribution<int> dist(0, moves.size()-1);
    return moves[dist(rng)];
}

Point GameAI::get_connect4_move(const C4_Board* board, char ai_sym, char human_sym) {
    // Prefer center columns
    std::vector<int> preferred = {3, 2, 4, 1, 5, 0, 6};
    for(int c : preferred) {
        for(int r = 5; r >= 0; --r) {
            if(board->get_cell(r, c) == ' ') {
                return {0, c};
            }
        }
    }
    return {0, 3};
}

Point GameAI::get_sus_move(const SUS_Board* board, char ai_sym, char human_sym) {
    auto moves = board->get_valid_moves();
    if(moves.empty()) return {-1, -1};

    if(ai_sym == 'S') {
        std::vector<Point> corners = {{0,0}, {0,2}, {2,0}, {2,2}};
        for(const auto& corner : corners) {
            if(board->get_cell(corner.r, corner.c) == ' ') {
                return corner;
            }
        }
    } else if(ai_sym == 'U') {
        if(board->get_cell(1, 1) == ' ') {
            return {1, 1};
        }
    }

    std::uniform_int_distribution<int> dist(0, moves.size()-1);
    return moves[dist(rng)];
}

Point GameAI::get_word_move(const Word_TTT_Board* board, char ai_sym, char human_sym) {
    auto moves = board->get_valid_moves();
    if(moves.empty()) return {-1, -1};

    std::uniform_int_distribution<int> dist(0, moves.size()-1);
    return moves[dist(rng)];
}

Point GameAI::get_numerical_move(const Numerical_TTT_Board* board, char ai_sym, char human_sym, bool is_odd) {
    auto moves = board->get_valid_moves();
    if(moves.empty()) return {-1, -1};

    if(board->get_cell(1, 1) == ' ') {
        return {1, 1};
    }

    std::uniform_int_distribution<int> dist(0, moves.size()-1);
    return moves[dist(rng)];
}

// ==========================================
// MainWindow Implementation
// ==========================================

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), memoryTimer(nullptr)
{
    ui->setupUi(this);
    applyModernStyle();
    this->setWindowTitle("🎮 Board Game Collection");
    this->setMinimumSize(900, 700);

    setupGameButtons();

    connect(ui->btnStart, &QPushButton::clicked, this, &MainWindow::onStartGameClicked);
    connect(ui->btnBack, &QPushButton::clicked, this, &MainWindow::onBackClicked);
    connect(ui->btnBackGame, &QPushButton::clicked, this, &MainWindow::onBackClicked);
    connect(ui->btnUndo, &QPushButton::clicked, this, &MainWindow::onUndoClicked);
    connect(ui->btnHint, &QPushButton::clicked, this, &MainWindow::onHintClicked);

    ui->stackedWidget->setCurrentIndex(0);

    ui->txtPlayer1->setText("Player 1");
    ui->txtPlayer2->setText("Player 2");

    GameAI::init();
}

MainWindow::~MainWindow() {
    delete ui;
    delete players[0];
    delete players[1];
    if(memoryTimer) delete memoryTimer;
}

void MainWindow::applyModernStyle() {
    QString style = R"(
        QMainWindow {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #0f0c29, stop:0.5 #302b63, stop:1 #24243e);
        }
        QLabel {
            color: #ffffff;
            font-family: 'Segoe UI', Arial, sans-serif;
        }
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #667eea, stop:1 #764ba2);
            color: white;
            border: none;
            border-radius: 15px;
            padding: 15px 25px;
            font-size: 16px;
            font-weight: bold;
            margin: 8px;
            min-height: 60px;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #764ba2, stop:1 #667eea);
        }
        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #5a3d8a, stop:1 #4f6ae8);
        }
        QLineEdit {
            background: rgba(255, 255, 255, 0.15);
            border: 2px solid #667eea;
            border-radius: 12px;
            padding: 12px;
            color: white;
            font-size: 16px;
            font-family: 'Segoe UI', Arial;
        }
        QLineEdit:focus {
            border: 2px solid #00d4ff;
            background: rgba(255, 255, 255, 0.2);
        }
        QRadioButton {
            color: white;
            font-size: 16px;
            font-family: 'Segoe UI', Arial;
            spacing: 15px;
            padding: 8px;
        }
        QRadioButton::indicator {
            width: 22px;
            height: 22px;
            border-radius: 11px;
            border: 3px solid #667eea;
        }
        QRadioButton::indicator:checked {
            background-color: #00d4ff;
            border: 3px solid #00d4ff;
        }
        QGroupBox {
            color: #00d4ff;
            font-weight: bold;
            font-size: 18px;
            font-family: 'Segoe UI', Arial;
            border: 3px solid #667eea;
            border-radius: 15px;
            margin-top: 15px;
            padding-top: 20px;
            background: rgba(0, 0, 0, 0.3);
        }
    )";

    this->setStyleSheet(style);
}

void MainWindow::setupGameButtons() {
    QList<QPair<QPushButton*, QString>> games = {
        {ui->btnGame1, "🎯 SUS Game"},
        {ui->btnGame2, "🔴 Connect 4"},
        {ui->btnGame3, "🔶 5x5 TTT"},
        {ui->btnGame4, "📝 Word TTT"},
        {ui->btnGame5, "⚠️ Misère TTT"},
        {ui->btnGame6, "💎 Diamond TTT"},
        {ui->btnGame7, "🟦 4x4 TTT"},
        {ui->btnGame8, "🔺 Pyramid TTT"},
        {ui->btnGame9, "🔢 Numerical TTT"},
        {ui->btnGame10, "🚧 Obstacles TTT"},
        {ui->btnGame11, "♾️ Infinity TTT"},
        {ui->btnGame12, "🌌 Ultimate TTT"},
        {ui->btnGame13, "🧠 Memory TTT"},
        {ui->btnGame14, "⭕ Classic TTT"}
    };

    for(int i = 0; i < games.size(); ++i) {
        games[i].first->setText(games[i].second);
        games[i].first->setCursor(Qt::PointingHandCursor);

        auto shadow = new QGraphicsDropShadowEffect;
        shadow->setBlurRadius(20);
        shadow->setColor(QColor(102, 126, 234, 150));
        shadow->setOffset(4, 4);
        games[i].first->setGraphicsEffect(shadow);

        connect(games[i].first, &QPushButton::clicked, [this, id = i + 1]() {
            onGameButtonClicked(id);
        });
    }
}

void MainWindow::onGameButtonClicked(int gameId) {
    selectedGameId = gameId;
    ui->stackedWidget->setCurrentIndex(1);

    QString gameName = getGameName(gameId);
    QString gameDesc = getGameDescription(gameId);
    ui->lblGameTitle->setText("<h1>🎮 " + gameName + "</h1><p style='color:#cccccc; font-size:14px;'>" + gameDesc + "</p>");

    if(gameId == 1) {
        ui->txtPlayer1->setPlaceholderText("Enter name (plays S)");
        ui->txtPlayer2->setPlaceholderText("Enter name (plays U)");
    } else if(gameId == 4) {
        ui->txtPlayer1->setPlaceholderText("Enter name");
        ui->txtPlayer2->setPlaceholderText("Enter name");
    } else {
        ui->txtPlayer1->setPlaceholderText("Enter name (plays X)");
        ui->txtPlayer2->setPlaceholderText("Enter name (plays O)");
    }
}

QString MainWindow::getGameName(int gameId) const {
    QMap<int, QString> names = {
        {1, "SUS Game"}, {2, "Connect 4"}, {3, "5x5 TTT"}, {4, "Word TTT"},
        {5, "Misère TTT"}, {6, "Diamond TTT"}, {7, "4x4 TTT"}, {8, "Pyramid TTT"},
        {9, "Numerical TTT"}, {10, "Obstacles TTT"}, {11, "Infinity TTT"},
        {12, "Ultimate TTT"}, {13, "Memory TTT"}, {14, "Classic TTT"}
    };
    return names.value(gameId, "Game " + QString::number(gameId));
}

QString MainWindow::getGameDescription(int gameId) const {
    QMap<int, QString> desc = {
        {1, "Form 'SUS' patterns to score points"},
        {2, "Connect 4 pieces vertically, horizontally, or diagonally"},
        {3, "5x5 grid, need 4 in a row to win"},
        {4, "Form valid 3-letter English words"},
        {5, "Avoid making 3 in a row - last to complete loses"},
        {6, "7x7 diamond shape, need both 3 and 4 in a row"},
        {7, "4x4 grid, need 3 in a row to win"},
        {8, "Pyramid shape (3 rows: 1-3-5 cells)"},
        {9, "Use numbers (odd/even) to sum to 15"},
        {10, "6x6 grid with obstacles, need 4 in a row"},
        {11, "Oldest pieces disappear every 3 moves"},
        {12, "9x9 ultimate grid with 3x3 mini-boards"},
        {13, "Memory challenge - marks are hidden"},
        {14, "Classic 3x3 Tic-Tac-Toe"}
    };
    return desc.value(gameId, "");
}

void MainWindow::onStartGameClicked() {
    resetGame();

    currentBoard = createGameBoard(selectedGameId);
    if(!currentBoard) {
        QMessageBox::warning(this, "Error", "Failed to create game");
        return;
    }

    QString p1Name = ui->txtPlayer1->text().isEmpty() ?
                         (selectedGameId == 1 ? "Player S" : "Player X") : ui->txtPlayer1->text();

    Player* p1 = new Player(p1Name.toStdString(),
                            selectedGameId == 1 ? 'S' : 'X',
                            PlayerType::HUMAN);

    Player* p2;
    if(ui->radioHumanVsAI->isChecked()) {
        QString aiName = selectedGameId == 1 ? "AI (U)" : "AI (O)";
        p2 = new Player(aiName.toStdString(),
                        selectedGameId == 1 ? 'U' : 'O',
                        PlayerType::COMPUTER);
    } else {
        QString p2Name = ui->txtPlayer2->text().isEmpty() ?
                             (selectedGameId == 1 ? "Player U" : "Player O") : ui->txtPlayer2->text();
        p2 = new Player(p2Name.toStdString(),
                        selectedGameId == 1 ? 'U' : 'O',
                        PlayerType::HUMAN);
    }

    players[0] = p1;
    players[1] = p2;

    createBoardGrid(currentBoard->get_rows(), currentBoard->get_columns());

    currentPlayerIndex = 0;
    gameActive = true;
    moveCount = 0;

    updateGameInfo();
    updateBoardUI();
    ui->stackedWidget->setCurrentIndex(2);

    if(selectedGameId == 13) {
        if(memoryTimer) delete memoryTimer;
        memoryTimer = new QTimer(this);
        connect(memoryTimer, &QTimer::timeout, this, &MainWindow::hideMemoryCells);
    }

    if(players[currentPlayerIndex]->get_type() == PlayerType::COMPUTER) {
        QTimer::singleShot(500, this, &MainWindow::processComputerTurn);
    }
}

std::unique_ptr<GameBoard> MainWindow::createGameBoard(int gameId) {
    switch(gameId) {
    case 1:  return std::make_unique<SUS_Board>();
    case 2:  return std::make_unique<C4_Board>();
    case 3:  return std::make_unique<XO_5x5_Board>(4);
    case 4:  return std::make_unique<Word_TTT_Board>();
    case 5:  return std::make_unique<Misere_TTT_Board>();
    case 6:  return std::make_unique<Diamond_XO_Board>();
    case 7:  return std::make_unique<XO_4_x_4_Board>();
    case 8:  return std::make_unique<Pyramid_X_O_Board>();
    case 9:  return std::make_unique<Numerical_TTT_Board>();
    case 10: return std::make_unique<Obstacles_TTT_Board>();
    case 11: return std::make_unique<Infinity_TTT_Board>();
    case 12: return std::make_unique<Ultimate_XO_Board>();
    case 13: return std::make_unique<Memory_XO_Board>();
    case 14: return std::make_unique<TTT_Board>(3, 3, 3);
    default: return std::make_unique<TTT_Board>(3, 3, 3);
    }
}
void MainWindow::createBoardGrid(int rows, int cols) {
    qDeleteAll(gridButtons);
    gridButtons.clear();

    QLayoutItem* item;
    while((item = ui->gridLayoutGame->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // Calculate button size based on available space
    QSize gridWidgetSize = ui->gridWidget->size();
    int maxWidth = (gridWidgetSize.width() - ui->gridLayoutGame->spacing() * (cols - 1)) / cols;
    int maxHeight = (gridWidgetSize.height() - ui->gridLayoutGame->spacing() * (rows - 1)) / rows;
    int maxSize = qMin(qMin(maxWidth, maxHeight), 80); // Maximum 80x80, minimum 40x40
    maxSize = qMax(maxSize, 40);

    for(int r = 0; r < rows; ++r) {
        for(int c = 0; c < cols; ++c) {
            QPushButton* btn = new QPushButton();
            btn->setFixedSize(maxSize, maxSize);
            btn->setProperty("row", r);
            btn->setProperty("col", c);

            // Simplified style
            btn->setStyleSheet(R"(
                QPushButton {
                    background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                        stop:0 rgba(44, 62, 80, 0.8), stop:1 rgba(52, 73, 94, 0.8));
                    border-radius: 8px;
                    border: 2px solid #3498db;
                    font-size: 18px;
                    font-weight: bold;
                    color: white;
                }
                QPushButton:hover {
                    background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                        stop:0 rgba(52, 152, 219, 0.9), stop:1 rgba(41, 128, 185, 0.9));
                    border: 2px solid #1abc9c;
                }
            )");

            auto shadow = new QGraphicsDropShadowEffect;
            shadow->setBlurRadius(8);
            shadow->setColor(QColor(52, 152, 219, 100));
            shadow->setOffset(2, 2);
            btn->setGraphicsEffect(shadow);

            connect(btn, &QPushButton::clicked, this, &MainWindow::onGridCellClicked);

            ui->gridLayoutGame->addWidget(btn, r, c);
            gridButtons.append(btn);
        }
    }

    // Update the grid widget size to fit content
    ui->gridWidget->setMinimumSize(cols * maxSize + ui->gridLayoutGame->spacing() * (cols - 1),
                                   rows * maxSize + ui->gridLayoutGame->spacing() * (rows - 1));
}
void MainWindow::onGridCellClicked() {
    if(!gameActive) return;

    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if(!btn) return;

    int r = btn->property("row").toInt();
    int c = btn->property("col").toInt();

    Player* currentPlayer = players[currentPlayerIndex];
    if(currentPlayer->get_type() == PlayerType::COMPUTER) return;

    char symbol = currentPlayer->get_symbol();

    if(selectedGameId == 4) {
        char letter = getWordInput();
        if(letter == '\0') return;
        symbol = letter;
    } else if(selectedGameId == 9) {
        bool is_odd = (currentPlayer->get_symbol() == 'X');
        char number = getNumberInput(is_odd);
        if(number == '\0') return;
        symbol = number;
    } else if(selectedGameId == 1) {
        char sus = getSUSInput();
        if(sus == '\0') return;
        symbol = sus;
    }

    performMove(r, c, symbol);
}

char MainWindow::getWordInput() {
    bool ok;
    QString text = QInputDialog::getText(this, "Word TTT - Enter Letter",
                                         "Enter a letter (A-Z):",
                                         QLineEdit::Normal, "", &ok);
    if(ok && !text.isEmpty()) {
        char letter = text.toUpper().at(0).toLatin1();
        if(letter >= 'A' && letter <= 'Z') {
            return letter;
        }
    }
    return '\0';
}

char MainWindow::getNumberInput(bool is_odd) {
    QString title = is_odd ? "Enter Odd Number (1,3,5,7,9)" : "Enter Even Number (2,4,6,8)";

    bool ok;
    QString text = QInputDialog::getText(this, "Numerical TTT - Enter Number",
                                         title, QLineEdit::Normal, "", &ok);
    if(ok && !text.isEmpty()) {
        bool valid;
        int num = text.toInt(&valid);
        if(valid) {
            if(is_odd && num % 2 == 1 && num >= 1 && num <= 9) {
                return '0' + num;
            } else if(!is_odd && num % 2 == 0 && num >= 2 && num <= 8) {
                return '0' + num;
            }
        }
    }
    return '\0';
}

char MainWindow::getSUSInput() {
    QStringList items;
    items << "S" << "U";

    bool ok;
    QString choice = QInputDialog::getItem(this, "SUS Game - Choose Symbol",
                                           "Select symbol to place:", items, 0, false, &ok);
    if(ok && !choice.isEmpty()) {
        return choice.at(0).toLatin1();
    }
    return '\0';
}

void MainWindow::performMove(int r, int c, char symbol) {
    if(selectedGameId == 2) {
        C4_Board* c4Board = dynamic_cast<C4_Board*>(currentBoard.get());
        if(c4Board) {
            int actualRow = c4Board->get_drop_row(c);
            if(actualRow == -1) {
                if(c < gridButtons.size()) {
                    QPropertyAnimation* anim = new QPropertyAnimation(gridButtons[c], "geometry");
                    anim->setDuration(300);
                    QRect start = gridButtons[c]->geometry();
                    anim->setKeyValueAt(0, start);
                    anim->setKeyValueAt(0.2, start.translated(-5, 0));
                    anim->setKeyValueAt(0.4, start.translated(5, 0));
                    anim->setKeyValueAt(0.6, start.translated(-5, 0));
                    anim->setKeyValueAt(0.8, start.translated(5, 0));
                    anim->setKeyValueAt(1, start);
                    anim->start(QAbstractAnimation::DeleteWhenStopped);
                }
                return;
            }
            r = actualRow;
        }
    }

    if(!currentBoard->update_board(r, c, symbol)) {
        int index = r * currentBoard->get_columns() + c;
        if(index < gridButtons.size()) {
            QPropertyAnimation* anim = new QPropertyAnimation(gridButtons[index], "geometry");
            anim->setDuration(300);
            QRect start = gridButtons[index]->geometry();
            anim->setKeyValueAt(0, start);
            anim->setKeyValueAt(0.2, start.translated(-5, 0));
            anim->setKeyValueAt(0.4, start.translated(5, 0));
            anim->setKeyValueAt(0.6, start.translated(-5, 0));
            anim->setKeyValueAt(0.8, start.translated(5, 0));
            anim->setKeyValueAt(1, start);
            anim->start(QAbstractAnimation::DeleteWhenStopped);
        }
        return;
    }

    moveCount++;

    if(selectedGameId == 13) {
        Memory_XO_Board* memoryBoard = dynamic_cast<Memory_XO_Board*>(currentBoard.get());
        if(memoryBoard) {
            memoryBoard->toggle_reveal(r, c);
            if(memoryTimer) {
                memoryTimer->start(2000);
            }
        }
    }

    animateButton(gridButtons[r * currentBoard->get_columns() + c]);
    updateBoardUI();

    if(currentBoard->is_win(symbol)) {
        QString winner = QString::fromStdString(players[currentPlayerIndex]->get_name());

        if(selectedGameId == 5) {
            QString loser = QString::fromStdString(players[1 - currentPlayerIndex]->get_name());
            handleGameOver("🎉 " + loser + " wins! (Misère: " + winner + " made 3 in a row)");
        } else {
            handleGameOver("🎉 " + winner + " wins!");
        }
        return;
    }

    if(currentBoard->is_draw()) {
        handleGameOver("🤝 It's a draw!");
        return;
    }

    switchPlayer();

    if(players[currentPlayerIndex]->get_type() == PlayerType::COMPUTER && gameActive) {
        QTimer::singleShot(800, this, &MainWindow::processComputerTurn);
    }
}

void MainWindow::processComputerTurn() {
    if(!gameActive) return;

    Point move = getAIMove();
    if(move.r != -1 && move.c != -1) {
        Player* aiPlayer = players[currentPlayerIndex];
        char symbol = aiPlayer->get_symbol();

        if(selectedGameId == 4) {
            symbol = 'A' + (QRandomGenerator::global()->bounded(26));
        } else if(selectedGameId == 9) {
            bool is_odd = (aiPlayer->get_symbol() == 'X');
            if(is_odd) {
                symbol = '0' + (1 + 2 * QRandomGenerator::global()->bounded(5));
            } else {
                symbol = '0' + (2 + 2 * QRandomGenerator::global()->bounded(4));
            }
        } else if(selectedGameId == 1) {
            symbol = QRandomGenerator::global()->bounded(2) == 0 ? 'S' : 'U';
        }

        performMove(move.r, move.c, symbol);
    }
}

Point MainWindow::getAIMove() {
    if(!currentBoard) return {-1, -1};

    Player* aiPlayer = players[currentPlayerIndex];
    Player* humanPlayer = players[1 - currentPlayerIndex];

    char aiSym = aiPlayer->get_symbol();
    char humanSym = humanPlayer->get_symbol();

    switch(selectedGameId) {
    case 1: {
        SUS_Board* susBoard = dynamic_cast<SUS_Board*>(currentBoard.get());
        if(susBoard) return GameAI::get_sus_move(susBoard, aiSym, humanSym);
        break;
    }
    case 2: {
        C4_Board* c4Board = dynamic_cast<C4_Board*>(currentBoard.get());
        if(c4Board) return GameAI::get_connect4_move(c4Board, aiSym, humanSym);
        break;
    }
    case 4: {
        Word_TTT_Board* wordBoard = dynamic_cast<Word_TTT_Board*>(currentBoard.get());
        if(wordBoard) return GameAI::get_word_move(wordBoard, aiSym, humanSym);
        break;
    }
    case 9: {
        Numerical_TTT_Board* numBoard = dynamic_cast<Numerical_TTT_Board*>(currentBoard.get());
        if(numBoard) {
            bool is_odd = (aiSym == 'X');
            return GameAI::get_numerical_move(numBoard, aiSym, humanSym, is_odd);
        }
        break;
    }
    default:
        return GameAI::get_random_move(currentBoard.get());
    }

    return {-1, -1};
}

void MainWindow::updateBoardUI() {
    if(!currentBoard || gridButtons.isEmpty()) return;

    const auto& board = currentBoard->get_board();
    int cols = currentBoard->get_columns();

    for(int r = 0; r < currentBoard->get_rows(); ++r) {
        for(int c = 0; c < cols; ++c) {
            int index = r * cols + c;
            if(index >= gridButtons.size()) continue;

            QPushButton* btn = gridButtons[index];
            char cell = board[r][c];

            QString text;
            QString color;

            if(cell == 'X') {
                text = "✕";
                color = "#e74c3c";
            } else if(cell == 'O') {
                text = "◯";
                color = "#3498db";
            } else if(cell == 'S') {
                text = "S";
                color = "#f1c40f";
            } else if(cell == 'U') {
                text = "U";
                color = "#9b59b6";
            } else if(cell >= '1' && cell <= '9') {
                text = cell;
                if((cell - '0') % 2 == 0) {
                    color = "#2ecc71";
                } else {
                    color = "#e67e22";
                }
            } else if(cell >= 'A' && cell <= 'Z' && cell != 'X' && cell != 'O' && cell != 'S' && cell != 'U') {
                text = cell;
                color = "#1abc9c";
            } else if(cell == '#') {
                text = "█";
                color = "#7f8c8d";
            } else if(cell == ' ') {
                text = "";
            }

            if(selectedGameId == 13) {
                Memory_XO_Board* memoryBoard = dynamic_cast<Memory_XO_Board*>(currentBoard.get());
                if(memoryBoard && !memoryBoard->is_revealed(r, c)) {
                    text = "?";
                    color = "#34495e";
                }
            }

            if(selectedGameId == 8) {
                Pyramid_X_O_Board* pyramidBoard = dynamic_cast<Pyramid_X_O_Board*>(currentBoard.get());
                if(pyramidBoard) {
                    if(!pyramidBoard->is_valid_position(r, c)) {
                        btn->setVisible(false);
                        continue;
                    } else {
                        btn->setVisible(true);
                    }
                }
            }

            if(selectedGameId == 6) {
                Diamond_XO_Board* diamondBoard = dynamic_cast<Diamond_XO_Board*>(currentBoard.get());
                if(diamondBoard) {
                    if(!diamondBoard->is_valid_position(r, c)) {
                        btn->setVisible(false);
                        continue;
                    } else {
                        btn->setVisible(true);
                    }
                }
            }

            btn->setText(text);
            if(!color.isEmpty()) {
                btn->setStyleSheet(btn->styleSheet() + "color: " + color + ";");
            }
        }
    }
}

void MainWindow::hideMemoryCells() {
    if(selectedGameId == 13 && currentBoard) {
        Memory_XO_Board* memoryBoard = dynamic_cast<Memory_XO_Board*>(currentBoard.get());
        if(memoryBoard) {
            for(int r = 0; r < 3; ++r) {
                for(int c = 0; c < 3; ++c) {
                    memoryBoard->toggle_reveal(r, c);
                }
            }
            updateBoardUI();
        }
    }
    if(memoryTimer) memoryTimer->stop();
}

void MainWindow::updateGameInfo() {
    if(!players[0] || !players[1]) return;

    QString gameName = getGameName(selectedGameId);
    QString mode = ui->radioHumanVsAI->isChecked() ? "Human vs AI" : "Human vs Human";

    ui->lblGameInfo->setText(QString("🎮 <b>%1</b> | 👥 %2 | 📊 Moves: %3")
                                 .arg(gameName).arg(mode).arg(moveCount));

    Player* current = players[currentPlayerIndex];
    QString status = QString("%1's Turn (%2)")
                         .arg(QString::fromStdString(current->get_name()))
                         .arg(current->get_symbol());

    if(current->get_type() == PlayerType::COMPUTER) {
        status += " 🤖";
    }

    if(selectedGameId == 1 && currentBoard) {
        SUS_Board* susBoard = dynamic_cast<SUS_Board*>(currentBoard.get());
        if(susBoard) {
            status += QString(" | S: %1 | U: %2").arg(susBoard->get_s_score()).arg(susBoard->get_u_score());
        }
    }

    ui->lblStatus->setText("<span style='font-size: 20px; color: #00d4ff; font-weight: bold;'>" + status + "</span>");
}

void MainWindow::animateButton(QPushButton* btn) {
    QPropertyAnimation* scale = new QPropertyAnimation(btn, "geometry");
    scale->setDuration(400);
    QRect start = btn->geometry();
    scale->setKeyValueAt(0, start);
    scale->setKeyValueAt(0.3, start.adjusted(-8, -8, 8, 8));
    scale->setKeyValueAt(1, start);
    scale->start(QAbstractAnimation::DeleteWhenStopped);

    QString originalStyle = btn->styleSheet();
    QTimer::singleShot(200, [btn, originalStyle]() {
        QString flashStyle = originalStyle + "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #f1c40f, stop:1 #e67e22);";
        btn->setStyleSheet(flashStyle);
        QTimer::singleShot(300, [btn, originalStyle]() {
            btn->setStyleSheet(originalStyle);
        });
    });
}

void MainWindow::handleGameOver(const QString& message) {
    gameActive = false;

    if(currentBoard->is_win('X') || currentBoard->is_win('O')) {
        char winner = currentBoard->is_win('X') ? 'X' : 'O';
        highlightWinningCells(winner);
    }

    QMessageBox msgBox;
    msgBox.setWindowTitle("🎮 Game Over");
    msgBox.setText("<h2>" + message + "</h2>");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStyleSheet("QMessageBox { background-color: #2c3e50; color: white; }");

    QPushButton* playAgain = msgBox.addButton("🔄 Play Again", QMessageBox::AcceptRole);
    QPushButton* mainMenu = msgBox.addButton("🏠 Main Menu", QMessageBox::RejectRole);

    msgBox.exec();

    if(msgBox.clickedButton() == playAgain) {
        onStartGameClicked();
    } else {
        onBackClicked();
    }
}

void MainWindow::highlightWinningCells(char winner) {
    const auto& board = currentBoard->get_board();
    int rows = currentBoard->get_rows();
    int cols = currentBoard->get_columns();

    for(int r = 0; r < rows; ++r) {
        for(int c = 0; c < cols; ++c) {
            if(board[r][c] == winner) {
                int index = r * cols + c;
                if(index < gridButtons.size()) {
                    QPushButton* btn = gridButtons[index];
                    QString style = btn->styleSheet();
                    style += "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #2ecc71, stop:1 #27ae60);";
                    btn->setStyleSheet(style);
                }
            }
        }
    }
}

void MainWindow::onUndoClicked() {
    if(!gameActive || moveCount == 0) return;

    if(selectedGameId == 13) {
        Memory_XO_Board* memoryBoard = dynamic_cast<Memory_XO_Board*>(currentBoard.get());
        if(memoryBoard) {
            memoryBoard->undoLastMove();
            Point last = currentBoard->get_last_move();
            if(last.r != -1 && last.c != -1) {
                memoryBoard->toggle_reveal(last.r, last.c);
            }
        }
    } else {
        currentBoard->undoLastMove();
    }

    moveCount--;
    currentPlayerIndex = 1 - currentPlayerIndex;

    updateBoardUI();
    updateGameInfo();
}

void MainWindow::onHintClicked() {
    if(!gameActive || players[currentPlayerIndex]->get_type() == PlayerType::COMPUTER)
        return;

    Point hint = getAIMove();
    if(hint.r != -1 && hint.c != -1) {
        int index = hint.r * currentBoard->get_columns() + hint.c;
        if(index < gridButtons.size()) {
            QPushButton* btn = gridButtons[index];

            QPropertyAnimation* anim = new QPropertyAnimation(btn, "styleSheet");
            anim->setDuration(1500);
            QString original = btn->styleSheet();

            anim->setKeyValueAt(0, original);
            anim->setKeyValueAt(0.2, "background: #f1c40f; border: 4px solid #e67e22;");
            anim->setKeyValueAt(0.4, original);
            anim->setKeyValueAt(0.6, "background: #f1c40f; border: 4px solid #e67e22;");
            anim->setKeyValueAt(0.8, original);
            anim->setKeyValueAt(1, original);
            anim->start(QAbstractAnimation::DeleteWhenStopped);
        }
    }
}

void MainWindow::onBackClicked() {
    resetGame();
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::resetGame() {
    gameActive = false;
    moveCount = 0;

    currentBoard.reset();

    delete players[0];
    delete players[1];
    players[0] = players[1] = nullptr;

    qDeleteAll(gridButtons);
    gridButtons.clear();

    if(memoryTimer) {
        memoryTimer->stop();
        delete memoryTimer;
        memoryTimer = nullptr;
    }
}

void MainWindow::switchPlayer() {
    currentPlayerIndex = 1 - currentPlayerIndex;
    updateGameInfo();
}
