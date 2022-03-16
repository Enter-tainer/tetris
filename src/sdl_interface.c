#include "sdl_interface.h"
#include "block.h"
#include "map.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

// #include "logsys.h"

#define MAX_TEXT 100

SDL_Window* window;
SDL_Renderer* renderer;
#define BLOCK_SIZE 8

int get_color(int c) {
  switch (c) {
  case Block_I:
    return COLOR_CYAN;
  case Block_T:
    return COLOR_PURPLE;
  case Block_O:
    return COLOR_YELLOW;
  case Block_J:
    return COLOR_BLUE;
  case Block_L:
    return COLOR_ORANGE;
  case Block_S:
    return COLOR_GREEN;
  case Block_Z:
    return COLOR_RED;
  case Block_I_L:
    return COLOR_CYAN_LIGHT;
  case Block_T_L:
    return COLOR_PURPLE_LIGHT;
  case Block_O_L:
    return COLOR_YELLOW_LIGHT;
  case Block_J_L:
    return COLOR_BLUE_LIGHT;
  case Block_L_L:
    return COLOR_ORANGE_LIGHT;
  case Block_S_L:
    return COLOR_GREEN_LIGHT;
  case Block_Z_L:
    return COLOR_RED_LIGHT;
  case Block_I_Top:
    return COLOR_CYAN_LIGHT;
  case Block_T_Top:
    return COLOR_PURPLE_LIGHT;
  case Block_O_Top:
    return COLOR_YELLOW_LIGHT;
  case Block_J_Top:
    return COLOR_BLUE_LIGHT;
  case Block_L_Top:
    return COLOR_ORANGE_LIGHT;
  case Block_S_Top:
    return COLOR_GREEN_LIGHT;
  case Block_Z_Top:
    return COLOR_RED_LIGHT;
  case Block_BG:
    return COLOR_BACKGROUND;
  case Block_BG_DARK:
    return COLOR_BACKGROUND_DARK;
  case Block_SD:
    return COLOR_SHADOW;
  }
  return -1;
}

int get_block_type(enum BlockType b) {
  if (b >= Block_I && b <= Block_Z) {
    return Block_Block;
  }
  if (b >= Block_I_L && b <= Block_Z_L) {
    return Block_Block_Light;
  }
  if (b >= Block_I_Top && b <= Block_Z_Top) {
    return Block_Block_Top;
  }
}

void sdl_init() {
  if (SDL_Init(SDL_INIT_VIDEO) == -1) {
    // log_msgf(FATAL, "SDL_Init: %s\n", SDL_GetError());
  }
  if (SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer) == -1) {
    // log_msgf(FATAL, "SDL_CreateWindowAndRenderer: %s\n", SDL_GetError());
  }
  SDL_SetWindowTitle(window, "Tetris");
}

void sdl_set_color(unsigned int color) {
  SDL_SetRenderDrawColor(renderer, color >> 24, color >> 16, color >> 8, color);
}

void sdl_draw_rect(int x, int y, int w, int h) {
  SDL_Rect rect = {x, y, w, h};
  SDL_RenderDrawRect(renderer, &rect);
}

void sdl_fill_rect(int x, int y, int w, int h) {
  SDL_Rect rect = {x, y, w, h};
  SDL_RenderFillRect(renderer, &rect);
}

void sdl_draw_line(int x1, int y1, int x2, int y2) {
  SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void sdl_sync() {
  for (int i = 0; i < MAP_WIDTH; i++) {
    for (int j = 0; j < MAP_HEIGHT; j++) {
      int t = get_block_type(map[idx(i, j)]);
      int c = get_color(map[idx(i, j)]);
      if (c == COLOR_UNKNOW)
        continue;
      sdl_set_color(c);
      if (t == Block_Block_Top) {
        sdl_fill_rect(i * BLOCK_SIZE, j * BLOCK_SIZE + (BLOCK_SIZE >> 1),
                      BLOCK_SIZE, (BLOCK_SIZE >> 1));
        sdl_set_color(COLOR_BACKGROUND);
        sdl_fill_rect(i * BLOCK_SIZE, j * BLOCK_SIZE, BLOCK_SIZE,
                      (BLOCK_SIZE >> 1));
      }else{
        sdl_fill_rect(i * BLOCK_SIZE, j * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
	  }
    }
  }
  SDL_RenderPresent(renderer);
}