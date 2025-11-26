#include <iostream> 
#include <string>   
#include <vector>   
#include <ctime>   
#include <cstdlib> 

#include "BoardGame_Classes.h"
#include "XO_Classes.h"
#include "Pyramid_XO.h" 

using namespace std;

int main() {

    srand(time(0));

    while (true) {
        Board<char>* board = nullptr;
        UI<char>* ui = nullptr;
        Player<char>** players = nullptr;

        cout << "\n================================\n";
        cout << "          Games Menu\n";
        cout << "================================\n";
        cout << "Choose a game to play:\n";
        cout << "1. X-O (3x3) \n";
        cout << "2. Pyramid X-O\n";
        cout << "3. Exit Program\n";

        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\nInvalid input. Please enter a number.\n";
            continue;
        }

        if (choice == 3) {
            cout << "\nThank you for playing. Goodbye!\n";
            break; 
        }

        if (choice == 1) {
            ui = new XO_UI();
            board = new X_O_Board();
            players = ui->setup_players();
        }
        else if (choice == 2) {
            ui = new Pyramid_XO_UI();
            board = new Pyramid_X_O_Board();
            players = ui->setup_players();
        }
        else {
            cout << "\nInvalid choice. Please select from the menu.\n";
            cin.ignore(1000, '\n');
            continue;
        }
       
        GameManager<char> game(board, players, ui);
        game.run();
        delete board;
        delete ui;
        delete players[0];
        delete players[1];
        delete[] players;
        // delete game;
        cout << "\nPress Enter to return to the main menu...";
        cin.ignore(1000, '\n');
        cin.get();
    }
    return 0;
}
