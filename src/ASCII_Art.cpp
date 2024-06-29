#include <iostream>
#include "Menu.h"

//----------------------------------------------------------------------------------------------------------------------
/**
 * @brief The main entry point of the program.
 * @param argc The number of command-line arguments.
 * @param argv An array of strings containing the command-line arguments.
 * @return An integer indicating the exit status of the program.
 */
int main(int argc, char **argv) {

    const char* ParamsFile = argv[1];
    if (argc != 2) {
        std::cout << "Usage: <program_name> <params_file>" << std::endl;
        return 1; // error
    }
    Menu menu;
    menu.processInput(ParamsFile);

    return 0;
}
//----------------------------------------------------------------------------------------------------------------------
