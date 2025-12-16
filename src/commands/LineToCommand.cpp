#include "LineToCommand.h"
#include <stdexcept>
#include <sstream>

void LineToCommand::execute(Grid& grid, RobotState& robot) {
    if (!grid.isInitialized()) {
        throw std::runtime_error("Grid not initialized. Use DIMENSION command first.");
    }

    Point current = robot.getPosition();

    if (!grid.isValidCoordinate(mTarget.x, mTarget.y)) {
        std::ostringstream oss;
        oss << "LINE_TO coordinates (" << mTarget.x << ", " << mTarget.y 
            << ") out of bounds [0, " << grid.getSize() - 1 << "]";
        throw std::out_of_range(oss.str());
    }

    // Draw line from current position to target
    grid.drawLine(current, mTarget);
    
    // Update robot position
    robot.setPosition(mTarget);
}

bool LineToCommand::validate() const {
    // Coordinates can be any integer; bounds checking done at execution time
    return true;
}
