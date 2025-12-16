#include <iostream>
#include <string>
#include "controller/Controller.h"

/**
 * @brief Robot Movement Application
 * 
 * Reads commands from a file and draws on an N×N grid using Bresenham line algorithm.
 * Supports concurrent parsing and execution for optimal performance with large files.
 */

int main(int argc, char* argv[]) {
    // Check command line arguments
    if (argc < 2 || argc > 3) {
        Controller::printUsage(argv[0]);
        return 1;
    }

    std::string filename = argv[1];
    bool realtimeMode = false;
    
    // Check for --realtime flag
    if (argc == 3 && std::string(argv[2]) == "--realtime") {
        realtimeMode = true;
    }

    // Create controller and run application
    Controller controller;
    return controller.run(filename, realtimeMode);
}
