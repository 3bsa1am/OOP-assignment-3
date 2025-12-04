#include <iostream>
#include <cctype>
#include <vector>
#include <cstdlib>
#include "Amir.h"

using namespace std;

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
        return true;
    }
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

XO_4_x_4_UI::XO_4_x_4_UI() : UI<char>("Welcome to 4x4 XO Game: ", 4) {}

Player<char>* XO_4_x_4_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

Move<char>* XO_4_x_4_UI::computerMove(Player<char>* aiPlayer) {
    XO_4_x_4_Board* board = dynamic_cast<XO_4_x_4_Board*>(aiPlayer->get_board_ptr());
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
                delete testMove;
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
        return true;
    }
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
    if (sym == 'S')return cnt1 > cnt2;
    else if (sym == 'U') return cnt2 > cnt1;
    return false;

}

bool sus_board::is_draw(Player<char> *player) {
    return (game_is_over(player) && cnt1 == cnt2);
}


sus_ui::sus_ui() : UI<char>("Welcome to SUS Game: ", 3) {}

Player<char>* sus_ui::create_player(string& name, char symbol, PlayerType type) {
    // static bool first = true;
    // if (first) {
    //     symbol = 'S';
    //     first = false;
    // } else {
    //     symbol = 'U';
    // }

    cout << "Creating "
         << (type == PlayerType::HUMAN ? "human" : "computer")
         << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);

}

Player<char> **sus_ui::setup_players() {
    Player<char>** players = new Player<char>*[2];
    vector<string> type_options = { "Human", "Computer" };

    string nameX = get_player_name("Player S");
    PlayerType typeX = get_player_type_choice("Player S", type_options);
    players[0] = create_player(nameX, static_cast<char>('S'), typeX);

    string nameO = get_player_name("Player U");
    PlayerType typeO = get_player_type_choice("Player U", type_options);
    players[1] = create_player(nameO, static_cast<char>('U'), typeO);

    return players;
}





Move<char>* sus_ui::get_move(Player<char>* player) {
    int x, y;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nPlease enter your move x and y (0 to 2): ";
        cin >> x >> y;
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        x = rand() % player->get_board_ptr()->get_rows();
        y = rand() % player->get_board_ptr()->get_columns();
    }
    return new Move<char>(x, y, player->get_symbol());
}
/////////////////////////// ultimate
ultimate_XO_board::ultimate_XO_board():Board(3,3) {
    for (int i = 0;i<3;i++) {
        for (int j = 0;j<3;j++) {
            board[i][j] = ' ';
            globalBoard[i][j] = ' ';
        }
    }
}

ultimate_XO_board::~ultimate_XO_board() {
}

bool ultimate_XO_board::isValidMove(int r, int c) const {
    return r >= 0 && r < rows && c >= 0 && c < columns && board[r][c] == ' ';
}

void ultimate_XO_board::reset_small_board() {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            board[i][j] = ' ';
    n_moves = 0;
}

bool ultimate_XO_board::small_board_is_draw() {
    return n_moves >= 9;
}

void ultimate_XO_board::update_board2(char symbol) {
    int r = get_big_cell_Row();
    int c = get_big_cell_Col();

    if (globalBoard[r][c] == ' ') {
        globalBoard[r][c] = symbol;
        N_moves++;
        reset_small_board();
    }
}

bool ultimate_XO_board::update_board(Move<char>* move) {
    int r = move->get_x();
    int c = move->get_y();
    char symbol = move->get_symbol();

    if (!isValidMove(r, c)) {
        cout << "Invalid move. Try again.\n";
        delete move;
        return false;
    }

    board[r][c] = toupper(symbol);
    n_moves++;


    if (is_win_small(move)) {
        update_board2(symbol);
        display_global_board();
        currentBigRow = -1;
        currentBigCol = -1;
    }

    else if (small_board_is_draw()) {
        update_board2('D');
        display_global_board();
        currentBigRow = -1;
        currentBigCol = -1;
    }

    delete move;
    return true;
}


bool ultimate_XO_board::is_win_small(Move<char>* move) {
    char s = move->get_symbol();
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == s && board[i][1] == s && board[i][2] == s) {

            return true;
        }
        if (board[0][i] == s && board[1][i] == s && board[2][i] == s)
        {

            return true;
        }
    }
    if (board[0][0] == s && board[1][1] == s && board[2][2] == s)
    {

        return true;
    }
    if (board[0][2] == s && board[1][1] == s && board[2][0] == s)
    {

        return true;
    }
    return false;
}

char ultimate_XO_board::get_small_cell(int r, int c)  {
    if (r < 0 || r >= rows || c < 0 || c >= columns) return '\0';
    return board[r][c];
}

char ultimate_XO_board::get_global_cell(int r, int c)  {
    if (r < 0 || r >= 3 || c < 0 || c >= 3) return '\0';
    return globalBoard[r][c];
}

