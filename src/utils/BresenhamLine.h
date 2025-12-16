#ifndef BRESENHAM_LINE_H
#define BRESENHAM_LINE_H

#include "../model/Point.h"
#include <vector>

/**
 * @brief Bresenham's line algorithm implementation
 * Generates all points on a line between two points
 */
class BresenhamLine {
public:
    /**
     * @brief Calculate all points on a line from start to end
     * @param start Starting point
     * @param end Ending point
     * @return Vector of all points on the line (including start and end)
     */
    static std::vector<Point> getLine(const Point& start, const Point& end);
};

#endif // BRESENHAM_LINE_H
