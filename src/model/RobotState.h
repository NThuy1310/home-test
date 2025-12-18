#pragma once

#include "Point.h"

class RobotState {
public:
    RobotState();
    Point getCurrentPosition() const;
    void setCurrentPosition(const Point& pos);
    void reset();
    
private:
    Point mCurrentPosition;
};
