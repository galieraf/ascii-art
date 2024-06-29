#include <iostream>
#include <cstdlib>
#include <cstring>
#include "Menu.h"
#include "image.h"
#include "sstream"
#include <chrono>
#include "algorithm"

#define ENTER_KEY 10

using namespace std;

static vector<int> filtersForAll;


static vector<string> keys = {
        "SourceFile",
        "DestinationFile",
        "Width",
        "ASCIICharacters",
        "Filters"
};

//----------------------------------------------------------------------------------------------------------------------
int Menu::display(const char* resultFile) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS, NULL);

    const char* choices[] = {
            "[1] Next Slide",
            "[2] Pause",
            "[3] Continue",
            "[4] Operation",
            "[5] Stop"
    };
    int nChoices = sizeof(choices) / sizeof(choices[0]);

    int choice;
    int highlight = 0;

    int timer = 10; // Timer in seconds
    bool isPaused = false; // Flag for pause

    chrono::steady_clock::time_point startTime; // Start time of the loop
    chrono::steady_clock::time_point pauseTime; // Pause time
    int remainingSeconds = timer; // Remaining seconds on the timer

    while (true) {
        erase(); // Clear the screen before each display

        // Display menu choices
        for (int i = 0; i < nChoices; i++) {
            if (i == highlight) {
                attron(A_REVERSE);
            }
            mvprintw(i + 1, 1, "%s", choices[i]);
            attroff(A_REVERSE);
        }

        // Display result in the menu
        if (resultFile != nullptr) {
            std::ifstream file(resultFile);
            if (file.is_open()) {
                std::string line;
                int row = nChoices + 5; // Row coordinate to start printing the file content
                int col = 0; // Column coordinate to start printing the file content

                mvprintw(nChoices + 4, 0, "Conversion result:");
                mvprintw(nChoices + 5, 0, "------------------");

                while (getline(file, line)) {
                    mvprintw(row, col, "%s", line.c_str());
                    row++;
                }

                file.close();
            }
        }

        // Display timer
        if (!isPaused) {
            if (startTime == std::chrono::steady_clock::time_point()) {
                startTime = std::chrono::steady_clock::now(); // Set the start time of the loop
            }

            auto currentTime = std::chrono::steady_clock::now(); // Current time
            auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
            remainingSeconds = timer - static_cast<int>(elapsedSeconds);

            if (remainingSeconds <= 0) {
                // Automatically select "Next Slide" when the timer expires
                endwin();
                return 1;
            }

            mvprintw(nChoices + 3, 0, "Auto switch in %d seconds", remainingSeconds);
        }

        refresh();

        if (!isPaused) {
            timeout(100); // Set input waiting timeout to 100 milliseconds

            choice = getch();

            MEVENT event;
            if (getmouse(&event) == OK) {
                if (event.bstate & BUTTON4_PRESSED) {
                    // Scroll up, decrease highlight
                    highlight--;
                    if (highlight < 0) {
                        highlight = 0;
                    }
                }
                else if (event.bstate & BUTTON5_PRESSED) {
                    // Scroll down, increase highlight
                    highlight++;
                    if (highlight >= nChoices) {
                        highlight = nChoices - 1;
                    }
                }
            }

            switch (choice) {
                case KEY_UP:
                    highlight--;
                    if (highlight < 0) {
                        highlight = 0;
                    }
                    break;
                case KEY_DOWN:
                    highlight++;
                    if (highlight >= nChoices) {
                        highlight = nChoices - 1;
                    }
                    break;
                case ENTER_KEY:
                    if (highlight + 1 == 2) {
                        // Pause when "Pause" is selected
                        isPaused = true;
                        pauseTime = std::chrono::steady_clock::now(); // Record the pause time
                    }
                    else if (highlight + 1 == 3) {
                        // Continue without previous pause
                        continue;
                    }
                    else if (highlight + 1 == 4) {
                        // Open the operations menu when "Operation" is selected
                        endwin();
                        return 4;
                    }
                    else {
                        endwin();
                        return highlight + 1;
                    }
            }
        }
        else {
            mvprintw(nChoices + 3, 0, "Paused");
            refresh();
            timeout(-1); // Wait for input without timeout
            choice = getch();

            MEVENT event;
            if (getmouse(&event) == OK) {
                if (event.bstate & BUTTON4_PRESSED) {
                    // Scroll up, decrease highlight
                    highlight--;
                    if (highlight < 0) {
                        highlight = 0;
                    }
                }
                else if (event.bstate & BUTTON5_PRESSED) {
                    // Scroll down, increase highlight
                    highlight++;
                    if (highlight >= nChoices) {
                        highlight = nChoices - 1;
                    }
                }
            }

            switch (choice) {
                case KEY_UP:
                    highlight--;
                    if (highlight < 0) {
                        highlight = 0;
                    }
                    break;
                case KEY_DOWN:
                    highlight++;
                    if (highlight >= nChoices) {
                        highlight = nChoices - 1;
                    }
                    break;
                case ENTER_KEY: // Enter key
                    if (highlight + 1 == 2) {
                        // "Pause" is selected
                        continue;
                    }

                    if (highlight + 1 == 3) {
                        // Continue when "Continue" is selected
                        isPaused = false;
                        startTime += (std::chrono::steady_clock::now() - pauseTime); // Adjust the start time of the loop
                    }
                    else if (highlight + 1 == 4) {
                        // Open the operations menu when "Operation" is selected
                        endwin();
                        return 4;
                    }
                    else {
                        endwin();
                        return highlight + 1;
                    }
            }
        }
    }

    endwin();
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
map<string, string> Menu::getParameters(ifstream& file) {
    map<string, string> parameters;
    string line;

    while (getline(file, line)) {
        if (line.empty() || std::all_of(line.begin(), line.end(), [](char c) { return std::isspace(c); })) {
            break;  // Stop reading after an empty line
        }

        size_t separatorPos = line.find(':');
        if (separatorPos != string::npos) {
            string key = line.substr(0, separatorPos);
            string value = line.substr(separatorPos + 1);

            // Remove leading and trailing whitespaces from the key and value
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);

            if (parameters.count(key) > 0) {
                throw std::invalid_argument("Duplicate key: " + key);
            }

            if (key == "Filters for all") {
                stringstream ss(value);
                int filter;
                while (ss >> filter) {
                    if (filter < 1 || filter > 5) {
                        throw std::invalid_argument("Invalid filter value: " + std::to_string(filter));
                    }
                    filtersForAll.push_back(filter);
                }
                continue;
            }

            else if (std::find(keys.begin(), keys.end(), key) == keys.end()) {
                throw std::invalid_argument("Invalid key: " + key);
            }

            parameters[key] = value;
        }
    }

    if (file.eof() && parameters.empty()) {
        return parameters;
    }

    // Check for the presence of mandatory parameters
    if (parameters.count(keys[0]) == 0 || parameters[keys[0]].empty()) {
        throw invalid_argument("Missing parameter: SourceFile");
    }

    if (parameters.count(keys[2]) == 0) {
        throw invalid_argument("Missing parameter: Width");
    }

    // Check the value of the Width parameter
    int width;
    try {
        width = stoi(parameters[keys[2]]);
    } catch (const std::invalid_argument& e) {
        throw std::invalid_argument("Invalid parameter value: Width must be an integer");
    } catch (const std::out_of_range& e) {
        throw std::invalid_argument("Invalid parameter value: Width is out of range");
    }

    if (width < 100) {
        throw std::invalid_argument("Invalid parameter value: Width must be an integer >= 100");
    }

    if (parameters.count(keys[3]) == 0) {
        throw invalid_argument("Missing parameter: ASCIICharacters");
    }

    // Check the value of the ASCIICharacters parameter
    string asciiCharactersString = parameters[keys[3]];

    // Check for double quotes at the beginning and end of the string
    if (asciiCharactersString.front() != '"' || asciiCharactersString.back() != '"') {
        throw invalid_argument("Invalid parameter value: ASCIICharacters must be enclosed in double quotes");
    }

    // Remove double quotes at the beginning and end of the string
    asciiCharactersString = asciiCharactersString.substr(1, asciiCharactersString.size() - 2);

    // Check that the character string is not empty
    if (asciiCharactersString.empty()) {
        throw invalid_argument("Invalid parameter value: Empty ASCIICharacters array");
    }

    // Create a character vector from the string
    vector<char> asciiCharacters(asciiCharactersString.begin(), asciiCharactersString.end());

    parameters[keys[3]] = string(asciiCharacters.begin(), asciiCharacters.end());
    return parameters;
}

