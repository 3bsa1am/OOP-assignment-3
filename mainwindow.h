#ifndef COMBINED_GAME_H
#define COMBINED_GAME_H


#include <QMainWindow>
#include <QPushButton>
#include <QGridLayout>
#include <QTimer>
#include <vector>
#include <memory>
#include <string>
#include <map>
#include <set>
#include <deque>
#include <algorithm>
#include <random>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// ==========================================
// Game Logic Types and Classes
// ==========================================

enum class PlayerType { HUMAN, COMPUTER };

struct Point {
    int r, c;
    Point(int r = -1, int c = -1) : r(r), c(c) {}
};

class Player {
    std::string name;
    char symbol;
    PlayerType type;
public:
    Player(const std::string& n, char s, PlayerType t) : name(n), symbol(s), type(t) {}
    std::string get_name() const { return name; }
    char get_symbol() const { return symbol; }
    PlayerType get_type() const { return type; }
};

class GameBoard {
protected:
    int rows, columns;
    std::vector<std::vector<char>> board;
    int n_moves = 0;
    std::vector<Point> history;
    char blank_symbol = ' ';

public:
    GameBoard(int r, int c, char blank = ' ') : rows(r), columns(c), blank_symbol(blank) {
        board = std::vector<std::vector<char>>(r, std::vector<char>(c, blank));
    }
    virtual ~GameBoard() = default;

    virtual bool update_board(int r, int c, char symbol) = 0;
    virtual bool is_win(char symbol) const = 0;
    virtual bool is_draw() const = 0;

    virtual void undoLastMove() {
        if(!history.empty()) {
            Point p = history.back();
            board[p.r][p.c] = blank_symbol;
            history.pop_back();
            n_moves--;
        }
    }

    virtual std::vector<Point> get_valid_moves() const {
        std::vector<Point> moves;
        for(int i = 0; i < rows; ++i)
            for(int j = 0; j < columns; ++j)
                if(board[i][j] == blank_symbol)
                    moves.push_back({i, j});
        return moves;
    }

    const std::vector<std::vector<char>>& get_board() const { return board; }
    int get_rows() const { return rows; }
    int get_columns() const { return columns; }
    int get_n_moves() const { return n_moves; }
    char get_cell(int r, int c) const {
        return (r >= 0 && r < rows && c >= 0 && c < columns) ? board[r][c] : '\0';
    }
    void set_cell(int r, int c, char val) {
        if(r >= 0 && r < rows && c >= 0 && c < columns)
            board[r][c] = val;
    }

    // Public access methods for history
    const std::vector<Point>& get_history() const { return history; }
    bool is_history_empty() const { return history.empty(); }
    Point get_last_move() const {
        return history.empty() ? Point(-1, -1) : history.back();
    }
};

// ==========================================
// Game Board Declarations
// ==========================================

class TTT_Board : public GameBoard {
    int winCondition;
public:
    TTT_Board(int r = 3, int c = 3, int win = 3);
    bool update_board(int r, int c, char sym) override;
    bool is_win(char sym) const override;
    bool is_draw() const override;
};

class Misere_TTT_Board : public TTT_Board {
public:
    Misere_TTT_Board();
    bool is_win(char sym) const override;
};

class C4_Board : public GameBoard {
public:
    C4_Board();
    bool update_board(int r, int c, char sym) override;
    bool is_win(char sym) const override;
    bool is_draw() const override;
    int get_drop_row(int col) const;
};

class XO_4_x_4_Board : public GameBoard {
public:
    XO_4_x_4_Board();
    bool update_board(int r, int c, char sym) override;
    bool is_win(char sym) const override;
    bool is_draw() const override;
};

class SUS_Board : public GameBoard {
    int s_score = 0, u_score = 0;
public:
    SUS_Board();
    bool update_board(int r, int c, char sym) override;
    bool is_win(char sym) const override;
    bool is_draw() const override;
    int get_s_score() const { return s_score; }
    int get_u_score() const { return u_score; }
};

class XO_5x5_Board : public GameBoard {
    int winCondition;
public:
    XO_5x5_Board(int win = 4);
    bool update_board(int r, int c, char sym) override;
    bool is_win(char sym) const override;
    bool is_draw() const override;
};

class Numerical_TTT_Board : public GameBoard {
    std::set<int> odd_used, even_used;
    bool is_odd_turn = true;
public:
    Numerical_TTT_Board();
    bool update_board(int r, int c, char sym) override;
    bool is_win(char sym) const override;
    bool is_draw() const override;
    bool is_valid_number(char num, bool is_odd) const;
};

