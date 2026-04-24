@echo off
:: ============================================================
::  Build Script for CC Asgn-2 — LL(1) Parser
::  Language: C++
::  Compiler: g++ (MinGW) — install via https://www.mingw-w64.org
::  Usage:    build.bat           <- compile
::            build.bat run       <- compile and run
::            build.bat clean     <- delete compiled output
:: ============================================================

set SRC_DIR=src
set OUT_DIR=output
set EXE=ll1parser.exe

set SOURCES=^
    %SRC_DIR%\main.cpp ^
    %SRC_DIR%\grammar.cpp ^
    %SRC_DIR%\left_factoring.cpp ^
    %SRC_DIR%\left_recursion.cpp ^
    %SRC_DIR%\first_follow.cpp ^
    %SRC_DIR%\parser.cpp ^
    %SRC_DIR%\tree.cpp

set FLAGS=-std=c++17 -Wall -I%SRC_DIR%

:: ---- clean ----
if "%1"=="clean" (
    echo Cleaning build...
    if exist %EXE% del /f %EXE%
    echo Done.
    goto end
)

:: ---- compile ----
echo Compiling...
g++ %FLAGS% %SOURCES% -o %EXE%

if %errorlevel% neq 0 (
    echo.
    echo [FAILED] Compilation failed. See errors above.
    goto end
)

echo [SUCCESS] Compiled successfully ^-^> %EXE%

:: ---- run ----
if "%1"=="run" (
    echo.
    echo Running %EXE%...
    echo ============================================================
    %EXE%
)

:end
