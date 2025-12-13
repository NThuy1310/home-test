#include <iostream>
#include <vector>
#include <cmath>
#include <cstdint>
#include <functional>
#include <algorithm>

using Coord = std::int64_t;

/**
 * @brief Draw Bresenham Line
 */
inline void bresenhamLine(Coord x0, Coord y0, Coord x1, Coord y1, 
                          const std::function<void(Coord, Coord)>& plot) noexcept {
    if (x0 == x1 && y0 == y1) {
        plot(x0, y0);
        return;
    }

    Coord dx = std::llabs(x1 - x0);
    Coord dy = std::llabs(y1 - y0);
    Coord sx = (x0 < x1) ? 1 : -1;
    Coord sy = (y0 < y1) ? 1 : -1;
    Coord err = dx - dy;

    while(true) {
        plot(x0,y0);
        if (x0 == x1 && y0 == y1) break;
        Coord err2 = err * 2;
        if (err2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (err2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

// Helper: collect plotted points from bresenhamLine
static std::vector<std::pair<Coord, Coord>> collectPoints(Coord x0, Coord y0, Coord x1, Coord y1) {
    std::vector<std::pair<Coord, Coord>> pts;
    bresenhamLine(x0, y0, x1, y1, [&](Coord x, Coord y){ pts.emplace_back(x, y); });
    return pts;
}

// Helper: verify Bresenham invariants
static bool verifyBresenham(const std::vector<std::pair<Coord, Coord>>& pts,
                            Coord x0, Coord y0, Coord x1, Coord y1) {
    if (pts.empty()) return false;
    if (pts.front() != std::make_pair(x0, y0)) return false;
    if (pts.back()  != std::make_pair(x1, y1)) return false;
    Coord dx = std::llabs(x1 - x0);
    Coord dy = std::llabs(y1 - y0);
    // expected length
    std::size_t expected_len = static_cast<std::size_t>(std::max(dx, dy) + 1);
    if (pts.size() != expected_len) return false;

    // each step moves at most 1 in x and at most 1 in y and not zero move
    for (std::size_t i = 1; i < pts.size(); ++i) {
        Coord px = pts[i-1].first, py = pts[i-1].second;
        Coord cx = pts[i].first, cy = pts[i].second;
        Coord stepx = std::llabs(cx - px);
        Coord stepy = std::llabs(cy - py);
        if (stepx > 1 || stepy > 1) return false;
        if (stepx == 0 && stepy == 0) return false;
    }
    return true;
}

// Test cases
static bool test_horizontal() {
    auto pts = collectPoints(2, 2, 6, 2);
    bool ok = verifyBresenham(pts, 2, 2, 6, 2);
    std::cout << "test_horizontal: " << (ok ? "PASS" : "FAIL") << '\n';
    return ok;
}

static bool test_vertical() {
    auto pts = collectPoints(3, 1, 3, 4);
    bool ok = verifyBresenham(pts, 3, 1, 3, 4);
    std::cout << "test_vertical: " << (ok ? "PASS" : "FAIL") << '\n';
    return ok;
}

static bool test_diagonal45() {
    auto pts = collectPoints(1, 1, 4, 4);
    bool ok = verifyBresenham(pts, 1, 1, 4, 4);
    std::cout << "test_diagonal45: " << (ok ? "PASS" : "FAIL") << '\n';
    return ok;
}

static bool test_steep() {
    auto pts = collectPoints(2, 2, 3, 7);
    bool ok = verifyBresenham(pts, 2, 2, 3, 7);
    std::cout << "test_steep: " << (ok ? "PASS" : "FAIL") << '\n';
    return ok;
}

static bool test_single_point() {
    auto pts = collectPoints(5, 5, 5, 5);
    bool ok = verifyBresenham(pts, 5, 5, 5, 5);
    std::cout << "test_single_point: " << (ok ? "PASS" : "FAIL") << '\n';
    return ok;
}

static bool test_reverse_direction() {
    auto pts = collectPoints(8, 6, 3, 2);
    bool ok = verifyBresenham(pts, 8, 6, 3, 2);
    std::cout << "test_reverse_direction: " << (ok ? "PASS" : "FAIL") << '\n';
    return ok;
}

// Run all tests and return number of failed tests
static int run_bresenham_tests() {
    int fails = 0;
    fails += test_horizontal() ? 0 : 1;
    fails += test_vertical() ? 0 : 1;
    fails += test_diagonal45() ? 0 : 1;
    fails += test_steep() ? 0 : 1;
    fails += test_single_point() ? 0 : 1;
    fails += test_reverse_direction() ? 0 : 1;
    if (fails == 0) std::cout << "All bresenhamLine tests PASSED\n\n";
    else std::cout << fails << " bresenhamLine test(s) FAILED\n\n";
    return fails;
}

// Reusable grid printing helper (same format as earlier)
static void printGrid(const std::vector<std::string>& grid, const std::string& title) {
    const int N = static_cast<int>(grid.size());
    std::cout << title << '\n';
    std::cout << "  ";
    for (int i = 0; i < N; ++i) std::cout << i << ' ';
    std::cout << '\n';
    for (int r = 0; r < N; ++r) {
        std::cout << r << ' ';
        for (int c = 0; c < N; ++c) {
            std::cout << grid[r][c];
            if (c + 1 < N) std::cout << ' ';
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

// Demo/test-case: horizontal line
static void demo_horizontal_grid() {
    const int N = 8;
    std::vector<std::string> grid(N, std::string(N, '.'));
    int rx = 1, ry = 3; // start
    printGrid(grid, "Demo horizontal - BEFORE:");
    // LINE_TO 6,3
    bresenhamLine(rx, ry, 6, 3, [&](Coord x, Coord y){
        if (x >= 0 && y >= 0 && x < N && y < N) grid[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)] = '+';
    });
    printGrid(grid, "Demo horizontal - AFTER:");
}

// Demo/test-case: vertical line
static void demo_vertical_grid() {
    const int N = 7;
    std::vector<std::string> grid(N, std::string(N, '.'));
    int rx = 4, ry = 1; // start
    printGrid(grid, "Demo vertical - BEFORE:");
    bresenhamLine(rx, ry, 4, 5, [&](Coord x, Coord y){
        if (x >= 0 && y >= 0 && x < N && y < N) grid[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)] = '+';
    });
    printGrid(grid, "Demo vertical - AFTER:");
}

// Demo/test-case: diagonal and steep
static void demo_diagonal_and_steep() {
    const int N = 10;
    std::vector<std::string> grid(N, std::string(N, '.'));
    // diagonal
    printGrid(grid, "Demo diagonal45 - BEFORE:");
    bresenhamLine(1,1,5,5, [&](Coord x, Coord y){ if (x>=0 && y>=0 && x<N && y<N) grid[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)] = '+'; });
    printGrid(grid, "Demo diagonal45 - AFTER:");

    // clear and steep
    grid.assign(N, std::string(N, '.'));
    printGrid(grid, "Demo steep - BEFORE:");
    bresenhamLine(2,2,3,9, [&](Coord x, Coord y){ if (x>=0 && y>=0 && x<N && y<N) grid[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)] = '+'; });
    printGrid(grid, "Demo steep - AFTER:");
}

// Demo/test-case: single point and reverse
static void demo_point_and_reverse() {
    const int N = 6;
    std::vector<std::string> grid(N, std::string(N, '.'));
    printGrid(grid, "Demo single-point - BEFORE:");
    bresenhamLine(3,3,3,3, [&](Coord x, Coord y){ if (x>=0 && y>=0 && x<N && y<N) grid[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)] = '+'; });
    printGrid(grid, "Demo single-point - AFTER:");

    grid.assign(N, std::string(N, '.'));
    printGrid(grid, "Demo reverse-direction - BEFORE:");
    // the reverse demo uses coordinates outside the small grid intentionally to show bounds checks
    bresenhamLine(8,4,2,1, [&](Coord x, Coord y){ if (x>=0 && y>=0 && x<9 && y<6) { if (x < N && y < N) grid[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)] = '+'; } });
    printGrid(grid, "Demo reverse-direction - AFTER:");
}

int main() {
    // Run unit tests for bresenhamLine first
    int testFails = run_bresenham_tests();
    if (testFails != 0) return testFails;

    // Test Case 3: Problem specification example - 5x5 grid
    std::cout << "Test 3 - Problem specification example (5x5 grid):\n";
    std::cout << "Commands: DIMENSION 5, MOVE_TO 1,1, LINE_TO 3,3, LINE_TO 3,2\n\n";
    
    const int N = 5;
    std::vector<std::string> grid(N, std::string(N, '.'));
    
    int robotX = 0, robotY = 0;  // Initial position

    // helper to print current grid (no coordinates, only cells)
    auto printGridSimple = [&](const char* title){
        std::cout << title << '\n';
        std::cout << "  ";
        for (int i = 0; i < N; ++i) std::cout << i << " ";
        std::cout << '\n';
        for (int r = 0; r < N; ++r) {
            std::cout << r << ' ';
            for (int c = 0; c < N; ++c) {
                std::cout << grid[r][c];
                if (c + 1 < N) std::cout << ' ';
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    };

    // Execute commands and mark visited cells with '+'
    // MOVE_TO 1,1 (no drawing, just move)
    robotX = 1;
    robotY = 1;

    // LINE_TO 3,3 (draw from current position to target)
    bresenhamLine(static_cast<Coord>(robotX), static_cast<Coord>(robotY),
                  static_cast<Coord>(3), static_cast<Coord>(3),
                  [&](Coord x, Coord y){
                      if (x >= 0 && y >= 0 &&
                          x < static_cast<Coord>(N) && y < static_cast<Coord>(N)) {
                          grid[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)] = '+';
                      }
                  });
    robotX = 3;
    robotY = 3;

    // LINE_TO 3,2 (draw from current position to target)
    bresenhamLine(static_cast<Coord>(robotX), static_cast<Coord>(robotY),
                  static_cast<Coord>(3), static_cast<Coord>(2),
                  [&](Coord x, Coord y){
                      if (x >= 0 && y >= 0 &&
                          x < static_cast<Coord>(N) && y < static_cast<Coord>(N)) {
                          grid[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)] = '+';
                      }
                  });
    robotX = 3;
    robotY = 2;

    // Print grid AFTER executing commands (visited cells shown as '+')
    printGridSimple("Grid AFTER commands:");
    
    // Additional demos: print grid before/after for several cases
    demo_horizontal_grid();
    demo_vertical_grid();
    demo_diagonal_and_steep();
    demo_point_and_reverse();

    return 0;
}