#include "DimensionCommand.h"
#include <stdexcept>

void DimensionCommand::execute(Grid& grid, RobotState& robot) {
    if (!validate()) {
        throw std::invalid_argument("Invalid DIMENSION command: size must be positive");
    }

    grid.initialize(mSize);
    robot.reset();  // Reset robot to origin when grid is initialized
}

bool DimensionCommand::validate() const {
    return mSize > 0;
}
