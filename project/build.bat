@echo off
set SDL2_INC=C:\msys64\mingw64\include
set SDL2_LIB=C:\msys64\mingw64\lib

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
