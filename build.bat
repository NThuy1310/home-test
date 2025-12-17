@echo off
REM Build script for Windows using CMake + Ninja

setlocal enabledelayedexpansion

REM Defaults
set BUILD_TYPE=Release
set CLEAN_BUILD=0
set USE_NINJA=1

:parse_args
if "%~1"=="" goto end_parse
if /i "%~1"=="debug" (
    set BUILD_TYPE=Debug
    shift
    goto parse_args
)
if /i "%~1"=="clean" (
    set CLEAN_BUILD=1
    shift
    goto parse_args
)
if /i "%~1"=="msvc" (
    set USE_NINJA=0
    shift
    goto parse_args
)
if /i "%~1"=="help" (
    echo Usage: build.bat [debug] [clean] [msvc]
    exit /b 0
)
echo Unknown option: %~1
exit /b 1

:end_parse

if %CLEAN_BUILD%==1 (
    if exist "build" rmdir /s /q "build"
)

if not exist "build" mkdir "build"
cd "build"

if %USE_NINJA%==1 (
    where ninja >nul 2>nul
    if errorlevel 1 (
        echo Warning: Ninja not found in PATH
        echo To install Ninja on Windows:
        echo   - Using Chocolatey: choco install ninja
        echo   - Using Scoop: scoop install ninja
        echo   - Download from: https://github.com/ninja-build/ninja/releases
        echo Attempting to continue with CMake...
    )
)

echo Configuring (BuildType=%BUILD_TYPE%)...
if %USE_NINJA%==1 (
    cmake -G Ninja -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ..
) else (
    cmake -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ..
)

if errorlevel 1 (
    echo CMake configuration failed
    exit /b 1
)

echo Building...
if %USE_NINJA%==1 (
    ninja
) else (
    cmake --build . --config %BUILD_TYPE% --parallel
)

if errorlevel 1 (
    echo Build failed
    exit /b 1
)

echo Build finished: build\robot.exe (BuildType=%BUILD_TYPE%)
