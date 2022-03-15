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
#define STAGE_H 21
#define BLOCK_SIZE 16
#define HALF_BLOCK_SIZE 8

#define STAGE_LEFT_MARGIN 7
#define STAGE_RIGHT_MARGIN 8

#define HOLD_LEFT_MARGIN 2
#define HOLD_TOP_MARGIN 8
#define HOLD_WIDTH 5
#define HOLD_HEIGHT 3

#define PREVIEW_LEFT_MARGIN (STAGE_LEFT_MARGIN + STAGE_W + 3)
#define PREVIEW_TOP_MARGIN 4
#define PREVIEW_WIDTH 5
#define PREVIEW_HEIGHT 13

#define SCREEN_W                                                               \
  ((STAGE_LEFT_MARGIN + STAGE_W + STAGE_RIGHT_MARGIN) * BLOCK_SIZE)
#define SCREEN_H (STAGE_H * BLOCK_SIZE)

#define MAX_PREVIEW 5

struct Field f;
enum CellType field[40][10];

int get_color(int c) {
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
    return COLOR_BACKGROUND;
  case Shadow:
    return COLOR_SHADOW;
  case Garbage:
    return COLOR_SHADOW;
  }
  return COLOR_BACKGROUND;
}

void draw_border() {
  graphics_set_color(COLOR_GRAY_DARK);
  graphics_draw_line(STAGE_LEFT_MARGIN * BLOCK_SIZE, SCREEN_H - 1,
                     (STAGE_LEFT_MARGIN + STAGE_W) * BLOCK_SIZE, SCREEN_H - 1);
  graphics_draw_line((STAGE_LEFT_MARGIN + STAGE_W) * BLOCK_SIZE, 0,
                     (STAGE_LEFT_MARGIN + STAGE_W) * BLOCK_SIZE, SCREEN_H);
  graphics_draw_rect((PREVIEW_LEFT_MARGIN - 1) * BLOCK_SIZE - HALF_BLOCK_SIZE,
                     PREVIEW_TOP_MARGIN * BLOCK_SIZE - HALF_BLOCK_SIZE,
                     PREVIEW_WIDTH * BLOCK_SIZE, PREVIEW_HEIGHT * BLOCK_SIZE);
  graphics_draw_rect((HOLD_LEFT_MARGIN - 1) * BLOCK_SIZE - HALF_BLOCK_SIZE,
                     HOLD_TOP_MARGIN * BLOCK_SIZE - HALF_BLOCK_SIZE,
                     HOLD_WIDTH * BLOCK_SIZE, HOLD_HEIGHT * BLOCK_SIZE);
}

