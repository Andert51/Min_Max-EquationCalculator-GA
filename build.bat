@echo off
REM Build script for GA_Cpp project using g++ (obligatory flags)
REM Genetic Algorithm Demonstration - Andres Torres Ceja - UG DICIS

echo ===============================================
echo Building Genetic Algorithm Demonstration
echo Author: Andres Torres Ceja - UG DICIS
echo ===============================================

REM Create bin directory if it doesn't exist
if not exist "bin" mkdir bin

REM Clean previous build
if exist "bin\GA_Demo.exe" del "bin\GA_Demo.exe"

echo.
echo Compiling source files...

REM Compile all source files with required flags
g++ -static-libgcc -static-libstdc++ -static -std=c++17 -Wall -Wextra -m64 -O2 -DUNICODE -D_UNICODE -I"src" -o "bin\GA_Demo.exe" "src\main.cpp" "src\Individual.cpp" "src\FitnessFunction.cpp" "src\GeneticAlgorithm.cpp" "src\ConsoleInterface.cpp"

REM Check if compilation was successful
if %ERRORLEVEL% EQU 0 (
    echo.
    echo ===============================================
    echo BUILD SUCCESSFUL!
    echo Executable created: bin\GA_Demo.exe
    echo ===============================================
    echo.
    echo To run the application, use: bin\GA_Demo.exe
    echo Or simply run: .\bin\GA_Demo.exe
    echo.
) else (
    echo.
    echo ===============================================
    echo BUILD FAILED!
    echo Please check the error messages above.
    echo ===============================================
    echo.
) 