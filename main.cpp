#include <iostream>
#include <limits> // for numeric_limits
#include "Class/Simulacia.h"

#if defined(_WIN32) || defined(_WIN64)   // čítanie klávesy z windowsu
#include <windows.h>
bool iskeypressed( unsigned timeout_ms = 0 )
{
    return WaitForSingleObject(
            GetStdHandle( STD_INPUT_HANDLE ),
            timeout_ms
    ) == WAIT_OBJECT_0;
}
#endif
#ifdef linux        // čítanie klávesy z linuxu
#include <stdio.h>
bool iskeypressed( unsigned timeout_ms = 0 )
{
    if (!initialized) return false;

    struct pollfd pls[ 1 ];
    pls[ 0 ].fd     = STDIN_FILENO;
    pls[ 0 ].events = POLLIN | POLLPRI;
    return poll( pls, 1, timeout_ms ) > 0;
}
#endif

int main() {
    std::cout << "Fire simluation!" << std::endl;
    std::cout << "Press any key to pause simulation and enter a command." << std::endl;

    Simulacia simulacia(2, 2);
    simulacia.init(50, 40, 5, 5);
    simulacia.print();

    // Run simulation in an infinite loop
    while (true) {
        simulacia.step();
        simulacia.print();
        // TODO: Nejak to nefunguje jak by to malo! Wtf kaslem na to idem spat...
        if (iskeypressed( 500 )) { // kontroluje, či bola stisknuta nejaka klavesa...
            std::cout << "Press 'f' to set a tile on fire: " << std::endl;
            std::cout << "Press 's' to Save and 'l' to Load the map from a file" << std::endl;
            std::cout << "Press 'q' to quit" << std::endl;
            char userInput;
            std::cin >> userInput;

            if (userInput  == 'q') {
                break; // Exit the loop if the user presses 'q'
            } else if (userInput == 'f') {
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
            }
            else if (userInput == 's') {    // SAVE FILE
                std::cout << "Enter a filename to save the simulation : ";
                std::string fileName;
                std::cin >> fileName;
                simulacia.saveFile(fileName.c_str());
            }
            else if (userInput == 'l') {    // LOAD FILE
                std::cout << "Enter a filename to load the simulation : ";
                std::string fileName;
                std::cin >> fileName;
                simulacia.loadFile(fileName.c_str());
            }
            else {
                // Clear the input buffer to handle unexpected input
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }

        }
    }

    return 0;
}
