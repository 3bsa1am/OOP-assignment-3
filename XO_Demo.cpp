/**
 * @file XO_Demo.cpp
 * @brief Entry point for the FCAI X-O (Tic-Tac-Toe) game.
 *
 * This file initializes the X-O game by creating the user interface, board, and players.
 * It then launches the game loop via the GameManager class.
 * All dynamically allocated objects are properly deleted at the end.
 */

#include <iostream> // Required for input/output operations (cout, cin)
#include <string>   // Required for string
#include <vector>   // Required for vector
#include <memory>   // Required for unique_ptr
#include <ctime>   
#include <cstdlib> 

#include "BoardGame_Classes.h"
#include "XO_Classes.h"
#include "Pyramid_XO.h" 

using namespace std;

int main() {
    srand(static_cast<unsigned int>(time(0)));

    Board<char>* game_board = nullptr;
    UI<char>* game_ui = nullptr;


    cout << "================================\n";
    cout << "          Games menu\n";
    cout << "================================\n";
    cout << "Choose a game to play:\n";
    cout << "1. X-O (3x3)\n";
    cout << "2: Pyramid X-O\n";
    cout << "Enter your choice (1 or 2): ";

    int choice;
    cin >> choice;


    if (choice == 1) {
        game_ui = new XO_UI();
        game_board = new X_O_Board();
    }
    else if (choice == 2) {
        game_ui = new Pyramid_XO_UI();
        game_board = new Pyramid_X_O_Board();
    }
    else {
        cout << "\nInvalid choice. Exiting the program.\n";
        return 0; 
    }

    // Use the UI to set up the players for the game.
    // The UI returns a dynamically allocated array of Player pointers.
    Player<char>** players = game_ui->setup_players();

    GameManager<char> game_manager(game_board, players, game_ui);
    game_manager.run();


    delete game_board;
    // Delete the individual player objects.
    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }
    // Delete the dynamically allocated array of player pointers itself.
    delete[] players;

    return 0; // Exit successfully
}