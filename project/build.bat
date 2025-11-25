@echo off
setlocal enabledelayedexpansion

echo Building...

REM --- SDL2 PATHS (your working vcpkg paths) ---
set SDL_INCLUDE=C:\Users\dillo\GitHub\CPSI-27703-Intro-to-Game-Programming-Fall-2025-Starter\build\win-mingw-debug\vcpkg_installed\x64-mingw-dynamic\include
set SDL_LIB=C:\Users\dillo\GitHub\CPSI-27703-Intro-to-Game-Programming-Fall-2025-Starter\build\win-mingw-debug\vcpkg_installed\x64-mingw-dynamic\lib

echo Using SDL2 include: %SDL_INCLUDE%
echo Using SDL2 lib:     %SDL_LIB%
echo.

REM --- Gather all source files ---
set SRC_FILES=
for %%f in (src\*.cpp) do (
    set SRC_FILES=!SRC_FILES! %%f
)

set OUT_EXE=game.exe

REM --- Compile ---
g++ -std=c++17 -Wall -Wextra -I"%SDL_INCLUDE%" ^
    %SRC_FILES% ^
    -L"%SDL_LIB%" ^
    -lmingw32 ^
    -lSDL2main ^
    -lSDL2 ^
    -static-libstdc++ -static-libgcc ^
    -o %OUT_EXE%

if %errorlevel% neq 0 (
    echo.
    echo Build FAILED.
    pause
    exit /b
)

echo.
echo Build SUCCESS!
echo Running %OUT_EXE%...
echo.

%OUT_EXE%

pause
