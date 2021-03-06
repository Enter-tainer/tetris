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
src/map.c
src/map.h
src/sdl_interface.c
src/sdl_interface.h
src/libdevice.h
src/handle.c
src/handle.h
src/garbage.c
src/garbage.h
src/communicate.h
src/communicate.c
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
src/libdevice.h
src/libdevice.c
src/map.c
src/map.h
src/handle.c
src/handle.h
src/garbage.c
src/garbage.h
src/communicate.h
src/arith64.c
)

target_compile_options(tetris PUBLIC -Wall -Wshadow -fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined)
target_link_options(tetris PUBLIC -fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined)
target_compile_definitions(tetris_rv PUBLIC RISCV)
set(LINKER_SCRIPT "rv_linker.lds")

target_compile_options(tetris_rv PUBLIC --target=riscv32-unknown-unknown -march=rv32im -ffreestanding -fno-builtin -nostdlib  -mno-relax -fno-PIE -G=0 -Oz -flto)
target_link_options(tetris_rv PUBLIC -T ${CMAKE_SOURCE_DIR}/${LINKER_SCRIPT} --target=riscv32-unknown-unknown -march=rv32im -ffreestanding -fno-builtin -nostdlib  -mno-relax -fno-PIE -G=0 -Oz -flto)
add_custom_command(TARGET tetris_rv POST_BUILD
    COMMAND llvm-objcopy ./tetris_rv --dump-section .text=rom.bin --dump-section .data=ram.bin
    COMMAND od -w4 -An --endian little -v -t x4 ./rom.bin > rom.mem
    COMMAND od -w4 -An --endian little -v -t x4 ./ram.bin > ram.mem
    COMMAND llvm-objdump -S tetris_rv > tetris.asm
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating rom and ram files..."
)
TARGET_LINK_LIBRARIES(tetris SDL2)
