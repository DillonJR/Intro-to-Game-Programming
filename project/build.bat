@echo off

REM Point to the SDL2 that vcpkg built for MinGW in your course starter repo
set SDL2_INC=C:\Users\dillo\GitHub\CPSI-27703-Intro-to-Game-Programming-Fall-2025-Starter\build\win-mingw-debug\vcpkg_installed\x64-mingw-dynamic\include
set SDL2_LIB=C:\Users\dillo\GitHub\CPSI-27703-Intro-to-Game-Programming-Fall-2025-Starter\build\win-mingw-debug\vcpkg_installed\x64-mingw-dynamic\lib

echo Using SDL2 include: %SDL2_INC%
echo Using SDL2 lib:     %SDL2_LIB%
echo.

g++ -std=c++17 ^
  src/main.cpp ^
  src/world/World.cpp ^
  src/tinyxml2.cpp ^
  -Isrc -Isrc/world -Isrc/components -Isrc/factory ^
  -I"%SDL2_INC%" ^
  -L"%SDL2_LIB%" ^
  -lmingw32 -lSDL2main -lSDL2 ^
  -o game.exe

if errorlevel 1 (
    echo.
    echo Build FAILED.
) else (
    echo.
    echo Build succeeded. Run: .\game.exe
)

pause
