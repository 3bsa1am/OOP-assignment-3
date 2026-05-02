# Board Game Console — OOP Assignment 3

A text-based, menu-driven **C++17 game console** that offers **14 board-game variants** built on a shared OOP framework. The project is a university OOP assignment demonstrating inheritance, templates, polymorphism, and design patterns via classic two-player games (Tic-Tac-Toe family, Connect 4, and creative variants). Every game supports both **Human vs Human** and **Human vs Computer** (AI) modes.

---

## Table of Contents

1. [Overview](#overview)
2. [Features & Supported Games](#features--supported-games)
3. [Project Structure](#project-structure)
4. [Build Instructions](#build-instructions)
5. [Usage](#usage)
6. [Design & Architecture](#design--architecture)
7. [Extending / Adding a Game](#extending--adding-a-game)
8. [Known Limitations](#known-limitations)
9. [Authors](#authors)

---

## Overview

The project implements a "game console" that the user launches from the terminal. A numbered menu lets them pick any of 14 games. For each game the player names and types (human/computer) are entered, then the game loop runs until a win, loss, or draw is detected. After a game ends the user is returned to the main menu.

Key OOP concepts used:

- **Abstract base classes / pure virtual methods** (`Board<T>`, `UI<T>`)
- **Template polymorphism** — all four core classes are `template <typename T>`
- **Inheritance chains** — e.g. `Misere_TTT_Board -> TTT_Board -> Board<char>`
- **Strategy pattern** — `GameAI` helper class with templated AI strategies
- **Command / Move pattern** — `Move<T>` objects encapsulate each play

---

## Features & Supported Games

| Menu # | Game | Board | Win Condition |
|--------|------|-------|---------------|
| 1 | **SUS** | 3x3 | Form the most "S-U-S" sequences; scored by symbol placement |
| 2 | **Four in a Row (Connect 4)** | 6x7 | First to get 4 in a row (gravity applies) |
| 3 | **5x5 Tic-Tac-Toe** | 5x5 | Most 3-in-a-row sequences when the board is full (24 moves) |
| 4 | **Word Tic-Tac-Toe** | 3x3 | Form a valid 3-letter English word in any row / column / diagonal |
| 5 | **Misere X-O** | 3x3 | Force the opponent to complete a 3-in-a-row (completing one = loss) |
| 6 | **Diamond Tic-Tac-Toe** | 7x7 diamond | Simultaneously hold a line of 3 **and** a line of 4 in **different** directions |
| 7 | **4x4 X-O** | 4x4 | Get 3 in a row (horizontal, vertical, or diagonal) |
| 8 | **Pyramid X-O** | 1+3+5 cells | Get 3 in a row along valid pyramid lines |
| 9 | **Numerical Tic-Tac-Toe** | 3x3 | Place odd (1,3,5,7,9) or even (2,4,6,8) numbers; any line summing to 15 wins |
| 10 | **Obstacles X-O** | 6x6 | Get 4 in a row; board starts with random `#` obstacle cells |
| 11 | **Infinity Tic-Tac-Toe** | 3x3 | Standard TTT but the oldest marks periodically disappear |
| 12 | **Ultimate Tic-Tac-Toe** | 9x9 (3x3 of 3x3 mini-boards) | Win three mini-boards in a row on the meta-board |
| 13 | **Memory Tic-Tac-Toe** | 3x3 | Standard TTT but placed marks are hidden during play |
| 14 | **X-O (classic 3x3)** | 3x3 | Classic Tic-Tac-Toe with undo support (`-1`) |
| 15 | **Exit** | — | — |

---

## Project Structure

```
.
├── BoardGame_Classes.h    # Core framework: Board, Move, Player, UI, GameManager templates
├── Main.cpp               # Entry point; main menu loop; instantiates each game
├── Mohammed.h / .cpp      # TTT, Misere TTT, Connect 4, Numerical TTT, Obstacles TTT + GameAI
├── Amir.h    / .cpp       # 4x4 XO, SUS, Ultimate Tic-Tac-Toe
├── Sayed.h   / .cpp       # 5x5 TTT, Word TTT, Infinity TTT
├── abdelmalik.h / .cpp    # Pyramid XO, Diamond XO, Memory XO
├── dic.txt                # English word list used by Word Tic-Tac-Toe
├── CMakeLists.txt         # CMake build (C++17, single executable "Game")
└── Doxyfile               # Doxygen config for generated HTML/LaTeX API docs
```

> **Note:** `Main.cpp` directly `#include`s all four `.cpp` files. This "unity build" approach is intentional — it ensures the templated `GameAI` methods are instantiated in the correct translation unit.

---

## Build Instructions

### Prerequisites

| Tool | Minimum version |
|------|----------------|
| C++ compiler | GCC 7 / Clang 5 / MSVC 2017 (C++17 required) |
| CMake | 3.10+ |

No external libraries are required; only the C++ standard library is used.

### With CMake (recommended)

```bash
# 1. Clone the repository
git clone https://github.com/3bsa1am/OOP-assignment-3.git
cd OOP-assignment-3

# 2. Configure and build
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# 3. Run
./build/Game
```

CMake automatically copies `dic.txt` next to the executable.

### Manual compilation (g++)

```bash
g++ -std=c++17 -O2 -o Game Main.cpp
# dic.txt must be in the same directory as the executable
./Game
```

---

## Usage

```
 Games menu
1. SUS 3x3
2. Four in a row (6x7)
3. 5x5 Tic Tac Toe
4. Word Tic-Tac-Toe
5. Misere X-O
6. Diamond Tic-Tac-Toe
7. 4x4 X-O
8. Pyramid X-O
9. Num X-O (3x3)
10. Obstacles X-O (6x6)
11. Infinity Tic-Tac-Toe
12. Ultimate Tic-Tac-Toe
13. Memory Tic-Tac-Toe
14. X-O (3x3)
15. Exit
Choose game:
```

1. Enter the menu number and press **Enter**.
2. Enter player names and choose the player type (`1` = Human, `2` = Computer) when prompted.
3. Follow in-game prompts. Coordinates are **0-based** (row then column) unless stated otherwise.

### Example — Classic X-O (choice 14)

```
Enter Player X name: Alice
Choose Player X type:
1. Human
2. Computer
1
Enter Player O name: Bob
...
Alice's Turn (X).
Enter row and col (0-2) or -1 to Undo: 1 1

     0   1   2
   -----------
 0 |   |   |   |
   -----------
 1 |   | X |   |
   -----------
 2 |   |   |   |
   -----------
```

Enter `-1` on the row prompt to **undo** the last move (classic X-O only).

### Example — Connect 4 (choice 2)

```
Alice's Turn (X).
Enter column (0-6): 3
```

The token falls to the lowest empty row automatically.

### Example — Word Tic-Tac-Toe (choice 4)

```
Alice's turn | Enter row (0-2), column (0-2), and a letter A-Z: 0 0 C
```

A win is triggered when any row, column, or diagonal spells a word found in `dic.txt`.

### Example — SUS (choice 1)

```
Alice, enter your move (row col) and symbol (S/U): 1 1 U
```

Each player may place either `S` or `U` on any turn. Points are scored each time the sequence **S-U-S** is completed.

---

## Design & Architecture

### Class Hierarchy

```
Board<T>  (BoardGame_Classes.h)
├── TTT_Board              (Mohammed)   — 3x3 standard
│   └── Misere_TTT_Board   (Mohammed)   — inverted win / lose
├── C4_Board               (Mohammed)   — 6x7 gravity-based
├── Numerical_TTT_Board    (Mohammed)   — number-sum game
├── Obstacles_TTT_Board    (Mohammed)   — 6x6 with obstacles
├── XO_4_x_4_Board         (Amir)       — 4x4 TTT
├── sus_board              (Amir)       — S-U-S scoring
├── ultimate_XO_board      (Amir)       — 9x9 meta TTT
├── XO_5x5_Board           (Sayed)      — 5x5 most-sequences
├── Word_TTT_Board         (Sayed)      — letter / word game
├── Infinity_TTT_Board     (Sayed)      — disappearing marks
├── Pyramid_X_O_Board      (Abdelmalik) — pyramid shape
├── Diamond_XO_Board       (Abdelmalik) — 7x7 diamond shape
└── Memory_XO_Board        (Abdelmalik) — hidden marks

Player<T>       — base player (name, symbol, PlayerType)
UI<T>           — abstract: get_move, setup_players, display_board_matrix
GameManager<T>  — orchestrates the alternating-player game loop
```

### Core Classes

#### `Board<T>`

Stores a `vector<vector<T>> board`, dimensions, and move count. Pure virtual methods: `update_board`, `is_win`, `is_lose`, `is_draw`, `game_is_over`. All concrete boards also implement `undoLastMove()` to support AI simulation.

#### `Move<T>`

A lightweight value object holding `(x, y, symbol)`. Allocated on the heap by the UI layer and **deleted inside `update_board`** by convention.

#### `Player<T>`

Holds the player's name, `PlayerType` enum value (`HUMAN`, `COMPUTER`, `AI`, `RANDOM`), symbol, and a pointer back to the board. AI logic lives in each game's `UI` class — there is no separate `ComputerPlayer` subclass.

#### `UI<T>`

Abstract base providing a default `setup_players()` and a formatted `display_board_matrix()`. Each game's UI subclass overrides `get_move()` (routes to human input or the internal `computerMove()` helper) and `create_player()`. Custom board layouts override `display_board_matrix()` (pyramid, diamond, ultimate, memory).

#### `GameManager<T>`

Runs the alternating-player loop: calls `ui->get_move()`, passes the result to `board->update_board()`, retries on invalid moves, then checks win / lose / draw after every half-turn.

### AI Strategy (`GameAI`)

All AI implementations use a **Win -> Block -> Random** heuristic powered by `undoLastMove()`:

1. **Win** — try every empty cell; commit if `is_win` triggers.
2. **Block** — try every empty cell as the opponent; block if it would trigger `is_win`.
3. **Random** — pick a uniformly random valid cell.

Four generic strategies are provided in `Mohammed.h / .cpp`:

| Method | Used by |
|--------|---------|
| `getBestMove_2D<B>` | TTT, 4x4, 5x5, Infinity, Pyramid, Diamond, Memory, Obstacles |
| `getBestMove_Column<B>` | Connect 4 (column input only) |
| `getMisereMove<B>` | Misere TTT (avoids forming a line) |
| `getRandomMove<B>` | Fallback / 5x5 UI |

SUS and Ultimate TTT each have hand-crafted heuristic `computerMove()` functions.

### Data Flow

```
main()
  └─ GameManager::run()
       ├─ UI::get_move(player)          -> Move<char>*
       ├─ Board::update_board(move)     -> bool  (deletes move internally)
       ├─ UI::display_board_matrix(...)
       └─ Board::is_win / is_draw / is_lose  -> end or continue
```

---

## Extending / Adding a Game

1. **Create `MyGame.h`** — declare:
   - `class MyGame_Board : public Board<char>` implementing all five pure virtual methods plus `undoLastMove()`.
   - `class MyGame_UI : public UI<char>` implementing `get_move()`, `create_player()`, and optionally `display_board_matrix()`.

2. **Implement in `MyGame.cpp`.**

3. **Register in `Main.cpp`:**

   ```cpp
   // At the top, with the other includes:
   #include "MyGame.h"
   #include "MyGame.cpp"

   // Inside the menu if-else chain:
   else if (choice == 16) {
       board   = new MyGame_Board();
       ui      = new MyGame_UI();
       players = ui->setup_players();
   }
   ```

   Update the exit choice number and the menu `cout` lines to match.

4. **Free AI:** call `GameAI::getBestMove_2D<MyGame_Board>(player)` inside your `computerMove()` for an instant Win/Block/Random computer opponent (requires `undoLastMove()` to be correct).

---

## Known Limitations

- **Unity-build pattern** — `Main.cpp` `#include`s all `.cpp` files, so there is only one translation unit. Splitting them into separate CMake sources without restructuring will cause linker errors from duplicate template instantiations.
- **Heuristic-only AI** — no minimax or alpha-beta pruning; an experienced player can beat the computer reliably.
- **Ultimate TTT constraint not enforced** — canonical Ultimate TTT requires each move to send the opponent to a specific mini-board; this constraint is absent.
- **SUS scoring model** — `cnt1`/`cnt2` track points by *symbol* (`S` vs `U`), not by *player*, which can produce unexpected results if both players use the same symbol.
- **`dic.txt` path sensitivity** — Word TTT searches up to four relative paths; if none is found the board accepts any letter without word validation.
- **Infinity TTT removal timing** — the removal condition may not match all published rule variants.

---

## Dependencies

| Dependency | Purpose |
|------------|---------|
| C++ Standard Library | All I/O, `<vector>`, `<deque>`, `<set>`, `<map>`, `<filesystem>` |
| `dic.txt` (bundled) | Word list for Word Tic-Tac-Toe validation |
| CMake >= 3.10 | Build system |
| Doxygen *(optional)* | Generate HTML / LaTeX API docs from `Doxyfile` |

---

## Authors

| Contribution | Name |
|---|---|
| Framework design (`BoardGame_Classes.h`) | Mohammad El-Ramly |
| TTT, Misere, Connect 4, Numerical TTT, Obstacles TTT, AI engine | Mohammed |
| 4x4 XO, SUS, Ultimate Tic-Tac-Toe | Amir |
| 5x5 TTT, Word TTT, Infinity TTT | Sayed |
| Pyramid XO, Diamond XO, Memory XO | Abdelmalik |

This repository is for academic / educational use. No license is currently specified — please contact the repository owner before reusing or redistributing any part of this work.
