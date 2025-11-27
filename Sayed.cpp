#include "Sayed.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cctype>

#include <queue>

using namespace std;


//  5x5 Tic Tac Toe 

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

bool XO_5x5_Board::check_three_in_row(char symbol) const {
    char sym = toupper(symbol);
    
    const int directions[4][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            for (int d = 0; d < 4; d++) {
                int dx = directions[d][0];
                int dy = directions[d][1];
                
                bool valid = true;
                for (int k = 0; k < 3; k++) {
                    int ni = i + k * dx;
                    int nj = j + k * dy;
                    if (ni < 0 || ni >= rows || nj < 0 || nj >= columns || board[ni][nj] != sym) {
                        valid = false;
                        break;
                    }
                }
                if (valid) return true;
            }
        }
    }
    return false;
}

int XO_5x5_Board::count_three_in_row(char symbol) const {
    char sym = toupper(symbol);
    int count = 0;
    
    const int directions[4][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            for (int d = 0; d < 4; d++) {
                int dx = directions[d][0];
                int dy = directions[d][1];
                
                bool valid = true;
                for (int k = 0; k < 3; k++) {
                    int ni = i + k * dx;
                    int nj = j + k * dy;
                    if (ni < 0 || ni >= rows || nj < 0 || nj >= columns || board[ni][nj] != sym) {
                        valid = false;
                        break;
                    }
                }
                if (valid) count++;
            }
        }
    }
    return count;
}

bool XO_5x5_Board::is_win(Player<char>* player) {
    if (n_moves < 24) {
        return false;
    }
    
    char current_symbol = toupper(player->get_symbol());
    char opponent_symbol = (current_symbol == 'X') ? 'O' : 'X';
    
    int current_score = count_three_in_row(current_symbol);
    int opponent_score = count_three_in_row(opponent_symbol);
    
    return current_score > opponent_score;
}

bool XO_5x5_Board::is_draw(Player<char>* player) {
    if (n_moves < 24) {
        return false;
    }
    
    char current_symbol = toupper(player->get_symbol());
    char opponent_symbol = (current_symbol == 'X') ? 'O' : 'X';
    
    int current_score = count_three_in_row(current_symbol);
    int opponent_score = count_three_in_row(opponent_symbol);
    
    return current_score == opponent_score;
}

bool XO_5x5_Board::game_is_over(Player<char>* player) {
    return n_moves >= 24;
}

XO_5x5_UI::XO_5x5_UI() : UI<char>("Welcome to 5x5 Tic Tac Toe (Most 3-in-a-row wins!)", 3) {}

Player<char>* XO_5x5_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";
    return new Player<char>(name, symbol, type);
}

Move<char>* XO_5x5_UI::computerMove(Player<char>* aiPlayer) {
    XO_5x5_Board* board = dynamic_cast<XO_5x5_Board*>(aiPlayer->get_board_ptr());
    if (!board) return nullptr;

    char aiSym = aiPlayer->get_symbol();
    char humanSym = (aiSym == 'X') ? 'O' : 'X';
    int rows = board->get_rows();
    int cols = board->get_columns();

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            Move<char>* testMove = new Move<char>(r, c, aiSym);
            if (board->update_board(testMove)) {
                if (board->count_three_in_row(aiSym) > 0) {
                    board->undoLastMove();
                    return new Move<char>(r, c, aiSym);
                }
                board->undoLastMove();
            } else {
                delete testMove;
            }
        }
    }

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            Move<char>* testMove = new Move<char>(r, c, humanSym);
            if (board->update_board(testMove)) {
                if (board->count_three_in_row(humanSym) > 0) {
                    board->undoLastMove();
                    delete testMove;
                    return new Move<char>(r, c, aiSym);
                }
                board->undoLastMove();
            } else {
                delete testMove;
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
    delete randomMove;
    return new Move<char>(r, c, aiSym);
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


// Word Tic-tac-toe


Word_TTT_Board::Word_TTT_Board() : Board(3, 3) {
    
        load_dictionary("dic.txt");  //   load dic.txt

    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            board[i][j] = blank_symbol;
        }
    }
}

