#include "Controller.h"
#include "../parser/CommandParser.h"
#include "../renderer/GridRenderer.h"
#include <iostream>
#include <sstream>

Controller::Controller() : mGrid(), mRobot(), mCommandQueue() {
}

void Controller::printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " <command_file> [--realtime]\n";
    std::cout << "\nOptions:\n";
    std::cout << "  --realtime        Enable real-time rendering (shows grid updates as commands execute)\n";
    std::cout << "\nCommands:\n";
    std::cout << "  DIMENSION N       - Set grid size to N×N\n";
    std::cout << "  MOVE_TO x,y       - Move robot to (x,y) without drawing\n";
    std::cout << "  LINE_TO x,y       - Move robot to (x,y) while drawing a line\n";
    std::cout << "\nExample command file:\n";
    std::cout << "  DIMENSION 5\n";
    std::cout << "  MOVE_TO 1,1\n";
    std::cout << "  LINE_TO 3,3\n";
    std::cout << "  LINE_TO 3,2\n";
    std::cout << "\nExamples:\n";
    std::cout << "  " << programName << " commands.txt\n";
    std::cout << "  " << programName << " commands.txt --realtime\n";
}

int Controller::run(const std::string& filename, bool realtimeMode) {
    // Create callback for real-time rendering
    RenderCallback renderCallback = nullptr;
    
    if (realtimeMode) {
        std::cout << "Real-time rendering mode enabled\n" << std::endl;
        
        renderCallback = [this](const std::string& commandType, size_t lineNumber) {
            // Clear screen and reposition cursor to top
            std::cout << "\033[2J\033[H";
            
            std::cout << "=== Real-time Grid Rendering ===\n";
            std::cout << "Last executed: " << commandType << " (line " << lineNumber << ")\n\n";
            
            if (mGrid.isInitialized()) {
                GridRenderer::print(mGrid);
            } else {
                std::cout << "Grid not initialized yet...\n";
            }
            
            std::cout << std::flush;
        };
    }

    // Create executor and start execution thread
    CommandExecutor executor(mGrid, mRobot, renderCallback);
    executor.start(mCommandQueue);

    // Parse commands from file (in main thread)
    std::cout << "Parsing commands from: " << filename << std::endl;
    
    std::vector<ParseResult> parseResults = CommandParser::parseFile(filename);

    // Check if file could be opened
    if (!parseResults.empty() && !parseResults[0].success && 
        parseResults[0].line_number == 0) {
        std::cerr << "Error: " << parseResults[0].error_message << std::endl;
        mCommandQueue.shutdown();
        executor.stop();
        executor.wait();
        return 1;
    }

    // Push parsed commands to queue
    size_t commandsPushed = 0;
    std::vector<std::string> parseErrors;

    for (auto& result : parseResults) {
        if (result.success && result.command) {
            CommandWithMetadata cmdMeta(std::move(result.command), result.line_number);
            mCommandQueue.push(std::move(cmdMeta));
            ++commandsPushed;
        } else if (!result.error_message.empty()) {
            std::ostringstream oss;
            oss << "Line " << result.line_number << ": " << result.error_message;
            parseErrors.push_back(oss.str());
        }
    }

    std::cout << "Parsed " << commandsPushed << " commands" << std::endl;

    // Signal that parsing is complete
    mCommandQueue.shutdown();
    executor.stop();

    // Wait for executor to finish
    std::cout << "Executing commands..." << std::endl;
    executor.wait();

    // Collect results
    std::vector<ExecutionResult> execResults = executor.getResults();

    // Report parse errors
    if (!parseErrors.empty()) {
        std::cerr << "\n=== Parse Errors ===\n";
        for (const auto& error : parseErrors) {
            std::cerr << error << std::endl;
        }
    }

    // Report execution errors
    std::vector<std::string> execErrors;
    for (const auto& result : execResults) {
        if (!result.success) {
            std::ostringstream oss;
            oss << "Line " << result.line_number << " (" << result.command_type 
                << "): " << result.error_message;
            execErrors.push_back(oss.str());
        }
    }

    if (!execErrors.empty()) {
        std::cerr << "\n=== Execution Errors ===\n";
        for (const auto& error : execErrors) {
            std::cerr << error << std::endl;
        }
    }

    // Render the grid (if not in real-time mode)
    if (mGrid.isInitialized()) {
        if (!realtimeMode) {
            std::cout << "\n=== Final Grid ===\n";
            GridRenderer::print(mGrid);
        } else {
            std::cout << "\n=== Rendering Complete ===\n";
            std::cout << "All commands executed successfully.\n";
        }
        
        if (!parseErrors.empty() || !execErrors.empty()) {
            std::cout << "\nNote: Grid rendered with errors. See above for details.\n";
            return 1;
        }
    } else {
        std::cerr << "\nError: Grid was not initialized. No DIMENSION command found.\n";
        return 1;
    }

    return 0;
}