//----------------------------------------------------------------------------------------------------------------------
void Menu::processInput(const char* paramsFile) {
    ifstream file(paramsFile);
    FILE* f = fopen(paramsFile, "r");
    if (!f) {
        throw runtime_error("Can't open file " + string(paramsFile));
    }

    int choice = 1;
    string sourceFile;
    string destinationFile;
    int width;
    string asciiCharactersString;
    vector<int> filters;

    // Create a temporary buffer file
    static char bufferFileName[] = "/tmp/bufferXXXXXX";
    int bufferFileDescriptor = mkstemp(bufferFileName);

    if (bufferFileDescriptor == -1) {
        throw runtime_error("Failed to create a temporary file");
    }

    while (choice != 5) {
        if (choice == 1) {


            map<string, string> parameters = getParameters(file);
            if (parameters.empty()) {
                break;  // Exit the loop if there are no more parameters
            }

            filters.clear();
            filters = filtersForAll;

            sourceFile = parameters[keys[0]];
            destinationFile = parameters[keys[1]];

            // If the destination file is not specified, use the buffer file
            destinationFile = destinationFile.empty() ? bufferFileName : destinationFile;

            width = stoi(parameters[keys[2]]);
            asciiCharactersString = parameters[keys[3]];

            //-------------------------------------------------
            //get filters
            stringstream ss(parameters[keys[4]]);
            int filter;
            while (ss >> filter) {
                if (filter < 1 || filter > 5) {
                    throw std::invalid_argument("Invalid filter value: " + std::to_string(filter));
                }
                filters.push_back(filter);
            }

            //-------------------------------------------------

            // creating a vector from ASCIICharacters string
            vector<char> asciiCharacters(asciiCharactersString.begin(), asciiCharactersString.end());
            // deleting duplicates
            auto it = std::unique(asciiCharacters.begin(), asciiCharacters.end());
            asciiCharacters.resize(std::distance(asciiCharacters.begin(), it));

            IMAGE *img = new IMAGE(sourceFile.c_str(), destinationFile.c_str(), width, asciiCharactersString.data(),
                                   filters);
            delete img;
        } else if (choice == 4) {
            int operationChoice = displayOperationOptions();
            if (operationChoice == 3) {
                continue;
            } else if (operationChoice >= 7 && operationChoice <= 12) {
                int filterSelected = operationChoice - 6;
                filters.push_back(filterSelected);

                IMAGE* img = new IMAGE(sourceFile.c_str(), destinationFile.c_str(), width, asciiCharactersString.data(), filters);
                delete img;
            }
        }

        choice = display(destinationFile.c_str());

        // Remove the buffer file
        remove(bufferFileName);
    }

    if (choice == 5) {
        cout << "Program stopped by user." << endl;
    } else {
        cout << "End of the program. No more examples." << endl;
    }

    file.close();
    fclose(f);
    //endwin();
}


