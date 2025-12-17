#!/bin/bash
# Build script for Linux/macOS using CMake + Ninja

set -e  # Exit on error

# Defaults
BUILD_TYPE="Release"
CLEAN_BUILD=false
USE_NINJA=true

while [[ $# -gt 0 ]]; do
    case $1 in
        debug) BUILD_TYPE="Debug"; shift ;;
        release) BUILD_TYPE="Release"; shift ;;
        clean) CLEAN_BUILD=true; shift ;;
        make) USE_NINJA=false; shift ;;
        help)
            echo "Usage: $0 [debug] [clean] [make]"
            exit 0
            ;;
        *) echo "Unknown option: $1"; exit 1 ;;
    esac
done

# Fallback if ninja not available
if $USE_NINJA && ! command -v ninja &> /dev/null; then
    echo "Ninja not found, falling back to Make."
    echo "To install Ninja on Ubuntu/Debian: sudo apt update && sudo apt install ninja-build"
    echo "To install Ninja on Fedora/RHEL: sudo dnf install ninja-build"
    echo "To install Ninja on macOS: brew install ninja"
    USE_NINJA=false
fi

BUILD_DIR="build"
if [ "$CLEAN_BUILD" = true ]; then
    rm -rf "$BUILD_DIR"
fi

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

echo "Configuring (BuildType=${BUILD_TYPE})..."
if $USE_NINJA; then
    cmake -G Ninja -DCMAKE_BUILD_TYPE="$BUILD_TYPE" ..
else
    cmake -DCMAKE_BUILD_TYPE="$BUILD_TYPE" ..
fi

echo "Building..."
if $USE_NINJA; then
    ninja
else
    make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)
fi

echo "Build finished: $BUILD_DIR/robot (BuildType=$BUILD_TYPE)"
