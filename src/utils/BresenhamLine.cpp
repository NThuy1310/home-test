#include "BresenhamLine.h"
#include <cmath>

std::vector<Point> BresenhamLine::getLine(const Point& start, const Point& end) {
    std::vector<Point> points;
    
    int x0 = start.x;
    int y0 = start.y;
    int x1 = end.x;
    int y1 = end.y;
    
    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    
    int err = dx - dy;
    
    int x = x0;
    int y = y0;
    
    while (true) {
        points.emplace_back(x, y);
        
        if (x == x1 && y == y1) {
            break;
        }
        
        int e2 = 2 * err;
        
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }
    
    return points;
}
