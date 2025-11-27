/*
1. SUS
2. four in a row (mohammed)
3. 5x5 Tic-Tac-Toe (Sayed)
4. Word Tic-tac-toe (Sayed)
5.misere Tic-Tac-Toe (mohammed)
6. Diamond Tic-Tac-Toe
7. 4 x 4 Tic-Tac-Toe (amir)
8. Pyramid Tic-Tac-Toe
9. Numerical Tic-Tac-Toe (Group)   (mohammed)
10. Obstacles Tic-Tac-Toe (Group)  (mohammed)
11. Infinity Tic-Tac-Toe (Group)   (Sayed)
13. Memory Tic-Tac-Toe (Group - Bonus)

*/
#include "BoardGame_Classes.h"
//#include "Mohammed.h"
//#include "Mohammed.cpp"
//#include "Amir.h"
//#include "Amir.cpp"
//#include "Sayed.h"
//#include "Sayed.cpp"
#include "Pyramid_XO.h"
//#include "Pyramid_XO.cpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
    srand(time(0));
    
    while (true) {
        int choice;
        cout << "\n Games menu\n";
        cout << "1. SUS 3x3\n";
        cout << "2. Four in a row (6x7)\n";
        cout << "3. 5x5 Tic Tac Toe\n";
        cout << "4. Word Tic-Tac-Toe\n";
        cout << "5. Misere X-O\n";

        cout << "7. 4x4 X-O\n";
        cout << "8. Pyramid X-O\n";
        cout << "9. Num X-O (3x3)\n";
        cout << "10. Obstacles X-O (6x6)\n";
        cout << "11. Infinity Tic-Tac-Toe\n";


        cout << "14. X-O (3x3)\n";
        cout << "15. Exit\n";
        cout << "Choose game: ";
        
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        if (choice == 15) {
            cout << "kfaya l3b ro7 zaker\n";
            break;
        }
        Board<char>* board = nullptr;
        UI<char>* ui = nullptr;
        Player<char>** players = nullptr;

        if (choice == 8) {
            ui = new Pyramid_XO_UI();
            board = new Pyramid_X_O_Board();
            players = ui->setup_players();
        }

/*        if (choice == 14) {
            board = new TTT_Board();
            ui = new TTT_UI();
            players = ui->setup_players();
        }
        else if (choice == 1) {
            board = new sus_board;
            ui = new sus_ui;
            players = ui->setup_players();
        }
        else if (choice == 2) {
            board = new C4_Board();
            ui = new C4_UI();
            players = ui->setup_players();
        }
        else if (choice == 3) {
        board = new XO_5x5_Board();
        ui = new XO_5x5_UI();
        players = ui->setup_players();
        }
        else if (choice == 4) {
        board = new Word_TTT_Board();
        ui = new Word_TTT_UI();
        players = ui->setup_players();
        } 
        else if (choice == 5) {
            board = new Misere_TTT_Board();
            ui = new Misere_TTT_UI();
            players = ui->setup_players();
        }





        else if (choice == 7) {
            board = new XO_4_x_4_Board();
            ui = new XO_4_x_4_UI();
            players = ui->setup_players();
        }



        else if (choice == 9) {
            board = new Numerical_TTT_Board();
            ui = new Numerical_TTT_UI();
            players = ui->setup_players();
        }
        else if (choice == 10) {
            board = new Obstacles_TTT_Board();
            ui = new Obstacles_TTT_UI();
            players = ui->setup_players();
        }
        else if (choice == 11) {
        board = new Infinity_TTT_Board();
        ui = new Infinity_TTT_UI();
        players = ui->setup_players();
        } */  
        else {
            cout << "Invalid choice. Please select from the menu.\n";
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