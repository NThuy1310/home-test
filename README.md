# Robot Movement and Drawing System

A C++ application that simulates robot movement and drawing on a 2D grid based on command files. Parses commands, executes them multi-threaded, and renders the grid.

## Features


# Robot Movement

Small console app that parses command files, runs a grid+robot model, and renders the final grid with `+` for visited cells and `.` for empty cells. Commands: `DIMENSION N`, `MOVE_TO x,y`, `LINE_TO x,y` (Bresenham drawing).

## Architecture (brief)
- `Controller` drives: parse → validate → execute → render.
- `Parser` builds typed commands via `CommandFactory` and checks bounds.
- Commands implement `ICommand` and act on shared `Grid` and `RobotState`.
- `Grid` is a simple bitset-backed N×N store; guarded against overflow at init.

## Build
- Requirements: CMake 3.15+, C++17 compiler, Ninja (optional).
- From repo root: `./build.sh debug` (or `./build.sh release`). Binary: `build/robot`.

## Run
- Interactive: `./build/robot` then enter a file path (type `exit` to quit).
- Capture large grid output: `printf "examples/stress_test.txt\nexit\n" | ./build/robot > build/stress_output.txt`.

## Test
- Examples live in `examples/` (simple, complex, stress, error cases).
- Valgrind check: `./test/valgrind_test.sh examples/simple.txt` (or any file list).

### Using Scripts

**Linux/macOS:**
```bash
./build.sh          # Release build
./build.sh debug    # Debug build
./build.sh clean    # Clean rebuild
```

**Windows:**
```batch
build.bat           # Release build
build.bat debug     # Debug build
build.bat clean     # Clean rebuild
```

### Manual
```bash
mkdir build && cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
ninja
```

## Usage

Run `./robot`, enter file path when prompted, or type 'exit' to quit.

**Command format:**
```
DIMENSION 20 20
MOVETO 5 5
LINETO 15 5
```

## Testing

Use example files in `examples/`:
- `simple.txt`, `complex_drawing.txt`, `error_test.txt`, etc.

Run: `./robot` → enter `examples/simple.txt`

### Memory Testing with Valgrind

Run comprehensive memory leak testing:

```bash
# From project root
./test/valgrind_test.sh examples/simple.txt

# Multiple files
./test/valgrind_test.sh examples/simple.txt examples/complex_drawing.txt
```

See `test/VALGRIND_REPORT.md` for detailed memory testing results.
