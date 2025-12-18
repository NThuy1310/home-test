#include "RobotState.h"

RobotState::RobotState() : mCurrentPosition(0, 0) {}

Point RobotState::getCurrentPosition() const {
    return mCurrentPosition;
}

void RobotState::setCurrentPosition(const Point& pos) {
    mCurrentPosition = pos;
}

void RobotState::reset() {
    mCurrentPosition = Point(0, 0);
}