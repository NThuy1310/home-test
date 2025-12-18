#!/bin/bash
# Valgrind memory testing script for Robot application

set -e

# Detect if we're running from project root or test directory
if [ -f "build.sh" ]; then
    # Running from project root
    BUILD_DIR="build"
    BUILD_SCRIPT="./build.sh"
    DEFAULT_TEST_FILE="examples/simple.txt"
else
    # Running from test directory
    BUILD_DIR="../build"
    BUILD_SCRIPT="../build.sh"
    DEFAULT_TEST_FILE="../examples/simple.txt"
fi

TEST_FILES=("${@:-$DEFAULT_TEST_FILE}")

echo "=== Valgrind Memory Test ==="
echo "Test files: ${TEST_FILES[@]}"
echo ""

# Ensure build directory exists and binary is built
if [ ! -f "$BUILD_DIR/robot" ]; then
    echo "Error: Robot binary not found. Building in Debug mode..."
    $BUILD_SCRIPT clean debug
fi

# Create input file for automated testing
# The app runs in a loop, so we test multiple files and then exit
cat > "$BUILD_DIR/valgrind_input.txt" << EOF
${TEST_FILES[0]}
exit
EOF

# If multiple test files provided, add them before exit
if [ ${#TEST_FILES[@]} -gt 1 ]; then
    # Recreate with all files
    > "$BUILD_DIR/valgrind_input.txt"
    for file in "${TEST_FILES[@]}"; do
        echo "$file" >> "$BUILD_DIR/valgrind_input.txt"
    done
    echo "exit" >> "$BUILD_DIR/valgrind_input.txt"
fi

echo "Running Valgrind memory check..."
echo "This may take a moment..."
echo ""

cd "$BUILD_DIR"

# Run Valgrind with comprehensive memory checking
valgrind \
    --leak-check=full \
    --show-leak-kinds=all \
    --track-origins=yes \
    --verbose \
    --log-file=valgrind_report.txt \
    ./robot < valgrind_input.txt

echo ""
echo "=== Valgrind Report Summary ==="
echo ""

# Extract and display key information
if [ -f valgrind_report.txt ]; then
    echo "--- Memory Leak Summary ---"
    grep -A 20 "HEAP SUMMARY" valgrind_report.txt || echo "No heap summary found"
    echo ""
    
    echo "--- Leak Summary ---"
    grep -A 10 "LEAK SUMMARY" valgrind_report.txt || echo "No leak summary found"
    echo ""
    
    echo "--- Error Summary ---"
    grep "ERROR SUMMARY" valgrind_report.txt || echo "No error summary found"
    echo ""
    
    # Check for specific issues
    ERRORS=$(grep "definitely lost" valgrind_report.txt | grep -oP '\d+(?= bytes)' | head -1 || echo 0)
    POSSIBLE_LEAKS=$(grep "possibly lost" valgrind_report.txt | grep -oP '\d+(?= bytes)' | head -1 || echo 0)
    ERRORS=${ERRORS:-0}
    POSSIBLE_LEAKS=${POSSIBLE_LEAKS:-0}
    
    echo "Full report saved to: $BUILD_DIR/valgrind_report.txt"
    echo ""
    
    if [ "$ERRORS" -gt 0 ] || [ "$POSSIBLE_LEAKS" -gt 0 ]; then
        echo "=> Memory issues detected! Review the full report."
        exit 1
    else
        echo "=> No memory leaks detected!"
        exit 0
    fi
else
    echo "Error: Valgrind report not generated"
    exit 1
fi
