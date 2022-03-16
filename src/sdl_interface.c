#include "sdl_interface.h"
#include "block.h"
#include "map.h"
#include <SDL2/SDL.h>
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
  }
  if (SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer) == -1) {
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
      } else {
        sdl_fill_rect(i * BLOCK_SIZE, j * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
      }
    }
  }
  SDL_RenderPresent(renderer);
}

#define K_LEFT 0x6b
#define K_RIGHT_0 0xe0
#define K_RIGHT_1 0x74
#define K_UP 0x75
#define K_DOWN 0x72
#define K_Z 0x1a
#define K_X 0x22
#define K_C 0x21
#define K_SPACE 0x29
#define BUF_SIZE 32
#define BUF_MASK (BUF_SIZE - 1)

unsigned char sdl_wait_key(unsigned char* queue, unsigned char* head,
                           unsigned char* tail) {
  if (((*tail + 1) & BUF_MASK) == *head) {
    return 0;
  }
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type != SDL_KEYDOWN && event.type != SDL_KEYUP) {
      continue;
    }

    if (event.key.keysym.sym == SDLK_RIGHT) {
      queue[(*tail)] = K_RIGHT_0;
      *tail          = (*tail + 1) & BUF_MASK;
      if (((*tail + 1) & BUF_MASK) == *head)
        return 0;
    }

    if (event.type == SDL_KEYUP) {
      queue[(*tail)] = 0xf0;
      *tail          = (*tail + 1) & BUF_MASK;
      if (((*tail + 1) & BUF_MASK) == *head)
        return 0;
    }

    switch (event.key.keysym.sym) {
    case SDLK_LEFT:
      queue[(*tail)] = K_LEFT;
      break;
    case SDLK_RIGHT:
      queue[(*tail)] = K_RIGHT_1;
      break;
    case SDLK_UP:
      queue[(*tail)] = K_UP;
      break;
    case SDLK_DOWN:
      queue[(*tail)] = K_DOWN;
      break;
    case SDLK_z:
      queue[(*tail)] = K_Z;
      break;
    case SDLK_x:
      queue[(*tail)] = K_X;
      break;
    case SDLK_c:
      queue[(*tail)] = K_C;
      break;
    case SDLK_SPACE:
      queue[(*tail)] = K_SPACE;
      break;
    default:
      continue;
      break;
    }
    *tail = (*tail + 1) & BUF_MASK;
  }
}