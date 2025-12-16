# Build Instructions

## Quick Start

```bash
# Navigate to project directory
cd home-test

# Create and enter build directory
mkdir -p build && cd build

# Configure with CMake (Release mode for best performance)
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build the project
cmake --build . -j$(nproc)

# Run with example
./robot ../examples/simple.txt
```

## Build Modes

### Release Build (Recommended)
```bash
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```
- Optimizations enabled (-O3)
- No debug symbols
- Best performance
- Use for production and benchmarking

### Debug Build
```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build .
```
- No optimizations (-O0)
- Debug symbols included (-g)
- Better for debugging with GDB/LLDB
- Easier to track issues

## Platform-Specific Notes

### Linux (Ubuntu/Debian)
```bash
# Install build tools if needed
sudo apt update
sudo apt install build-essential cmake

# Build
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

### macOS
```bash
# Install Xcode command line tools if needed
xcode-select --install

# Or install CMake via Homebrew
brew install cmake

# Build
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(sysctl -n hw.ncpu)
```

### Windows (Visual Studio)
```bash
# Using Visual Studio Developer Command Prompt
mkdir build && cd build
cmake .. -G "Visual Studio 16 2019"
cmake --build . --config Release
```

Or open CMakeLists.txt directly in Visual Studio 2019+ which has native CMake support.

## Cleaning Build

```bash
# Remove build directory and rebuild from scratch
cd home-test
rm -rf build
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

## Installation (Optional)

```bash
# Install to system (default: /usr/local/bin)
sudo cmake --install .

# Now you can run from anywhere
robot ~/my_commands.txt

# Custom installation prefix
cmake .. -DCMAKE_INSTALL_PREFIX=/opt/robot
sudo cmake --install .
```

## Verification

After building, verify the executable works:

```bash
# Test with simple example
./robot ../examples/simple.txt

# Expected output:
# Parsing commands from: ../examples/simple.txt
# Parsed 4 commands
# Executing commands...
# 
# === Final Grid ===
# . . . . .
# . + . . .
# . . + + .
# . . . + .
# . . . . .
```

## Troubleshooting

### CMake version too old
```
Error: CMake 3.15 or higher is required
```
**Solution**: Update CMake
```bash
# Ubuntu/Debian
sudo apt install cmake

# Or download from https://cmake.org/download/
```

### Compiler not found
```
Error: No CMAKE_CXX_COMPILER could be found
```
**Solution**: Install a C++ compiler
```bash
# Ubuntu/Debian
sudo apt install g++

# macOS
xcode-select --install
```

### Threading library not found
```
Error: Could NOT find Threads
```
**Solution**: Install pthread library
```bash
# Usually included with build-essential
sudo apt install build-essential
```

### Build warnings as errors
If you see compilation errors due to warnings, you can temporarily disable -Werror:

Edit CMakeLists.txt and remove `-Werror` from compile options, or:
```bash
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-Wno-error"
```

## Performance Testing

To benchmark performance:

```bash
# Build in Release mode (critical for performance)
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .

# Test with large grid
time ./robot ../examples/large_grid.txt
```

## Advanced Options

### Verbose Build
```bash
cmake --build . --verbose
```

### Parallel Build with Specific Job Count
```bash
cmake --build . -j8  # Use 8 parallel jobs
```

### Build with Address Sanitizer (debugging memory issues)
```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug \
         -DCMAKE_CXX_FLAGS="-fsanitize=address -fno-omit-frame-pointer"
cmake --build .
```

### Build with Thread Sanitizer (debugging threading issues)
```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug \
         -DCMAKE_CXX_FLAGS="-fsanitize=thread"
cmake --build .
```

## IDE Integration

### Visual Studio Code
1. Install "C/C++" and "CMake Tools" extensions
2. Open project folder
3. CMake will auto-configure
4. Use bottom toolbar to build/run

### CLion
1. Open project folder
2. CLion automatically detects CMakeLists.txt
3. Use build/run buttons

### Visual Studio 2019+
1. File → Open → CMake
2. Select CMakeLists.txt
3. VS will configure automatically
4. Use Debug/Release dropdown to build
