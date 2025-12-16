#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../model/Grid.h"
#include "../model/RobotState.h"
#include "../model/ThreadSafeQueue.h"
#include "../executor/CommandExecutor.h"
#include <string>
#include <vector>
#include <memory>

/**
 * @brief Main controller for robot application
 * 
 * Manages the complete workflow: parsing, execution, and rendering
 * Encapsulates all application logic from main.cpp
 */
class Controller {
public:
    /**
     * @brief Construct controller
     */
    Controller();

    /**
     * @brief Run the robot application
     * @param filename Path to command file
     * @param realtimeMode Enable real-time rendering
     * @return Exit code (0 for success, 1 for failure)
     */
    int run(const std::string& filename, bool realtimeMode);

    /**
     * @brief Print usage information
     * @param programName Name of the program
     */
    static void printUsage(const char* programName);

private:
    /**
     * @brief Render callback for real-time mode
     * @param commandType Type of command executed
     * @param lineNumber Line number of command
     */
    void renderCallback(const std::string& commandType, size_t lineNumber);

    // Core components
    Grid mGrid;
    RobotState mRobot;
    ThreadSafeQueue<CommandWithMetadata> mCommandQueue;
};

#endif // CONTROLLER_H