bool Word_TTT_Board::load_dictionary(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open dictionary file '" << filename << "'\n";
        return false;
    }
    
    string word;
    while (file >> word) {
        for (char &c : word) {
            c = toupper(c);
        }
        if (word.length() == 3) {
            dictionary.insert(word);
        }
    }
    
    file.close();
    cout << "Loaded " << dictionary.size() << " words from dictionary.\n";
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
    for (int i = 0; i < 3; i++) {
        string word = get_word(i, 0, 0, 1); 
        if (word.length() == 3 && dictionary.find(word) != dictionary.end()) {
            cout << "Word found: " << word << " (row " << i << ")\n";
            return true;
        }
    }
    
    for (int j = 0; j < 3; j++) {
        string word = get_word(0, j, 1, 0);
        if (word.length() == 3 && dictionary.find(word) != dictionary.end()) {
            cout << "Word found: " << word << " (column " << j << ")\n";
            return true;
        }
    }
    
    string diag1 = get_word(0, 0, 1, 1);
    if (diag1.length() == 3 && dictionary.find(diag1) != dictionary.end()) {
        cout << "Word found: " << diag1 << " (main diagonal)\n";
        return true;
    }
    
    string diag2 = get_word(0, 2, 1, -1);
    if (diag2.length() == 3 && dictionary.find(diag2) != dictionary.end()) {
        cout << "Word found: " << diag2 << " (anti-diagonal)\n";
        return true;
    }
    
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
    
    cout << "Invalid move. Cell may be occupied or out of bounds.\n";
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




//  Word TTT AI




Word_TTT_UI::Word_TTT_UI() : UI<char>("Welcome to Word Tic-Tac-Toe! Form 3-letter words to win.", 3) {}

Player<char>* Word_TTT_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (uses letters)\n";
    return new Player<char>(name, symbol, type);
}


Move<char>* Word_TTT_UI::computerMove(Player<char>* aiPlayer) {
    Word_TTT_Board* board = dynamic_cast<Word_TTT_Board*>(aiPlayer->get_board_ptr());
    if (!board) return nullptr;

    vector<vector<char>> current_board = board->get_board_matrix();
    vector<Point> available_moves;
    
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            if (current_board[r][c] == ' ') {
                available_moves.push_back({r, c});
            }
        }
    }
    
    if (available_moves.empty()) {
        return new Move<char>(0, 0, 'A'); 
    }
    
    
    for (int r = 0; r < 3; r++) {
        string row = "";
        int empty_count = 0;
        int empty_pos = -1;
        
        for (int c = 0; c < 3; c++) {
            if (current_board[r][c] == ' ') {
                empty_count++;
                empty_pos = c;
            } else {
                row += current_board[r][c];
            }
        }
        
        if (empty_count == 1 && row.length() == 2) {
            for (char letter = 'A'; letter <= 'Z'; letter++) {
                string test_word = row;
                test_word.insert(empty_pos, 1, letter);
                
                Move<char>* testMove = new Move<char>(r, empty_pos, letter);
                if (board->update_board(testMove)) {
                    board->undoLastMove();
                    return new Move<char>(r, empty_pos, letter);
                }
                delete testMove;
            }
        }
    }
    
    for (int c = 0; c < 3; c++) {
        string col = "";
        int empty_count = 0;
        int empty_pos = -1;
        
        for (int r = 0; r < 3; r++) {
            if (current_board[r][c] == ' ') {
                empty_count++;
                empty_pos = r;
            } else {
                col += current_board[r][c];
            }
        }
        
        if (empty_count == 1 && col.length() == 2) {
            for (char letter = 'A'; letter <= 'Z'; letter++) {
                Move<char>* testMove = new Move<char>(empty_pos, c, letter);
                if (board->update_board(testMove)) {
                    board->undoLastMove();
                    return new Move<char>(empty_pos, c, letter);
                }
                delete testMove;
            }
        }
    }

    int move_index = rand() % available_moves.size();
    Point chosen_move = available_moves[move_index];
    
    char random_letter = 'A' + (rand() % 26);
    
    return new Move<char>(chosen_move.r, chosen_move.c, random_letter);

}


///////////////////////////////////////////////////


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





//  Infinity Tic-Tac-Toe




Infinity_TTT_Board::Infinity_TTT_Board() : Board(3, 3) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            board[i][j] = blank_symbol;
        }
    }
}

bool Infinity_TTT_Board::check_win(char symbol) const {
    char sym = toupper(symbol);
    
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == sym && board[i][1] == sym && board[i][2] == sym)
            return true;
    }
    
    for (int j = 0; j < 3; j++) {
        if (board[0][j] == sym && board[1][j] == sym && board[2][j] == sym)
            return true;
    }
    
    if (board[0][0] == sym && board[1][1] == sym && board[2][2] == sym)
        return true;
    if (board[0][2] == sym && board[1][1] == sym && board[2][0] == sym)
        return true;
    
    return false;
}

void Infinity_TTT_Board::remove_oldest_move() {
    if (!move_history.empty()) {
        Point oldest = move_history.front();
        move_history.pop();
        board[oldest.r][oldest.c] = blank_symbol;
        
        cout << "Removed oldest move at (" << oldest.r << ", " << oldest.c << ")\n";
    }
}

