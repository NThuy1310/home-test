#ifndef LINE_TO_COMMAND_H
#define LINE_TO_COMMAND_H

#include "ICommand.h"
#include "../model/Point.h"
#include <string>

/**
 * @brief Command to move robot to a position while drawing a line
 * Format: LINE_TO x,y
 */
class LineToCommand : public ICommand {
public:
    /**
     * @brief Construct a LINE_TO command
     * @param x Target X coordinate
     * @param y Target Y coordinate
     */
    LineToCommand(int x, int y) : mTarget(x, y) {}

    void execute(Grid& grid, RobotState& robot) override;
    bool validate() const override;
    std::string getType() const override { return "LINE_TO"; }

    Point getTarget() const { return mTarget; }

private:
    Point mTarget;
};

#endif // LINE_TO_COMMAND_H
