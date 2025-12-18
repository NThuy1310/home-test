#pragma once

#include "parser/Parser.h"
#include "model/Grid.h"
#include "model/RobotState.h"
#include <string>

class Controller {
public:
    Controller();
    void run();

private:
    std::string getFilePathFromUser();
    void processFile(const std::string& filename);
    bool isFileExists(const std::string& filename);
    bool askUserToContinue();
    void renderFinalGrid() const;

    Grid mGrid;
    RobotState mRobot;
};
