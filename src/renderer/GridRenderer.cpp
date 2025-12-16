#include "GridRenderer.h"
#include <iostream>
#include <sstream>

std::string GridRenderer::render(const Grid& grid) {
    if (!grid.isInitialized()) {
        return "Grid not initialized\n";
    }

    std::ostringstream oss;
    size_t size = grid.getSize();

    for (size_t y = 0; y < size; ++y) {
        for (size_t x = 0; x < size; ++x) {
            oss << grid.getCell(x, y);
            if (x < size - 1) {
                oss << ' ';  // Space between cells
            }
        }
        oss << '\n';
    }

    return oss.str();
}

void GridRenderer::print(const Grid& grid) {
    std::cout << render(grid);
}
