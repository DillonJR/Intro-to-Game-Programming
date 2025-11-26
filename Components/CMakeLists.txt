cmake_minimum_required(VERSION 3.16)
project(GameEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SDL2
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)

# Add executable
add_executable(${PROJECT_NAME}
    main.cpp
    Engine.cpp
    Input.cpp
    Textures.cpp
)

# SDL2 defines main as SDL_main
target_compile_definitions(${PROJECT_NAME} PRIVATE main=SDL_main)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
)

# Copy assets
configure_file(${CMAKE_SOURCE_DIR}/squirrel.png ${CMAKE_BINARY_DIR}/squirrel.png COPYONLY)
