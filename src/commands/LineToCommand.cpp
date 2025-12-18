#include "commands/LineToCommand.h"
#include "utils/BresenhamLine.h"
#include <stdexcept>
#include <sstream>

void LineToCommand::execute(Grid& grid, RobotState& robot) {
    if (!grid.isInitialized()) {
        throw std::runtime_error("Grid not initialized. Use DIMENSION command first.");
    }

    Point current = robot.getCurrentPosition();

    if (!grid.isValidPoint(mTarget)) {
        std::ostringstream oss;
        oss << "LINE_TO coordinates (" << mTarget.x << ", " << mTarget.y 
            << ") out of bounds [0, " << grid.getSize() - 1 << "]";
        throw std::out_of_range(oss.str());
    }

    auto line = BresenhamLine::getLine(current, mTarget);
    for (const auto& point : line) {
        grid.markCell(point);
    }
    
    robot.setCurrentPosition(mTarget);
}

bool LineToCommand::validate(const Grid& grid) const {
    return grid.isInitialized() && grid.isValidPoint(mTarget);
}
