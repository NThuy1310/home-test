#include "MoveToCommand.h"
#include <stdexcept>
#include <sstream>

void MoveToCommand::execute(Grid& grid, RobotState& robot) {
    if (!grid.isInitialized()) {
        throw std::runtime_error("Grid not initialized. Use DIMENSION command first.");
    }

    if (!grid.isValidPoint(mTarget)) {
        std::ostringstream oss;
        oss << "MOVE_TO coordinates (" << mTarget.x << ", " << mTarget.y 
            << ") out of bounds [0, " << grid.getSize() - 1 << "]";
        throw std::out_of_range(oss.str());
    }

    robot.setCurrentPosition(mTarget);
}

bool MoveToCommand::validate(const Grid& grid) const {
    return grid.isInitialized() && grid.isValidPoint(mTarget);
}
