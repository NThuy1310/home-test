#include "DimensionCommand.h"
#include <stdexcept>

void DimensionCommand::execute(Grid& grid, RobotState& robot) {
    if (!(mSize > 0)) {
        throw std::invalid_argument("Invalid DIMENSION command: size must be positive");
    }

    grid.initialize(mSize);
    robot.setCurrentPosition(Point(0, 0));
}

bool DimensionCommand::validate(const Grid& /*grid*/) const {
    return mSize > 0;
}
