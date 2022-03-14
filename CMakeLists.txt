cmake_minimum_required(VERSION 3.13)
project(tetris)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(tetris 
src/main.c
src/queue.c
src/random.c
src/random.h
src/tetris.h
src/graphics.h
src/graphics.c
src/input.c
src/input.h
src/board.c
src/falling_mino.c
)

TARGET_LINK_LIBRARIES(tetris SDL2 SDL2_ttf)