class Obstacles_TTT_Board : public GameBoard {
    std::vector<Point> obstacles;
public:
    Obstacles_TTT_Board();
    bool update_board(int r, int c, char sym) override;
    bool is_win(char sym) const override;
    bool is_draw() const override;
    bool is_obstacle(int r, int c) const;
};

class Word_TTT_Board : public GameBoard {
    std::set<std::string> dictionary;
public:
    Word_TTT_Board();
    bool update_board(int r, int c, char sym) override;
    bool is_win(char sym) const override;
    bool is_draw() const override;
    bool is_valid_letter(char c) const;
};

class Ultimate_XO_Board : public GameBoard {
    char mini_wins[3][3] = {{0}};
public:
    Ultimate_XO_Board();
    bool update_board(int r, int c, char sym) override;
    bool is_win(char sym) const override;
    bool is_draw() const override;
};

class Infinity_TTT_Board : public GameBoard {
    std::deque<Point> move_queue;
    struct MoveInfo {
        Point placed;
        bool removed;
        Point removed_pos;
        char removed_sym;
    };
    std::vector<MoveInfo> history_info;
public:
    Infinity_TTT_Board();
    bool update_board(int r, int c, char sym) override;
    bool is_win(char sym) const override;
    bool is_draw() const override;
    void undoLastMove() override;
};

class Pyramid_X_O_Board : public GameBoard {
public:
    Pyramid_X_O_Board();
    bool update_board(int r, int c, char sym) override;
    bool is_win(char sym) const override;
    bool is_draw() const override;
    bool is_valid_position(int r, int c) const;
};

class Diamond_XO_Board : public GameBoard {
public:
    Diamond_XO_Board();
    bool update_board(int r, int c, char sym) override;
    bool is_win(char sym) const override;
    bool is_draw() const override;
    bool is_valid_position(int r, int c) const;
};

class Memory_XO_Board : public GameBoard {
    std::vector<std::vector<bool>> revealed;
public:
    Memory_XO_Board();
    bool update_board(int r, int c, char sym) override;
    bool is_win(char sym) const override;
    bool is_draw() const override;
    bool is_revealed(int r, int c) const;
    void toggle_reveal(int r, int c);
};

// ==========================================
// AI System
// ==========================================

class GameAI {
    static std::mt19937 rng;
public:
    static void init() { rng.seed(std::random_device{}()); }

    template<typename BoardType>
    static Point get_best_move(const BoardType* board, char ai_sym, char human_sym);

    template<typename BoardType>
    static Point get_random_move(const BoardType* board);

    static Point get_connect4_move(const C4_Board* board, char ai_sym, char human_sym);
    static Point get_sus_move(const SUS_Board* board, char ai_sym, char human_sym);
    static Point get_word_move(const Word_TTT_Board* board, char ai_sym, char human_sym);
    static Point get_numerical_move(const Numerical_TTT_Board* board, char ai_sym, char human_sym, bool is_odd);
};

// ==========================================
// Main Window Class
// ==========================================

class MainWindow : public QMainWindow {
    Q_OBJECT



public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onStartGameClicked();
    void onBackClicked();
    void onUndoClicked();
    void onHintClicked();
    void onGridCellClicked();
    void processComputerTurn();
    void hideMemoryCells();

private:
    Ui::MainWindow *ui;

    // Game State
    int selectedGameId = 14;
    std::unique_ptr<GameBoard> currentBoard;
    Player* players[2] = {nullptr, nullptr};
    int currentPlayerIndex = 0;
    bool gameActive = false;
    int moveCount = 0;
    QTimer* memoryTimer = nullptr;

    QVector<QPushButton*> gridButtons;

    void setupGameButtons();
    void createBoardGrid(int rows, int cols);
    void updateBoardUI();
    void updateGameInfo();
    void handleGameOver(const QString& message);
    void performMove(int r, int c, char symbol);
    void switchPlayer();
    void resetGame();
    void animateButton(QPushButton* btn);
    void highlightWinningCells(char winner);

    void onGameButtonClicked(int gameId);
    Point getAIMove();
    std::unique_ptr<GameBoard> createGameBoard(int gameId);
    QString getGameName(int gameId) const;
    QString getGameDescription(int gameId) const;
    void applyModernStyle();

    // Input dialogs
    char getWordInput();
    char getNumberInput(bool is_odd);
    char getSUSInput();
};

#endif // COMBINED_GAME_H
