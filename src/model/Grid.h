#ifndef GRID_H
#define GRID_H

#include "Point.h"
#include <vector>
#include <mutex>
#include <unordered_set>
#include <stdexcept>

/**
 * @brief Thread-safe grid implementation with optimization for large/sparse grids
 * 
 * For small to medium grids (N < 1000), uses dense 2D vector representation.
 * For large grids (N >= 1000), uses sparse representation with unordered_set.
 */
class Grid {
public:
    /**
     * @brief Default constructor - grid not initialized
     */
    Grid() : mSize(0), mInitialized(false), mUseSparse(false) {}

    /**
     * @brief Initialize grid with given size
     * @param gridSize Grid dimension (N x N)
     * @throws std::invalid_argument if gridSize <= 0
     */
    void initialize(size_t gridSize);

    /**
     * @brief Mark a single cell at given position
     * @param x X coordinate
     * @param y Y coordinate
     * @throws std::runtime_error if grid not initialized
     * @throws std::out_of_range if coordinates out of bounds
     */
    void markCell(int x, int y);

    /**
     * @brief Draw a line from current position to target using Bresenham algorithm
     * @param from Starting point
     * @param to Ending point
     * @throws std::runtime_error if grid not initialized
     * @throws std::out_of_range if coordinates out of bounds
     */
    void drawLine(const Point& from, const Point& to);

    /**
     * @brief Get the character at a specific cell
     * @param x X coordinate
     * @param y Y coordinate
     * @return '+' if marked, '.' otherwise
     * @throws std::runtime_error if grid not initialized
     * @throws std::out_of_range if coordinates out of bounds
     */
    char getCell(int x, int y) const;

    /**
     * @brief Check if grid is initialized
     * @return true if initialized, false otherwise
     */
    bool isInitialized() const {
        std::lock_guard<std::mutex> lock(mMutex);
        return mInitialized;
    }

    /**
     * @brief Get grid size
     * @return Grid dimension N
     */
    size_t getSize() const {
        std::lock_guard<std::mutex> lock(mMutex);
        return mSize;
    }

    /**
     * @brief Validate if coordinates are within grid bounds
     * @param x X coordinate
     * @param y Y coordinate
     * @return true if valid, false otherwise
     */
    bool isValidCoordinate(int x, int y) const;

    /**
     * @brief Get all marked cells (for rendering)
     * @return Vector of all marked cells (y, x format for row-major access)
     */
    std::vector<std::pair<int, int>> getMarkedCells() const;

    /**
     * @brief Clear the grid (for testing)
     */
    void clear();

private:
    mutable std::mutex mMutex;
    size_t mSize;
    bool mInitialized;
    bool mUseSparse;  // true for large grids (N >= 1000)
    
    // Dense representation (for small/medium grids)
    std::vector<std::vector<char>> mDenseGrid;
    
    // Sparse representation (for large grids)
    std::unordered_set<Point> mMarkedCells;

    // Threshold for switching to sparse representation
    static constexpr size_t SPARSE_THRESHOLD = 1000;
};

#endif // GRID_H
