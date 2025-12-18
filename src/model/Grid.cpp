#include "Grid.h"
#include <stdexcept>

Grid::Grid() : mSize(0), mInitialized(false) {}

void Grid::initialize(size_t size) {
    // Guard against overflow: size*size must fit in size_t and be reasonably allocatable
    if (size == 0) {
        mSize = 0;
        mCells.clear();
        mInitialized = false;
        return;
    }

    const size_t max = std::numeric_limits<size_t>::max();
    if (size > 0 && size > max / size) {
        throw std::overflow_error("Grid size is too large to allocate");
    }

    const size_t total = size * size;
    mSize = size;
    mCells.resize(total, false);
    mInitialized = true;
}

void Grid::markCell(const Point& point) {
    if (isValidPoint(point)) {
        mCells[point.y * mSize + point.x] = true;
    }
}

bool Grid::isValidPoint(const Point& point) const {
    return point.x >= 0 && point.x < static_cast<int>(mSize) &&
           point.y >= 0 && point.y < static_cast<int>(mSize);
}

size_t Grid::getSize() const {
    return mSize;
}

bool Grid::isInitialized() const {
    return mInitialized;
}

bool Grid::isCellMarked(int x, int y) const {
    if (x >= 0 && x < static_cast<int>(mSize) && y >= 0 && y < static_cast<int>(mSize)) {
        return mCells[y * mSize + x];
    }
    return false;
}

void Grid::reset() {
    mSize = 0;
    mCells.clear();
    mInitialized = false;
}