void draw_block(int x, int y) {
  graphics_fill_rect(x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
  graphics_set_color(COLOR_GRAY_DARK);
  // graphics_draw_rect(x*BLOCK_SIZE, y*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
  graphics_draw_line(x * BLOCK_SIZE, y * BLOCK_SIZE,
                     x * BLOCK_SIZE + BLOCK_SIZE, y * BLOCK_SIZE);
  graphics_draw_line(x * BLOCK_SIZE, y * BLOCK_SIZE, x * BLOCK_SIZE,
                     y * BLOCK_SIZE + BLOCK_SIZE);
}

void draw_field() {
  get_field(&f, field);
  for (int i = 20; i >= 0; i--) {
    for (int j = 0; j <= 9; j++) {
      graphics_set_color(get_color(field[i][j]));
      int x = (STAGE_LEFT_MARGIN + j);
      int y = (STAGE_H - (i + 1));
      draw_block(x, y);
    }
  }
}

void draw_hold() {
  graphics_set_color(COLOR_BACKGROUND);
  graphics_fill_rect(0, 0, STAGE_LEFT_MARGIN * BLOCK_SIZE, SCREEN_H);

  int x = HOLD_LEFT_MARGIN;
  int y = HOLD_TOP_MARGIN;
  int x_offset_px;
  int y_offset_px;
  int y_offset = 0;

  struct FallingMino hold;
  hold.type  = f.hold.some;
  hold.x     = x;
  hold.y     = y;
  hold.state = CounterClockwise;
  if (!f.hold.is_some) {
    return;
  }

  // make mino align to the center
  if (hold.type == TMino || hold.type == ZMino || hold.type == SMino ||
      hold.type == LMino || hold.type == JMino) {
    x_offset_px = HALF_BLOCK_SIZE;
  } else {
    x_offset_px = 0;
  }
  if (hold.type == IMino) {
    y_offset_px = HALF_BLOCK_SIZE;
  } else {
    y_offset_px = 0;
  }

  int cell_x[4], cell_y[4];
  get_cells(&hold, cell_x, cell_y);

  // move the mino all below the original y
  for (int j = 0; j < 4; ++j) {
    if (y - cell_y[j] > y_offset) {
      y_offset = y - cell_y[j];
    }
  }

  graphics_set_color(get_color(f.hold.some));

  for (int i = 0; i < 4; ++i) {
    cell_y[i] += y_offset;
    graphics_fill_rect(cell_x[i] * BLOCK_SIZE + x_offset_px,
                       cell_y[i] * BLOCK_SIZE + y_offset_px, BLOCK_SIZE,
                       BLOCK_SIZE);
  }
}

void draw_preview() {
  // clear the preview area
  graphics_set_color(COLOR_BACKGROUND);
  graphics_fill_rect((PREVIEW_LEFT_MARGIN - 1) * BLOCK_SIZE, 0,
                     PREVIEW_WIDTH * BLOCK_SIZE, SCREEN_H);

  int x      = PREVIEW_LEFT_MARGIN;
  int y      = PREVIEW_TOP_MARGIN;
  int next_y = y;
  struct FallingMino preview;
  int max_index = f.next.size - 1;
  if (MAX_PREVIEW - 1 < max_index) {
    max_index = MAX_PREVIEW - 1;
  }
  int cnt = 0;
  for (int i = 0; i <= max_index; i++) {
    preview.type = peek_queue(&f.next, i);
    preview.x    = x;
    preview.y    = y;
    int y_offset = 0;
    int x_offset_px;
    int y_offset_px = HALF_BLOCK_SIZE;
    preview.state   = CounterClockwise;

    // make mino align to the center
    if (preview.type == TMino || preview.type == ZMino ||
        preview.type == SMino || preview.type == LMino ||
        preview.type == JMino) {
      x_offset_px = HALF_BLOCK_SIZE;
    } else {
      x_offset_px = 0;
    }
    
    // IMino padding half a block both top and bottom
    if (preview.type == IMino) {
      cnt++;
    }

    int cell_x[4], cell_y[4];
    get_cells(&preview, cell_x, cell_y);

    // move the mino all below the original y
    for (int j = 0; j < 4; ++j) {
      if (y - cell_y[j] > y_offset) {
        y_offset = y - cell_y[j];
      }
    }
    graphics_set_color(get_color(preview.type));
    for (int j = 0; j < 4; ++j) {
      cell_y[j] += y_offset;
      graphics_fill_rect(cell_x[j] * BLOCK_SIZE + x_offset_px,
                         cell_y[j] * BLOCK_SIZE + y_offset_px * cnt, BLOCK_SIZE,
                         BLOCK_SIZE);

      // get next y
      if (cell_y[j] + 1 > next_y) {
        next_y = cell_y[j] + 1;
      }
    }

    // IMino padding half a block both top and bottom
    if (preview.type == IMino) {
      cnt++;
    }
    y = next_y;
    cnt++;
  }
}

void draw() {
  draw_field();
  draw_hold();
  draw_preview();
  draw_border();
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
  graphics_set_color(COLOR_BACKGROUND);
  graphics_fill_rect(0, 0, SCREEN_W, SCREEN_H);
  graphics_flip();
  init_field(&f);
  while (true) {
    draw();
    if (input_update())
      return 0;
    field_update();
  }
  return 0;
}
