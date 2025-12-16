#ifndef GRID_RENDERER_H
#define GRID_RENDERER_H

#include "../model/Grid.h"
#include <string>

/**
 * @brief Renders grid to console output
 * Handles formatting and spacing
 */
class GridRenderer {
public:
    /**
     * @brief Render grid to string
     * @param grid Grid to render
     * @return String representation of the grid
     */
    static std::string render(const Grid& grid);

    /**
     * @brief Print grid to console
     * @param grid Grid to print
     */
    static void print(const Grid& grid);
};

#endif // GRID_RENDERER_H
