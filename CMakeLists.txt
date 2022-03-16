cmake_minimum_required(VERSION 3.13)
project(tetris)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(tetris 
src/main.c
src/block.h
src/block.c
src/draw.h
src/draw.c
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
src/libdevice.h
src/libdevice.c
src/map.c
src/map.h
src/sdl_interface.c
src/sdl_interface.h
)

target_include_directories(tetris
PUBLIC D:\\PROGRA~2\\MINGW-~1\\mingw64\\x86_64-w64-mingw32\\include
)

add_executable(tetris_rv 
src/main.c
src/block.h
src/block.c
src/draw.h
src/draw.c
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
src/map.c
src/map.h
)

target_compile_definitions(tetris_rv PUBLIC RISCV)

TARGET_LINK_LIBRARIES(tetris SDL2)
