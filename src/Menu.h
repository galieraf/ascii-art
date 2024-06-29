#include <ncurses.h>
#include "vector"
#include "string"
#include "fstream"
#include "map"
#include "vector"
#ifndef GALIERAF_MENU_H
#define GALIERAF_MENU_H

//----------------------------------------------------------------------------------------------------------------------
/**
 * @brief Class for handling the menu and user input.
 */
class Menu {
public:
    /**
     * @brief Processes the input from the params file.
     * @param paramsFile The path to the params file.
     *
     * @throws std::runtime_error If the input file cannot be opened or a temporary file cannot be created.
     */
    void processInput(const char * paramsFile); // Zpracuje vstup

private:
    /**
     * @brief Displays the menu and handles user input.
     * @param resultFile The path to the result file.
     * @return The selected option.
     */
    int display(const char* resultFile);

    /**
     * @brief Displays the operation options and handles user input.
     * @return The selected operation.
     */
    int displayOperationOptions();

    /**
     * @brief Converts an image to ASCII art with specified parameters.
     * @param inFile The input image file.
     * @param outFile The output art file.
     * @param nCol The number of symbols in one line of the art image.
     * @param artnom The index of the art symbol array.
     * @param filterSelections The vector of selected filters.
     * @return The status of the conversion.
     */
    int convertImageToArt(char* inFile, char* outFile, int nCol, int artnom,  const std::vector<int>& filterSelections);

    /**
    * @brief Retrieves parameters from a file and returns them as an associative map.
    * @param file The reference to the ifstream object for reading the file.
    * @return An associative map containing the parameters from the file as key-value pairs.
    *
    * @throws std::invalid_argument If an invalid key is found in the file or if a required parameter is missing.
    */
    std::map<std::string, std::string> getParameters(std::ifstream& file);

};
//----------------------------------------------------------------------------------------------------------------------
#endif //GALIERAF_MENU_H
