#pragma once

#include "model/Point.h"
#include <vector>

// Bresenham line algorithm
class BresenhamLine {
public:
    static std::vector<Point> getLine(const Point& start, const Point& end);
};
