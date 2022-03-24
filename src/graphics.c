#include "graphics.h"
#include "block.h"
#include "libdevice.h"

#ifndef RISCV
#include "map.h"
#include "sdl_interface.h"
#endif

#ifndef RISCV
unsigned char v_mem[MAP_SIZE];
#endif

void graphics_flip() {
#ifndef RISCV
  sdl_sync();
#else
  commit_vram();
#endif
}

void graphics_init(int x, int y) {
#ifndef RISCV
  sdl_init();
  map = (unsigned char*)v_mem;
#endif
  for (int i = 0; i < VRAM_Y; i++) {
    for (int j = 0; j < VRAM_X; j++) {
#ifdef RISCV
      set_vram(j, i, BlockBackgroundDark);
#else
      map[idx(i, j)] = BlockBackgroundDark;
#endif
    }
  }
  graphics_flip();
#ifdef RISCV
  for (int i = 0; i < VRAM_Y; i++) {
    for (int j = 0; j < VRAM_X; j++) {
      set_vram(j, i, BlockBackgroundDark);
    }
  }
  graphics_flip();
#endif
}

void graphics_draw_rect(int x, int y, int w, int h, unsigned char block_type) {
  for (int i = 0; i < w; i++) {
#ifdef RISCV
    set_vram(y, x + i, BlockBackgroundTop);
#else
    map[idx(x + i, y)]         = BlockBackgroundTop;
#endif
  }

  for (int i = 0; i < w; i++) {
#ifdef RISCV
    set_vram(y + h - 1, x + i, BlockBackgroundBottom);
#else
    map[idx(x + i, y + h - 1)] = BlockBackgroundBottom;
#endif
  }

  for (int i = 0; i < h; i++) {
#ifdef RISCV
    set_vram(y + i, x, BlockBackgroundLeft);
#else
    map[idx(x, y + i)]         = BlockBackgroundLeft;
#endif
  }

  for (int i = 0; i < h; i++) {
#ifdef RISCV
    set_vram(y + i, x + w - 1, BlockBackgroundRight);
#else
    map[idx(x + w - 1, y + i)] = BlockBackgroundRight;
#endif
  }
}

void graphics_draw_line(int x1, int y1, int x2, int y2,
                        unsigned char block_type) {}

void graphics_fill_rect(int x, int y, int w, int h, unsigned char block_type) {
  for (int i = x; i < x + w; i++) {
    for (int j = y; j < y + h; j++) {
#ifdef RISCV
      set_vram(j, i, block_type);
#else
      map[idx(i, j)] = block_type;
#endif
    }
  }
}
