#pragma once

#include "commands/ICommand.h"
#include "model/Point.h"
#include <string>

class MoveToCommand : public ICommand {
public:
    MoveToCommand(int x, int y) : mTarget(x, y) {}

    void execute(Grid& grid, RobotState& robot) override;
    bool validate(const Grid& grid) const override;
    std::string getType() const override { return "MOVE_TO"; }

    Point getTarget() const { return mTarget; }

private:
    Point mTarget;
};
