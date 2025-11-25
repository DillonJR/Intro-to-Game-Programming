@echo off
REM ==== EDIT THESE TWO LINES TO MATCH WHERE SDL2 IS INSTALLED ====
REM Common MSYS2 default:
REM   C:\msys64\mingw64\include\SDL2
REM   C:\msys64\mingw64\lib
REM Or your MinGW/SDL2 dev package paths.

set SDL2_INC=C:\msys64\mingw64\include\SDL2
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
