cmake_minimum_required(VERSION 3.15)

project(RobotMovement 
    VERSION 1.0.0
    DESCRIPTION "Robot Movement and Drawing System"
    LANGUAGES CXX
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler warnings
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Threading support
find_package(Threads REQUIRED)

# Source files
set(MODEL_SOURCES
    src/model/Grid.cpp
)

set(COMMAND_SOURCES
    src/commands/DimensionCommand.cpp
    src/commands/MoveToCommand.cpp
    src/commands/LineToCommand.cpp
)

set(PARSER_SOURCES
    src/parser/CommandParser.cpp
)

set(EXECUTOR_SOURCES
    src/executor/CommandExecutor.cpp
)

set(UTILS_SOURCES
    src/utils/BresenhamLine.cpp
)

set(RENDERER_SOURCES
    src/renderer/GridRenderer.cpp
)

set(CONTROLLER_SOURCES
    src/controller/Controller.cpp
)

# Main executable
add_executable(robot
    src/main.cpp
    ${MODEL_SOURCES}
    ${COMMAND_SOURCES}
    ${PARSER_SOURCES}
    ${EXECUTOR_SOURCES}
    ${UTILS_SOURCES}
    ${RENDERER_SOURCES}
    ${CONTROLLER_SOURCES}
)

# Include directories
target_include_directories(robot PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link threading library
target_link_libraries(robot PRIVATE Threads::Threads)

# Installation
install(TARGETS robot
    RUNTIME DESTINATION bin
)

# Copy example files to build directory (if they exist)
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/examples/)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/examples/ 
         DESTINATION ${CMAKE_BINARY_DIR}/examples/
    )
endif()

# Print build information
message(STATUS "Robot Movement System v${PROJECT_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