//----------------------------------------------------------------------------------------------------------------------
int Menu::displayOperationOptions() {
    const char* choices[] = {
            "[1] Edge detection",
            "[2] Mirror",
            "[3] Sharpen",
            "[4] Negative",
            "[5] Blur",
            "[6] Back"
    };
    int nChoices = sizeof(choices) / sizeof(choices[0]);

    int choice;
    int highlight = 0;

    clear();

    while (true) {
        for (int i = 0; i < nChoices; i++) {
            if (i == highlight) {
                attron(A_REVERSE);
            }
            mvprintw(i + 1, 1, "%s", choices[i]);
            attroff(A_REVERSE);
        }

        refresh();

        choice = getch();

        MEVENT event;
        if (getmouse(&event) == OK) {
            if (event.bstate & BUTTON4_PRESSED) {
                // Scroll up, decrease highlight
                highlight--;
                if (highlight < 0) {
                    highlight = 0;
                }
            }
            else if (event.bstate & BUTTON5_PRESSED) {
                // Scroll down, increase highlight
                highlight++;
                if (highlight >= nChoices) {
                    highlight = nChoices - 1;
                }
            }
        }

        switch (choice) {
            case KEY_UP:
                highlight--;
                if (highlight < 0) {
                    highlight = 0;
                }
                break;
            case KEY_DOWN:
                highlight++;
                if (highlight >= nChoices) {
                    highlight = nChoices - 1;
                }
                break;
            case 10: // Enter key
                if (highlight + 1 == 6) {
                    return 2;

                } else {
                    return 6 + highlight + 1;
                }
        }
    }
}
//----------------------------------------------------------------------------------------------------------------------
