# Robot Movement System - Architecture Design

## Overview
This project implements a robot movement and drawing system using a **Command Pattern + Observer Pattern** architecture, optimized for handling large-scale inputs with concurrent processing.

## Why Not MVC?

While MVC was initially considered, it's not the optimal choice because:
- **No interactive UI**: The system reads from file and outputs once
- **Single rendering**: No continuous view updates during user interaction
- **Command-driven**: Better suited for command processing patterns

## Chosen Architecture: Command + Observer + Pipeline

### Core Patterns

1. **Command Pattern**
   - Each command (DIMENSION, MOVE_TO, LINE_TO) is an object
   - Easy to extend with new commands (CIRCLE_TO, ARC_TO, etc.)
   - Commands are self-validating and self-executing

2. **Observer Pattern**
   - Grid observes command execution
   - Enables real-time updates as commands are processed
   - Decouples command execution from grid rendering

3. **Pipeline Pattern**
   - Parser → Command Queue → Executor → Grid
   - Supports concurrent parsing and execution
   - Optimized for large file streaming

## Architecture Diagram

```
┌─────────────────────────────────────────────────────────────┐
│                      Application Layer                       │
│  ┌──────────────┐         ┌──────────────┐                 │
│  │ CommandFile  │────────▶│  Application │                 │
│  │  Reader      │         │   Controller │                 │
│  └──────────────┘         └──────────────┘                 │
└─────────────────────────────────────────────────────────────┘
                                    │
                    ┌───────────────┼───────────────┐
                    ▼               ▼               ▼
         ┌──────────────┐  ┌──────────────┐  ┌──────────────┐
         │    Parser    │  │   Command    │  │   Executor   │
         │   (Thread 1) │─▶│    Queue     │◀─│  (Thread 2)  │
         └──────────────┘  │  (Thread-    │  └──────────────┘
                           │    Safe)     │         │
                           └──────────────┘         │
                                                    ▼
                                          ┌──────────────────┐
                                          │   Grid Model     │
                                          │  (Thread-Safe)   │
                                          └──────────────────┘
                                                    │
                                                    ▼
                                          ┌──────────────────┐
                                          │  Grid Renderer   │
                                          └──────────────────┘
```

## Component Design

### 1. Core Components

#### Grid Model (`Grid`)
- **Responsibility**: Maintains the N×N grid state
- **Thread-Safety**: Mutex-protected for concurrent access
- **Optimization**: Uses `std::vector<std::vector<char>>` for cache efficiency
- **Methods**:
  - `initialize(size_t N)`: Creates N×N grid
  - `markCell(int x, int y)`: Marks a single cell
  - `drawLine(Point from, Point to)`: Bresenham line algorithm
  - `render()`: Outputs the final grid

#### Command Interface (`ICommand`)
```cpp
class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute(Grid& grid, RobotState& robot) = 0;
    virtual bool validate() const = 0;
};
```

#### Command Implementations
- `DimensionCommand`: Initializes grid
- `MoveToCommand`: Updates robot position
- `LineToCommand`: Draws line and updates position

