#include "MoveToCommand.h"
#include <stdexcept>
#include <sstream>

void MoveToCommand::execute(Grid& grid, RobotState& robot) {
    if (!grid.isInitialized()) {
        throw std::runtime_error("Grid not initialized. Use DIMENSION command first.");
    }

    if (!grid.isValidCoordinate(mTarget.x, mTarget.y)) {
        std::ostringstream oss;
        oss << "MOVE_TO coordinates (" << mTarget.x << ", " << mTarget.y 
            << ") out of bounds [0, " << grid.getSize() - 1 << "]";
        throw std::out_of_range(oss.str());
    }

    robot.setPosition(mTarget);
}

bool MoveToCommand::validate() const {
    // Coordinates can be any integer; bounds checking done at execution time
    return true;
}
