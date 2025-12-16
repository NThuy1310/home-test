#include "Grid.h"
#include "../utils/BresenhamLine.h"
#include <sstream>

void Grid::initialize(size_t gridSize) {
    if (gridSize == 0) {
        throw std::invalid_argument("Grid size must be positive");
    }

    std::lock_guard<std::mutex> lock(mMutex);
    
    mSize = gridSize;
    mInitialized = true;
    mUseSparse = (gridSize >= SPARSE_THRESHOLD);
    
    if (mUseSparse) {
        // Use sparse representation for large grids
        mMarkedCells.clear();
    } else {
        // Use dense representation for small/medium grids
        mDenseGrid.assign(gridSize, std::vector<char>(gridSize, '.'));
    }
}

void Grid::markCell(int x, int y) {
    std::lock_guard<std::mutex> lock(mMutex);
    
    if (!mInitialized) {
        throw std::runtime_error("Grid not initialized. Use DIMENSION command first.");
    }
    
    if (x < 0 || y < 0 || static_cast<size_t>(x) >= mSize || static_cast<size_t>(y) >= mSize) {
        std::ostringstream oss;
        oss << "Coordinates (" << x << ", " << y << ") out of bounds [0, " << mSize - 1 << "]";
        throw std::out_of_range(oss.str());
    }
    
    if (mUseSparse) {
        mMarkedCells.insert(Point(x, y));
    } else {
        mDenseGrid[y][x] = '+';
    }
}

void Grid::drawLine(const Point& from, const Point& to) {
    // Validate coordinates first (will throw if invalid)
    {
        std::lock_guard<std::mutex> lock(mMutex);
        
        if (!mInitialized) {
            throw std::runtime_error("Grid not initialized. Use DIMENSION command first.");
        }
        
        if (!isValidCoordinate(from.x, from.y)) {
            std::ostringstream oss;
            oss << "Start coordinates (" << from.x << ", " << from.y 
                << ") out of bounds [0, " << mSize - 1 << "]";
            throw std::out_of_range(oss.str());
        }
        
        if (!isValidCoordinate(to.x, to.y)) {
            std::ostringstream oss;
            oss << "End coordinates (" << to.x << ", " << to.y 
                << ") out of bounds [0, " << mSize - 1 << "]";
            throw std::out_of_range(oss.str());
        }
    }
    
    // Calculate line points (outside lock to reduce contention)
    std::vector<Point> points = BresenhamLine::getLine(from, to);
    
    // Mark all points (with lock)
    std::lock_guard<std::mutex> lock(mMutex);
    
    if (mUseSparse) {
        for (const auto& point : points) {
            mMarkedCells.insert(point);
        }
    } else {
        for (const auto& point : points) {
            mDenseGrid[point.y][point.x] = '+';
        }
    }
}

char Grid::getCell(int x, int y) const {
    std::lock_guard<std::mutex> lock(mMutex);
    
    if (!mInitialized) {
        throw std::runtime_error("Grid not initialized");
    }
    
    if (x < 0 || y < 0 || static_cast<size_t>(x) >= mSize || static_cast<size_t>(y) >= mSize) {
        throw std::out_of_range("Coordinates out of bounds");
    }
    
    if (mUseSparse) {
        return mMarkedCells.find(Point(x, y)) != mMarkedCells.end() ? '+' : '.';
    } else {
        return mDenseGrid[y][x];
    }
}

bool Grid::isValidCoordinate(int x, int y) const {
    // Note: mutex should be locked by caller
    return x >= 0 && y >= 0 && 
           static_cast<size_t>(x) < mSize && 
           static_cast<size_t>(y) < mSize;
}

std::vector<std::pair<int, int>> Grid::getMarkedCells() const {
    std::lock_guard<std::mutex> lock(mMutex);
    
    std::vector<std::pair<int, int>> cells;
    
    if (!mInitialized) {
        return cells;
    }
    
    if (mUseSparse) {
        for (const auto& point : mMarkedCells) {
            cells.emplace_back(point.y, point.x);
        }
    } else {
        for (size_t y = 0; y < mSize; ++y) {
            for (size_t x = 0; x < mSize; ++x) {
                if (mDenseGrid[y][x] == '+') {
                    cells.emplace_back(y, x);
                }
            }
        }
    }
    
    return cells;
}

void Grid::clear() {
    std::lock_guard<std::mutex> lock(mMutex);
    
    if (mUseSparse) {
        mMarkedCells.clear();
    } else {
        for (auto& row : mDenseGrid) {
            std::fill(row.begin(), row.end(), '.');
        }
    }
}
