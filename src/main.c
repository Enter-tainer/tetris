#include "block.h"
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
#define STAGE_H 22
#define STAGE_TOP_MARGIN 4

#define BLOCK_SIZE 2
#define HALF_BLOCK_SIZE (BLOCK_SIZE >> 1)

#define STAGE_LEFT_MARGIN 15
#define STAGE_RIGHT_MARGIN 15

#define HOLD_LEFT_MARGIN 6
#define HOLD_TOP_MARGIN 13
#define HOLD_WIDTH 5
#define HOLD_HEIGHT 3

#define PREVIEW_LEFT_MARGIN (STAGE_LEFT_MARGIN + STAGE_W + 5)
#define PREVIEW_TOP_MARGIN 8
#define PREVIEW_WIDTH 5
#define PREVIEW_HEIGHT 13

#define SCREEN_W                                                               \
  ((STAGE_LEFT_MARGIN + STAGE_W + STAGE_RIGHT_MARGIN) * BLOCK_SIZE)
#define SCREEN_H (STAGE_H * BLOCK_SIZE)

#define MAX_PREVIEW 5

struct Field f;
enum CellType field[40][10];

int get_block(int c) {
  switch (c) {
  case IBlock:
    return Block_I;
  case TBlock:
    return Block_T;
  case OBlock:
    return Block_O;
  case JBlock:
    return Block_J;
  case LBlock:
    return Block_L;
  case SBlock:
    return Block_S;
  case ZBlock:
    return Block_Z;
  case Clean:
    return Block_BG;
  case Shadow:
    return Block_SD;
  case Garbage:
    return Block_SD;
  }
  return Block_BG;
}

int get_block_light(int c) {
  switch (c) {
  case IBlock:
    return Block_I_L;
  case TBlock:
    return Block_T_L;
  case OBlock:
    return Block_O_L;
  case JBlock:
    return Block_J_L;
  case LBlock:
    return Block_L_L;
  case SBlock:
    return Block_S_L;
  case ZBlock:
    return Block_Z_L;
  }
  return Block_BG;
}

int get_block_top(int c) {
  switch (c) {
  case IBlock:
    return Block_I_Top;
  case TBlock:
    return Block_T_Top;
  case OBlock:
    return Block_O_Top;
  case JBlock:
    return Block_J_Top;
  case LBlock:
    return Block_L_Top;
  case SBlock:
    return Block_S_Top;
  case ZBlock:
    return Block_Z_Top;
  }
  return Block_BG;
}

void draw_border() {
  // graphics_set_block(COLOR_GRAY_DARK);
  // graphics_draw_line(STAGE_LEFT_MARGIN * BLOCK_SIZE, SCREEN_H - 1,
  //                    (STAGE_LEFT_MARGIN + STAGE_W) * BLOCK_SIZE, SCREEN_H -
  //                    1);
  // graphics_draw_line((STAGE_LEFT_MARGIN + STAGE_W) * BLOCK_SIZE, 0,
  //                    (STAGE_LEFT_MARGIN + STAGE_W) * BLOCK_SIZE, SCREEN_H);
  // graphics_draw_rect((PREVIEW_LEFT_MARGIN - 1) * BLOCK_SIZE -
  // HALF_BLOCK_SIZE,
  //                    PREVIEW_TOP_MARGIN * BLOCK_SIZE - HALF_BLOCK_SIZE,
  //                    PREVIEW_WIDTH * BLOCK_SIZE, PREVIEW_HEIGHT *
  //                    BLOCK_SIZE);
  // graphics_draw_rect((HOLD_LEFT_MARGIN - 1) * BLOCK_SIZE - HALF_BLOCK_SIZE,
  //                    HOLD_TOP_MARGIN * BLOCK_SIZE - HALF_BLOCK_SIZE,
  //                    HOLD_WIDTH * BLOCK_SIZE, HOLD_HEIGHT * BLOCK_SIZE);
}

void draw_block(int x, int y) {
  graphics_fill_rect(x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
  // graphics_set_block(COLOR_GRAY_DARK);
  // graphics_draw_rect(x*BLOCK_SIZE, y*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
  // graphics_draw_line(x * BLOCK_SIZE, y * BLOCK_SIZE,
  //                    x * BLOCK_SIZE + BLOCK_SIZE, y * BLOCK_SIZE);
  // graphics_draw_line(x * BLOCK_SIZE, y * BLOCK_SIZE, x * BLOCK_SIZE,
  //                    y * BLOCK_SIZE + BLOCK_SIZE);
}

void draw_field() {
  get_field(&f, field);
  for (int i = 21; i >= 0; i--) {
    for (int j = 0; j <= 9; j++) {
      int x = (STAGE_LEFT_MARGIN + j);
      int y = (STAGE_H - (i + 1) + STAGE_TOP_MARGIN);
      if (field[i][j] == Clean && i > 0 &&
          get_block_top(field[i - 1][j]) != Block_BG) {
        graphics_set_block(get_block_top(field[i - 1][j]));
        graphics_fill_rect(x * BLOCK_SIZE, y * BLOCK_SIZE + HALF_BLOCK_SIZE,
                           BLOCK_SIZE, HALF_BLOCK_SIZE);
        graphics_set_block(Block_BG);
        graphics_fill_rect(x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE,
                           HALF_BLOCK_SIZE);
        continue;
      }
      graphics_set_block(get_block(field[i][j]));
      if (field[i][j] == Shadow) {
        graphics_set_block(get_block_light(f.current.type));
      }
      draw_block(x, y);
    }
  }
}

void draw_hold() {
  graphics_set_block(Block_BG_DARK);
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

  graphics_set_block(get_block(f.hold.some));

  for (int i = 0; i < 4; ++i) {
    cell_y[i] += y_offset;
    graphics_fill_rect(cell_x[i] * BLOCK_SIZE + x_offset_px,
                       cell_y[i] * BLOCK_SIZE + y_offset_px, BLOCK_SIZE,
                       BLOCK_SIZE);
  }
}

void draw_preview() {
  // clear the preview area
  graphics_set_block(Block_BG_DARK);
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
    int x_offset = 0;
    int x_offset_px;
    int y_offset_px = HALF_BLOCK_SIZE;
    preview.state   = CounterClockwise;

    // make mino align to the center
    if (preview.type == TMino || preview.type == ZMino ||
        preview.type == SMino || preview.type == LMino ||
        preview.type == JMino) {
      x_offset_px = HALF_BLOCK_SIZE;
    } else if (preview.type == OMino) {
      x_offset_px = BLOCK_SIZE;
    } else {
      x_offset_px = 0;
    }

    // IMino padding half a block both top and bottom
    if (preview.type == IMino) {
      cnt++;
    }

    int cell_x[4], cell_y[4];
    get_cells(&preview, cell_x, cell_y);

    // move the mino all behind the original x
    for (int j = 0; j < 4; ++j) {
      if (x - cell_x[j] > x_offset) {
        x_offset = x - cell_x[j];
      }
    }

    // move the mino all below the original y
    for (int j = 0; j < 4; ++j) {
      if (y - cell_y[j] > y_offset) {
        y_offset = y - cell_y[j];
      }
    }
    graphics_set_block(get_block(preview.type));
    for (int j = 0; j < 4; ++j) {
      cell_x[j] += x_offset;
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
  init_field(&f);
  while (true) {
    draw();
    if (input_update())
      return 0;
    field_update();
  }
  return 0;
}
