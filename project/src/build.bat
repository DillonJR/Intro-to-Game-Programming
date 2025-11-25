g++ -std=c++17 -I src -I src/factory -I src/components -I src/world -o game.exe ^
  src/main.cpp ^
  src/world/World.cpp ^
  src/tinyxml2.cpp ^
  -lSDL2