#### Command Parser (`CommandParser`)
- **Responsibility**: Parses text commands into Command objects
- **Features**:
  - Streaming file reading (doesn't load entire file)
  - Line-by-line parsing
  - Error handling with line numbers
  - Factory pattern for command creation

#### Command Queue (`ThreadSafeQueue<T>`)
- **Responsibility**: Thread-safe queue for producer-consumer pattern
- **Features**:
  - Condition variables for blocking operations
  - `push()`, `pop()`, `tryPop()` operations
  - Shutdown signaling

#### Command Executor (`CommandExecutor`)
- **Responsibility**: Executes commands from queue
- **Features**:
  - Runs on separate thread
  - Waits for DIMENSION before processing movement commands
  - Thread-safe grid updates

### 2. Supporting Components

#### Robot State (`RobotState`)
- Current position (x, y)
- Thread-safe position updates

#### Grid Renderer (`GridRenderer`)
- Converts grid state to console output
- Formats with proper spacing

#### Bresenham Algorithm (`BresenhamLine`)
- Efficient line drawing
- Returns all cells the line passes through

## File Structure

```
home-test/
├── CMakeLists.txt
├── ARCHITECTURE.md
├── README.md
├── src/
│   ├── main.cpp
│   ├── core/
│   │   ├── Grid.h
│   │   ├── Grid.cpp
│   │   ├── RobotState.h
│   │   └── RobotState.cpp
│   ├── commands/
│   │   ├── ICommand.h
│   │   ├── DimensionCommand.h
│   │   ├── DimensionCommand.cpp
│   │   ├── MoveToCommand.h
│   │   ├── MoveToCommand.cpp
│   │   ├── LineToCommand.h
│   │   └── LineToCommand.cpp
│   ├── parser/
│   │   ├── CommandParser.h
│   │   └── CommandParser.cpp
│   ├── executor/
│   │   ├── CommandExecutor.h
│   │   └── CommandExecutor.cpp
│   ├── utils/
│   │   ├── ThreadSafeQueue.h
│   │   ├── BresenhamLine.h
│   │   ├── BresenhamLine.cpp
│   │   └── Point.h
│   └── renderer/
│       ├── GridRenderer.h
│       └── GridRenderer.cpp
├── tests/
│   ├── test_grid.cpp
│   ├── test_commands.cpp
│   ├── test_parser.cpp
│   └── test_bresenham.cpp
└── examples/
    ├── simple.txt
    ├── large_grid.txt
    └── complex_drawing.txt
```

## Threading Model

### Strategy: Producer-Consumer Pattern

1. **Producer Thread (Parser)**
   - Reads commands from file line-by-line
   - Validates and creates Command objects
   - Pushes to thread-safe queue
   - Signals completion when file ends

2. **Consumer Thread (Executor)**
   - Waits for DIMENSION command first
   - Initializes grid when DIMENSION received
   - Executes commands as they arrive
   - Updates grid in thread-safe manner

3. **Main Thread**
   - Launches parser and executor threads
   - Waits for completion
   - Renders final grid

### Synchronization Points

1. **Grid Initialization**: Executor waits for DIMENSION
2. **Command Queue**: Condition variables for blocking
3. **Grid Updates**: Mutex protection for concurrent writes
4. **Shutdown**: Graceful termination signaling

## Performance Optimizations

### For Large Grids (N = 10,000+)
1. **Memory Efficiency**
   - Only store marked cells in sparse representation
   - Switch from `vector<vector<char>>` to `unordered_set<Point>`
   - Lazy allocation of grid cells

2. **Rendering Optimization**
   - Only render occupied bounding box
   - Skip empty rows/columns

### For Large Command Files (Millions of commands)
1. **Streaming Processing**
   - Never load entire file into memory
   - Process line-by-line with fixed buffer
   
2. **Concurrent Processing**
   - Parser runs while executor draws
   - Overlap I/O with computation

3. **Command Batching**
   - Group multiple LINE_TO commands
   - Execute in batches to reduce locking overhead

## Extensibility

### Adding New Commands

1. Create new command class implementing `ICommand`
2. Add parser case in `CommandParser::parse()`
3. No changes needed to core Grid or Executor

Example:
```cpp
class CircleToCommand : public ICommand {
public:
    CircleToCommand(int cx, int cy, int radius);
    void execute(Grid& grid, RobotState& robot) override;
    bool validate() const override;
};
```

### Future Extensions

1. **Image Export**: Add `ImageRenderer` alongside `GridRenderer`
2. **Color Support**: Extend cell data from `char` to `Color` struct
3. **Undo/Redo**: Store command history
4. **Real-time Visualization**: Add observer for live updates
5. **Distributed Processing**: Partition grid for multi-machine execution

## Error Handling

### Validation Layers

1. **Parse-time**: Invalid syntax, malformed commands
2. **Pre-execution**: Coordinates out of bounds, DIMENSION validation
3. **Runtime**: Grid not initialized, invalid state

### Error Recovery

- Continue processing on non-fatal errors
- Collect all errors and report at end
- Line number tracking for debugging

## Memory Safety

1. **No Raw Pointers**: Use `unique_ptr` for command ownership
2. **RAII**: All resources in classes with destructors
3. **No Manual Memory**: STL containers handle allocation
4. **Thread Safety**: Mutexes prevent data races

## Build System

### CMake Structure
- Modern CMake 3.15+
- Separate targets for main app and tests
- Optional test building
- Cross-platform support (Linux, Windows, macOS)

## Testing Strategy

1. **Unit Tests**: Individual components (Grid, Commands, Bresenham)
2. **Integration Tests**: Parser + Executor pipeline
3. **Performance Tests**: Large grids and command files
4. **Thread Safety Tests**: Concurrent execution validation
