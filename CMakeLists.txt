cmake_minimum_required(VERSION 3.15)

project(RobotMovement VERSION 1.0.0 
        DESCRIPTION "Robot Movement and Drawing System"
        LANGUAGES CXX)

# Use Ninja in advance
if(NOT CMAKE_GENERATOR MATCHES "Ninja")
    message(WARNING "Consider using Ninja for faster builds: cmake -G Ninja ..")
endif()

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

# Compiler warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Collect sources (use GLOB_RECURSE so CMake picks up files in subdirs)
file(GLOB_RECURSE ROBOT_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.cpp"
)

if(NOT ROBOT_SOURCES)
    message(FATAL_ERROR "No source files found in ${CMAKE_CURRENT_SOURCE_DIR}/src")
endif()

# Find libraries
find_package(Threads REQUIRED)

# Main executable
add_executable(robot ${ROBOT_SOURCES})

# Include directories and link threading library
target_include_directories(robot PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(robot PRIVATE Threads::Threads)

# Minimal platform-specific settings
if(APPLE)
    set_target_properties(robot PROPERTIES MACOSX_RPATH ON)
elseif(UNIX)
    set_target_properties(robot PROPERTIES BUILD_RPATH_USE_ORIGIN ON)
endif()

# Installation
install(TARGETS robot
    RUNTIME DESTINATION bin
)

# Copy example files to build directory
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/examples/)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/examples/ 
        DESTINATION ${CMAKE_BINARY_DIR}/examples/
    )
endif()

message(STATUS "RobotMovement ${PROJECT_VERSION} | BuildType=${CMAKE_BUILD_TYPE}")
