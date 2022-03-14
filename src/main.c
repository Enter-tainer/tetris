#include "graphics.h"
#include "input.h"
#include "tetris.h"
#include <stddef.h>
#include <stdio.h>
#include <time.h>
#ifdef __linux__
#include <unistd.h>
#endif

// Size of the stage
#define STAGE_W 10
#define STAGE_H 20
#define BLOCK_SIZE 16
#define SCREEN_W STAGE_W* BLOCK_SIZE
#define SCREEN_H STAGE_H* BLOCK_SIZE

struct Field f;
enum CellType field[40][10];

int get_color(enum CellType c) {
  switch (c) {
  case IBlock:
    return COLOR_CYAN;
  case TBlock:
    return COLOR_PURPLE;
  case OBlock:
    return COLOR_YELLOW;
  case JBlock:
    return COLOR_BLUE;
  case LBlock:
    return COLOR_ORANGE;
  case SBlock:
    return COLOR_GREEN;
  case ZBlock:
    return COLOR_RED;
  case Clean:
    return COLOR_BLACK;
  case Shadow:
    return COLOR_SHADOW;
  case Garbage:
    return COLOR_SHADOW;
  }
    return COLOR_BLACK;
}

void draw() {
  get_field(&f, field);
  for (int i = 19; i >= 0; i--) {
    for (int j = 0; j <= 9; j++) {
      graphics_set_color(get_color(field[i][j]));
      graphics_draw_rect(j * BLOCK_SIZE, (19 - i) * BLOCK_SIZE, BLOCK_SIZE,
                         BLOCK_SIZE);
    }
  }
  graphics_flip();
}

void field_update() {
  if (key.down) {
    drop_step(&f);
  }
  if (key.left) {
    move_left_step(&f);
  }
  if (key.right) {
    move_right_step(&f);
  }
  if (key.z) {
    rotate_counter_clockwise(&f);
  }
  if (key.x) {
    rotate_clockwise(&f);
  }
  if (key.c) {
    hold_mino(&f);
  }
  if (key.space) {
    while (drop_step(&f))
      ;
    lock_mino(&f);
    struct OptionMinoType tmp = {.is_some = false};
    bool res = spawn_mino(&f, tmp); // test failure here to indicate game over
    if (!res) {
      exit(1);
    }
  }
}
#ifdef __linux__
#define MAIN main
#else
#define MAIN WinMain
#endif
int MAIN(int argc, char* args[]) {
  graphics_init(SCREEN_W, SCREEN_H);
  graphics_set_color(COLOR_BLACK);
  graphics_draw_rect(0, 0, SCREEN_W, SCREEN_H);
  graphics_flip();
  init_field(&f);
  while (true) {
    draw();
    input_update();
    field_update();
  }
  return 0;
}
