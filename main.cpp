#include <iostream>
#include <limits> // for numeric_limits
#include "Class/Simulacia.h"

int main() {
    Simulacia simulacia(2, 2);
    simulacia.init(50, 40, 5, 5);
    simulacia.print();

    // Run simulation in an infinite loop
    while (true) {
        simulacia.step();
        simulacia.print();

        std::cout << "Press any key to continue, 'q' to quit, or 's' to set flame: ";
        char userInput;
        std::cin >> userInput;

        if (userInput == 'q') {
            break; // Exit the loop if the user presses 'q'
        } else if (userInput == 's') {
            // Ask the user how many times to call setFlame
            int numSetFlame;
            std::cout << "Enter how many times to call setFlame: ";
            std::cin >> numSetFlame;

            // Call setFlame based on user input
            for (int i = 0; i < numSetFlame; ++i) {
                int row, col;
                std::cout << "Enter row and column for setFlame (0-indexed): ";
                std::cin >> row >> col;
                simulacia.setFlame(row, col);
            }

            simulacia.print();
        } else {
            // Clear the input buffer to handle unexpected input
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    return 0;
}
1