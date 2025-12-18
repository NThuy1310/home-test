#pragma once

#include <vector>
#include <mutex>
#include "Point.h"

class Grid {
public:
    Grid();
    
    void initialize(size_t size);
    void markCell(const Point& point);
    bool isValidPoint(const Point& point) const;
    size_t getSize() const;
    bool isInitialized() const;
    bool isCellMarked(int x, int y) const;
    void reset();

private:
    size_t mSize;
    std::vector<bool> mCells;
    bool mInitialized;
};