bool ultimate_XO_board::is_small_board_available(int br, int bc)  {
    if (br < 0 || br >= 3 || bc < 0 || bc >= 3) return false;
    return globalBoard[br][bc] == ' ';
}


bool ultimate_XO_board::is_win(Player<char> *player) {
    char s = player->get_symbol();
    for (int i = 0; i < 3; i++) {
        if (globalBoard[i][0] == s && globalBoard[i][1] == s && globalBoard[i][2] == s) return true;
        if (globalBoard[0][i] == s && globalBoard[1][i] == s && globalBoard[2][i] == s) return true;
    }
    if (globalBoard[0][0] == s && globalBoard[1][1] == s && globalBoard[2][2] == s) return true;
    if (globalBoard[0][2] == s && globalBoard[1][1] == s && globalBoard[2][0] == s) return true;
    return false;
}

bool ultimate_XO_board::is_draw(Player<char> *player) {
    return (N_moves >= 9 && !is_win(player));
}


bool ultimate_XO_board::game_is_over(Player<char> *player) {
    return is_win(player) || is_draw(player);
}


ultimate_XO_UI::ultimate_XO_UI():UI("Welcome to ultimate XO",3) {}

Player<char> *ultimate_XO_UI::create_player(string &name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

Move<char>* ultimate_XO_UI::get_move(Player<char>* player) {
    ultimate_XO_board* ub = dynamic_cast<ultimate_XO_board*>(player->get_board_ptr());
    if (!ub) {
        // fallback for non-ultimate games
        int x, y;
        cout << "\nPlease enter your move x and y: ";
        cin >> x >> y;
        return new Move<char>(x, y, player->get_symbol());
    }

    int br = ub->get_big_cell_Row();
    int bc = ub->get_big_cell_Col();
    int x = -1, y = -1;

    // If no current big board selected, ask user/computer to pick one.
    if (br == -1 || bc == -1 || !ub->is_small_board_available(br, bc)) {
        // Need to choose a new big board
        if (player->get_type() == PlayerType::HUMAN) {
            while (true) {
                cout << "\nChoose which small board to play (bigRow bigCol) (0-2 0-2): ";
                cin >> br >> bc;
                if (cin.fail()) { cin.clear(); cin.ignore(1000,'\n'); cout << "Invalid input\n"; continue; }
                if (br < 0 || br > 2 || bc < 0 || bc > 2) { cout << "Indices must be 0..2\n"; continue; }
                if (!ub->is_small_board_available(br, bc)) { cout << "That small board is finished. Pick another.\n"; continue; }
                break;
            }
        } else { // COMPUTER: pick random available big board
            do {
                br = rand() % 3;
                bc = rand() % 3;
            } while (!ub->is_small_board_available(br, bc));
            cout << "\nComputer picks small board (" << br << "," << bc << ")\n";
        }
        ub->set_big_cell_Row(br);
        ub->set_big_cell_Col(bc);
    } else {
        // current big board is valid and still available — play there directly
        // no prompt for big board
        // optionally print which big board is active:
        if (player->get_type() == PlayerType::COMPUTER) {
            cout << "\nComputer continues in small board (" << br << "," << bc << ")\n";
        } else {
            cout << "\nYou will play in current small board (" << br << "," << bc << ")\n";
        }
    }

    // Now choose a cell inside the selected small board
    if (player->get_type() == PlayerType::HUMAN) {
        while (true) {
            cout << "Enter your move inside board (" << br << "," << bc << ") -> (row col) 0-2: ";
            cin >> x >> y;
            if (cin.fail()) { cin.clear(); cin.ignore(1000,'\n'); cout << "Invalid input\n"; continue; }
            if (x < 0 || x > 2 || y < 0 || y > 2) { cout << "Indices must be 0..2\n"; continue; }
            if (ub->get_small_cell(x, y) != ' ') { cout << "Cell occupied. Choose another.\n"; continue; }
            break;
        }
    } else { // COMPUTER: pick random free cell in current small board
        do {
            x = rand() % 3;
            y = rand() % 3;
        } while (ub->get_small_cell(x, y) != ' ');
        cout << "Computer plays inside (" << x << "," << y << ")\n";
    }

    return new Move<char>(x, y, player->get_symbol());
}

void ultimate_XO_board::display_global_board() const {
    cout << "\nGlobal Board Status:\n";

    int rows = 3;
    int cols = 3;

    cout << "\n    ";
    for (int j = 0; j < cols; ++j)
        cout << setw(3 + 1) << j;
    cout << "\n   " << string((3 + 2) * cols, '-') << "\n";

    for (int i = 0; i < rows; ++i) {
        cout << setw(2) << i << " |";
        for (int j = 0; j < cols; ++j)
            cout << setw(3) << globalBoard[i][j] << " |";
        cout << "\n   " << string((3 + 2) * cols, '-') << "\n";
    }
    cout << endl;

}

