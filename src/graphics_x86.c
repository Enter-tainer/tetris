#include "graphics.h"
#include "sdl_interface.h"
#include "map.h"
#include "block.h"


unsigned char v_mem[MAP_SIZE];
enum BlockType block_type;

void graphics_flip() { sdl_sync(); }

void graphics_init(int x, int y) {
  sdl_init();
  map = (unsigned char*)v_mem;
  for (int i = 0; i < MAP_WIDTH; i++) {
    for (int j = 0; j < MAP_HEIGHT; j++) {
      map[idx(i,j)] = Block_BG_DARK;
    }
  }
  graphics_flip();
}

void graphics_set_block(unsigned char color) { block_type = color; }

void graphics_draw_rect(int x, int y, int w, int h) {
  for (int i = 0; i < w; i++)
    map[idx(x + i,y)] = Block_BG_T;
  for (int i = 0; i < w; i++)
    map[idx(x + i,y + h - 1)] = Block_BG_B;
  for (int i = 0; i < h; i++)
    map[idx(x,y + i)] = Block_BG_L;
  for (int i = 0; i < h; i++)
    map[idx(x + w - 1,y + i)] = Block_BG_R;
}

void graphics_draw_line(int x1, int y1, int x2, int y2) {
}

void graphics_fill_rect(int x, int y, int w, int h) {
	for(int i=x; i<x+w; i++) {
		for(int j=y; j<y+h; j++) {
			map[idx(i,j)] = block_type;
		}
	}
}