bool Infinity_TTT_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char sym = move->get_symbol();

    if (x >= 0 && x < rows && y >= 0 && y < columns && board[x][y] == blank_symbol) {
        n_moves++;
        board[x][y] = toupper(sym);
        
        move_history.push({x, y});
        
        // [4 , 7 , 10 ....] delet element
        if (n_moves >= 4 && (n_moves - 1) % 3 == 0) {
            remove_oldest_move();
        }
        
        delete move;
        return true;
    }
    
    cout << "Invalid move. Cell may be occupied or out of bounds.\n";
    delete move;
    return false;
}

bool Infinity_TTT_Board::is_win(Player<char>* player) {
    return check_win(player->get_symbol());
}

bool Infinity_TTT_Board::is_draw(Player<char>* player) {
    return false;
}

bool Infinity_TTT_Board::game_is_over(Player<char>* player) {
    return is_win(player);
}


Infinity_TTT_UI::Infinity_TTT_UI() : UI<char>("Welcome to Infinity Tic-Tac-Toe! Oldest moves disappear every 3 moves.", 3) {}

Player<char>* Infinity_TTT_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
         << " player: " << name << " (" << symbol << ")\n";
    return new Player<char>(name, symbol, type);
}

Move<char>* Infinity_TTT_UI::computerMove(Player<char>* aiPlayer) {
    Infinity_TTT_Board* board = dynamic_cast<Infinity_TTT_Board*>(aiPlayer->get_board_ptr());
    if (!board) return nullptr;

    char aiSym = aiPlayer->get_symbol();
    char humanSym = (aiSym == 'X') ? 'O' : 'X';
    vector<vector<char>> current_board = board->get_board_matrix();
    
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            if (current_board[r][c] == ' ') {
                vector<vector<char>> temp_board = current_board;
                temp_board[r][c] = aiSym;
                
                bool win = false;
                
        
                for (int i = 0; i < 3; i++) {
                    if (temp_board[i][0] == aiSym && temp_board[i][1] == aiSym && temp_board[i][2] == aiSym) 
                        win = true;
                }
                
                            for (int j = 0; j < 3; j++) {
                    if (temp_board[0][j] == aiSym && temp_board[1][j] == aiSym && temp_board[2][j] == aiSym) 
                        win = true;
                }
                
                            if (temp_board[0][0] == aiSym && temp_board[1][1] == aiSym && temp_board[2][2] == aiSym) 
                    win = true;
                if (temp_board[0][2] == aiSym && temp_board[1][1] == aiSym && temp_board[2][0] == aiSym) 
                    win = true;
                
                if (win) {
                    return new Move<char>(r, c, aiSym);
                }
            }
        }
    }
    
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            if (current_board[r][c] == ' ') {
                vector<vector<char>> temp_board = current_board;
                temp_board[r][c] = humanSym;
                
                bool opponent_win = false;
                
        
                for (int i = 0; i < 3; i++) {
                    if (temp_board[i][0] == humanSym && temp_board[i][1] == humanSym && temp_board[i][2] == humanSym) 
                        opponent_win = true;
                }
                
                            for (int j = 0; j < 3; j++) {
                    if (temp_board[0][j] == humanSym && temp_board[1][j] == humanSym && temp_board[2][j] == humanSym) 
                        opponent_win = true;
                }
                
                            if (temp_board[0][0] == humanSym && temp_board[1][1] == humanSym && temp_board[2][2] == humanSym) 
                    opponent_win = true;
                if (temp_board[0][2] == humanSym && temp_board[1][1] == humanSym && temp_board[2][0] == humanSym) 
                    opponent_win = true;
                
                if (opponent_win) {
                    return new Move<char>(r, c, aiSym);
                }
            }
        }
    }
    
    vector<Point> strategic_moves = {{1, 1}, {0, 0}, {0, 2}, {2, 0}, {2, 2}, {0, 1}, {1, 0}, {1, 2}, {2, 1}};
    
    for (const Point& move : strategic_moves) {
        if (current_board[move.r][move.c] == ' ') {
            return new Move<char>(move.r, move.c, aiSym);
        }
    }
    
    vector<Point> available_moves;
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            if (current_board[r][c] == ' ') {
                available_moves.push_back({r, c});
            }
        }
    }
    
    if (available_moves.empty()) {
        return new Move<char>(0, 0, aiSym);
    }
    
    int move_index = rand() % available_moves.size();
    Point chosen_move = available_moves[move_index];
    return new Move<char>(chosen_move.r, chosen_move.c, aiSym);
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