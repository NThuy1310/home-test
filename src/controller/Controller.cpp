#include "controller/Controller.h"
#include "commands/ICommand.h"
#include <iostream>
#include <fstream>
#include <iomanip>

Controller::Controller() = default;

// Main application loop
void Controller::run() {
    std::cout << "Robot Movement Application" << std::endl;
    std::cout << "Type 'exit' or 'quit' to terminate the program" << std::endl;
    std::cout << std::endl;
    
    while (true) {
        // Get file path from user
        std::string filename = getFilePathFromUser();
        
        // Check for exit command
        if (filename == "exit" || filename == "quit") {
            std::cout << "Exiting application..." << std::endl;
            break;
        }
        
        // Process the file
        processFile(filename);
        std::cout << std::endl;
    }
}

std::string Controller::getFilePathFromUser() {
    std::cout << "Enter file path: ";
    std::string filename;
    std::getline(std::cin, filename);
    return filename;
}

void Controller::processFile(const std::string& filename) {
    try {
        if (!isFileExists(filename)) {
            std::cerr << "Error: File not found: " << filename << std::endl;
            return;
        }
        
        // Check syntax and parse commands
        auto parseResult = Parser::parseFile(filename);
        
        if (parseResult.hasErrors) {
            std::cerr << "Parsing errors found:" << std::endl;
            for (const auto& error : parseResult.errors) {
                std::cerr << "  " << error << std::endl;
            }
            
            if (!askUserToContinue()) {
                return;
            }
        }
        
        // Validate command bounds
        Parser::validateCommandBounds(parseResult);
        
        if (parseResult.hasErrors) {
            std::cerr << "Boundary validation errors:" << std::endl;
            for (const auto& error : parseResult.errors) {
                std::cerr << "  " << error << std::endl;
            }
            
            if (!askUserToContinue()) {
                return;
            }
        }
        
        // Create commands
        auto commands = Parser::createCommands(parseResult);
        
        // Ensure DIMENSION command is first (already enforced by parser)
        if (commands.empty() || commands[0]->getType() != "DIMENSION") {
            std::cerr << "Error: DIMENSION command must be first" << std::endl;
            return;
        }

        // Execute commands sequentially on shared grid/state
        for (const auto& command : commands) {
            if (!command->validate(mGrid)) {
                std::cerr << "Error: validation failed for command " << command->getType() << std::endl;
                return;
            }
            command->execute(mGrid, mRobot);
        }

        // Render final grid with indices
        renderFinalGrid();

        // Resert grid and robot state for next file
        mGrid.reset();
        mRobot.reset();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

bool Controller::isFileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

bool Controller::askUserToContinue() {
    std::cout << "Do you want to ignore errors and continue? (y/n): ";
    std::string response;
    std::getline(std::cin, response);
    return response == "y" || response == "Y";
}

void Controller::renderFinalGrid() const {
    const size_t size = mGrid.getSize();
    if (size == 0) {
        std::cout << "(Empty grid)" << std::endl;
        return;
    }

    size_t indexWidth = 1;
    size_t temp = (size > 0) ? size - 1 : 0;
    while (temp >= 10) {
        ++indexWidth;
        temp /= 10;
    }

    std::cout << "Result:" << std::endl;

    const int colWidth = static_cast<int>(indexWidth) + 1;

    std::cout << std::setw(colWidth) << "";
    for (size_t x = 0; x < size; ++x) {
        std::cout << std::setw(colWidth) << x;
    }
    std::cout << std::endl;

    for (size_t y = 0; y < size; ++y) {
        std::cout << std::setw(colWidth) << y;
        for (size_t x = 0; x < size; ++x) {
            const char symbol = mGrid.isCellMarked(static_cast<int>(x), static_cast<int>(y)) ? '+' : '.';
            std::cout << std::setw(colWidth) << symbol;
        }
        std::cout << std::endl;
    }
}
