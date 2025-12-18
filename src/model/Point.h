#pragma once

struct Point {
    int x;
    int y;
    
    Point();
    Point(int x, int y);
    
    bool operator==(const Point& other) const;
};
