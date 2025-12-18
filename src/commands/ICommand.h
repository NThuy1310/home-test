#pragma once

#include "model/Grid.h"
#include "model/RobotState.h"
#include <string>

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute(Grid& grid, RobotState& robot) = 0;
    virtual bool validate(const Grid& grid) const = 0;
    virtual std::string getType() const = 0;
};
