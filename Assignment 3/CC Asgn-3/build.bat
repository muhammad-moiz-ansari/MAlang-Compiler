@echo off
:: ============================================================
::  build.bat  -  Assignment 03 Bottom-Up Parser
::  Works with MinGW g++ on Windows
::
::  Usage:
::    build.bat           -> compile only
::    build.bat run       -> compile + run
::    build.bat clean     -> delete build artifacts
:: ============================================================

SET CXX=g++
SET FLAGS=-std=c++17 -Wall -g
SET TARGET=parser.exe

SET SRCS=src/main.cpp src/grammar.cpp src/grammar_augment.cpp src/first_follow.cpp src/items.cpp src/parsing_table.cpp src/slr_parser.cpp src/parser.cpp src/tree.cpp

:: ── Clean ───────────────────────────────────────────────────
IF "%1"=="clean" (
    echo [CLEAN] Removing build artifacts...
    IF EXIST %TARGET% del /Q %TARGET%
    IF EXIST src\*.o del /Q src\*.o
    echo [CLEAN] Done.
    goto :EOF
)

:: ── Create output folder if missing ─────────────────────────
IF NOT EXIST output mkdir output

:: ── Compile ─────────────────────────────────────────────────
echo [BUILD] Compiling...
%CXX% %FLAGS% -o %TARGET% %SRCS%

IF ERRORLEVEL 1 (
    echo.
    echo [FAILED] Build failed. Check errors above.
    exit /b 1
)

echo.
echo [SUCCESS] Build complete: %TARGET%
echo.

:: ── Run ─────────────────────────────────────────────────────
IF "%1"=="run" (
    echo [RUN] Running %TARGET%...
    echo ============================================================
    echo.
    %TARGET%
)
