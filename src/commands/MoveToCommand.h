#ifndef MOVE_TO_COMMAND_H
#define MOVE_TO_COMMAND_H

#include "ICommand.h"
#include "../model/Point.h"
#include <string>

/**
 * @brief Command to move robot to a position without drawing
 * Format: MOVE_TO x,y
 */
class MoveToCommand : public ICommand {
public:
    /**
     * @brief Construct a MOVE_TO command
     * @param x Target X coordinate
     * @param y Target Y coordinate
     */
    MoveToCommand(int x, int y) : mTarget(x, y) {}

    void execute(Grid& grid, RobotState& robot) override;
    bool validate() const override;
    std::string getType() const override { return "MOVE_TO"; }

    Point getTarget() const { return mTarget; }

private:
    Point mTarget;
};

#endif // MOVE_TO_COMMAND_H
