#include "draw.h"
#include "block.h"
#include "graphics.h"
#include "tetris.h"
#include <stdint.h>

void draw_block(int x, int y, unsigned char block_type) {
  graphics_fill_rect(x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE,
                     block_type);
}

void draw_field(struct Field* f) {
  enum CellType field[40][10];
  get_field(f, field);
  for (int i = 21; i >= 0; i--) {
    for (int j = 0; j <= 9; j++) {
      int x = (STAGE_LEFT_MARGIN + j);
      int y = (STAGE_H - (i + 1) + STAGE_TOP_MARGIN);
      if (field[i][j] == Clean && i > 0 &&
          get_block_top(field[i - 1][j]) != Block_BG) {
        graphics_fill_rect(x * BLOCK_SIZE, y * BLOCK_SIZE + HALF_BLOCK_SIZE,
                           BLOCK_SIZE, HALF_BLOCK_SIZE,
                           get_block_top(field[i - 1][j]));
        graphics_fill_rect(x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE,
                           HALF_BLOCK_SIZE, Block_BG);
        continue;
      }
      unsigned char block_type = get_block(field[i][j]);
      if (field[i][j] == Shadow) {
        block_type = get_block_light(f->current.type);
      }
      draw_block(x, y, block_type);
    }
  }
}

void draw_hold(struct Field* f) {
  graphics_fill_rect(0, 0, STAGE_LEFT_MARGIN * BLOCK_SIZE, SCREEN_H,
                     Block_BG_DARK);

  int x = HOLD_LEFT_MARGIN;
  int y = HOLD_TOP_MARGIN;
  int x_offset_px;
  int y_offset_px;
  int y_offset = 0;

  struct FallingMino hold;
  hold.type  = get_hold(f).some;
  hold.x     = x;
  hold.y     = y;
  hold.state = CounterClockwise;
  if (!get_hold(f).is_some) {
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

  unsigned char block_type = get_block(get_hold(f).some);

  for (int i = 0; i < 4; ++i) {
    cell_y[i] += y_offset;
    graphics_fill_rect(cell_x[i] * BLOCK_SIZE + x_offset_px,
                       cell_y[i] * BLOCK_SIZE + y_offset_px, BLOCK_SIZE,
                       BLOCK_SIZE, block_type);
  }
}

void draw_preview(struct Field* f) {
  // clear the preview area
  graphics_fill_rect((PREVIEW_LEFT_MARGIN - 1) * BLOCK_SIZE, 0,
                     PREVIEW_WIDTH * BLOCK_SIZE, SCREEN_H, Block_BG_DARK);

  int x      = PREVIEW_LEFT_MARGIN;
  int y      = PREVIEW_TOP_MARGIN;
  int next_y = y;
  struct FallingMino preview;
  int max_index = f->next.size - 1;
  if (MAX_PREVIEW - 1 < max_index) {
    max_index = MAX_PREVIEW - 1;
  }
  int cnt = 0;
  for (int i = 0; i <= max_index; i++) {
    preview.type = peek_queue(&f->next, i);
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
    unsigned char block_type = get_block(preview.type);
    for (int j = 0; j < 4; ++j) {
      cell_x[j] += x_offset;
      cell_y[j] += y_offset;
      graphics_fill_rect(cell_x[j] * BLOCK_SIZE + x_offset_px,
                         cell_y[j] * BLOCK_SIZE + y_offset_px * cnt, BLOCK_SIZE,
                         BLOCK_SIZE, block_type);

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

void draw(struct Field* f, uint32_t garbage_cnt, uint32_t apm) {
  draw_field(f);
  draw_hold(f);
  draw_preview(f);
  draw_garbage_indicator(garbage_cnt);
  draw_apm(apm);
  graphics_flip();
}

void draw_start_view() {
  graphics_fill_rect(STAGE_LEFT_MARGIN * BLOCK_SIZE,
                     STAGE_TOP_MARGIN * BLOCK_SIZE, STAGE_W * BLOCK_SIZE,
                     STAGE_H * BLOCK_SIZE, Block_BG);

  // draw character "G"
  graphics_fill_rect(STAGE_LEFT_MARGIN * BLOCK_SIZE,
                     (STAGE_TOP_MARGIN + 7) * BLOCK_SIZE, 4 * BLOCK_SIZE,
                     6 * BLOCK_SIZE, Block_S);
  graphics_fill_rect((STAGE_LEFT_MARGIN + 1) * BLOCK_SIZE,
                     (STAGE_TOP_MARGIN + 8) * BLOCK_SIZE, 2 * BLOCK_SIZE,
                     4 * BLOCK_SIZE, Block_BG);
  graphics_fill_rect((STAGE_LEFT_MARGIN + 3) * BLOCK_SIZE,
                     (STAGE_TOP_MARGIN + 8) * BLOCK_SIZE, 1 * BLOCK_SIZE,
                     2 * BLOCK_SIZE, Block_BG);
  graphics_fill_rect((STAGE_LEFT_MARGIN + 2) * BLOCK_SIZE,
                     (STAGE_TOP_MARGIN + 10) * BLOCK_SIZE, 1 * BLOCK_SIZE,
                     1 * BLOCK_SIZE, Block_S);

  // draw character "O"
  graphics_fill_rect((STAGE_LEFT_MARGIN + 6) * BLOCK_SIZE,
                     (STAGE_TOP_MARGIN + 7) * BLOCK_SIZE, 4 * BLOCK_SIZE,
                     6 * BLOCK_SIZE, Block_S);
  graphics_fill_rect((STAGE_LEFT_MARGIN + 7) * BLOCK_SIZE,
                     (STAGE_TOP_MARGIN + 8) * BLOCK_SIZE, 2 * BLOCK_SIZE,
                     4 * BLOCK_SIZE, Block_BG);
  graphics_flip();
}

void draw_end_view() {
  graphics_fill_rect(STAGE_LEFT_MARGIN * BLOCK_SIZE,
                     STAGE_TOP_MARGIN * BLOCK_SIZE, STAGE_W * BLOCK_SIZE,
                     STAGE_H * BLOCK_SIZE, Block_BG);

  // draw character "G"
  graphics_fill_rect(STAGE_LEFT_MARGIN * BLOCK_SIZE,
                     (STAGE_TOP_MARGIN + 7) * BLOCK_SIZE, 4 * BLOCK_SIZE,
                     6 * BLOCK_SIZE, Block_Z);
  graphics_fill_rect((STAGE_LEFT_MARGIN + 1) * BLOCK_SIZE,
                     (STAGE_TOP_MARGIN + 8) * BLOCK_SIZE, 2 * BLOCK_SIZE,
                     4 * BLOCK_SIZE, Block_BG);
  graphics_fill_rect((STAGE_LEFT_MARGIN + 3) * BLOCK_SIZE,
                     (STAGE_TOP_MARGIN + 8) * BLOCK_SIZE, 1 * BLOCK_SIZE,
                     2 * BLOCK_SIZE, Block_BG);
  graphics_fill_rect((STAGE_LEFT_MARGIN + 2) * BLOCK_SIZE,
                     (STAGE_TOP_MARGIN + 10) * BLOCK_SIZE, 1 * BLOCK_SIZE,
                     1 * BLOCK_SIZE, Block_Z);

  // draw character "G"
  graphics_fill_rect((STAGE_LEFT_MARGIN + 6) * BLOCK_SIZE,
                     (STAGE_TOP_MARGIN + 7) * BLOCK_SIZE, 4 * BLOCK_SIZE,
                     6 * BLOCK_SIZE, Block_Z);
  graphics_fill_rect((STAGE_LEFT_MARGIN + 7) * BLOCK_SIZE,
                     (STAGE_TOP_MARGIN + 8) * BLOCK_SIZE, 2 * BLOCK_SIZE,
                     4 * BLOCK_SIZE, Block_BG);
  graphics_fill_rect((STAGE_LEFT_MARGIN + 9) * BLOCK_SIZE,
                     (STAGE_TOP_MARGIN + 8) * BLOCK_SIZE, 1 * BLOCK_SIZE,
                     2 * BLOCK_SIZE, Block_BG);
  graphics_fill_rect((STAGE_LEFT_MARGIN + 8) * BLOCK_SIZE,
                     (STAGE_TOP_MARGIN + 10) * BLOCK_SIZE, 1 * BLOCK_SIZE,
                     1 * BLOCK_SIZE, Block_Z);
  graphics_flip();
}

void draw_number(int x, int y, uint8_t num, enum BlockType b) {
  num %= 10;
  graphics_fill_rect(x * HALF_BLOCK_SIZE, y * HALF_BLOCK_SIZE,
                     3 * HALF_BLOCK_SIZE, 5 * HALF_BLOCK_SIZE, b);
  switch (num) {
  case 0:
    graphics_fill_rect((x + 1) * HALF_BLOCK_SIZE, (y + 1) * HALF_BLOCK_SIZE,
                       1 * HALF_BLOCK_SIZE, 3 * HALF_BLOCK_SIZE, Block_BG_DARK);
    break;
  case 1:
    graphics_fill_rect((x)*HALF_BLOCK_SIZE, (y)*HALF_BLOCK_SIZE, 1 * HALF_BLOCK_SIZE,
                       1 * HALF_BLOCK_SIZE, Block_BG_DARK);
    graphics_fill_rect((x)*HALF_BLOCK_SIZE, (y + 2) * HALF_BLOCK_SIZE,
                       1 * HALF_BLOCK_SIZE, 2 * HALF_BLOCK_SIZE, Block_BG_DARK);
    graphics_fill_rect((x + 2) * HALF_BLOCK_SIZE, (y)*HALF_BLOCK_SIZE,
                       1 * HALF_BLOCK_SIZE, 4 * HALF_BLOCK_SIZE, Block_BG_DARK);
    break;
  case 2:
    graphics_fill_rect((x)*HALF_BLOCK_SIZE, (y + 1) * HALF_BLOCK_SIZE,
                       2 * HALF_BLOCK_SIZE, 1 * HALF_BLOCK_SIZE, Block_BG_DARK);
    graphics_fill_rect((x + 1) * HALF_BLOCK_SIZE, (y + 3) * HALF_BLOCK_SIZE,
                       2 * HALF_BLOCK_SIZE, 1 * HALF_BLOCK_SIZE, Block_BG_DARK);
    break;
  case 3:
    graphics_fill_rect((x)*HALF_BLOCK_SIZE, (y + 1) * HALF_BLOCK_SIZE,
                       2 * HALF_BLOCK_SIZE, 1 * HALF_BLOCK_SIZE, Block_BG_DARK);
    graphics_fill_rect((x)*HALF_BLOCK_SIZE, (y + 3) * HALF_BLOCK_SIZE,
                       2 * HALF_BLOCK_SIZE, 1 * HALF_BLOCK_SIZE, Block_BG_DARK);
    break;
  case 4:
    graphics_fill_rect((x)*HALF_BLOCK_SIZE, (y)*HALF_BLOCK_SIZE, 2 * HALF_BLOCK_SIZE,
                       1 * HALF_BLOCK_SIZE, Block_BG_DARK);
    graphics_fill_rect((x)*HALF_BLOCK_SIZE, (y)*HALF_BLOCK_SIZE, 1 * HALF_BLOCK_SIZE,
                       2 * HALF_BLOCK_SIZE, Block_BG_DARK);
    graphics_fill_rect((x + 1) * HALF_BLOCK_SIZE, (y + 2) * HALF_BLOCK_SIZE,
                       1 * HALF_BLOCK_SIZE, 1 * HALF_BLOCK_SIZE, Block_BG_DARK);
    graphics_fill_rect((x)*HALF_BLOCK_SIZE, (y + 4) * HALF_BLOCK_SIZE,
                       2 * HALF_BLOCK_SIZE, 1 * HALF_BLOCK_SIZE, Block_BG_DARK);
    break;
  case 5:
    graphics_fill_rect((x + 1) * HALF_BLOCK_SIZE, (y + 1) * HALF_BLOCK_SIZE,
                       2 * HALF_BLOCK_SIZE, 1 * HALF_BLOCK_SIZE, Block_BG_DARK);
    graphics_fill_rect((x)*HALF_BLOCK_SIZE, (y + 3) * HALF_BLOCK_SIZE,
                       2 * HALF_BLOCK_SIZE, 1 * HALF_BLOCK_SIZE, Block_BG_DARK);
    break;
  case 6:
    graphics_fill_rect((x + 1) * HALF_BLOCK_SIZE, (y + 1) * HALF_BLOCK_SIZE,
                       2 * HALF_BLOCK_SIZE, 1 * HALF_BLOCK_SIZE, Block_BG_DARK);
    graphics_fill_rect((x + 1) * HALF_BLOCK_SIZE, (y + 3) * HALF_BLOCK_SIZE,
                       1 * HALF_BLOCK_SIZE, 1 * HALF_BLOCK_SIZE, Block_BG_DARK);
    break;
  case 7:
    graphics_fill_rect((x)*HALF_BLOCK_SIZE, (y + 1) * HALF_BLOCK_SIZE,
                       2 * HALF_BLOCK_SIZE, 1 * HALF_BLOCK_SIZE, Block_BG_DARK);
    graphics_fill_rect((x)*HALF_BLOCK_SIZE, (y + 2) * HALF_BLOCK_SIZE,
                       1 * HALF_BLOCK_SIZE, 2 * HALF_BLOCK_SIZE, Block_BG_DARK);
    graphics_fill_rect((x + 2) * HALF_BLOCK_SIZE, (y + 2) * HALF_BLOCK_SIZE,
                       1 * HALF_BLOCK_SIZE, 3 * HALF_BLOCK_SIZE, Block_BG_DARK);
    graphics_fill_rect((x + 1) * HALF_BLOCK_SIZE, (y + 4) * HALF_BLOCK_SIZE,
                       1 * HALF_BLOCK_SIZE, 1 * HALF_BLOCK_SIZE, Block_BG_DARK);
    break;
  case 8:
    graphics_fill_rect((x + 1) * HALF_BLOCK_SIZE, (y + 1) * HALF_BLOCK_SIZE,
                       1 * HALF_BLOCK_SIZE, 1 * HALF_BLOCK_SIZE, Block_BG_DARK);
    graphics_fill_rect((x + 1) * HALF_BLOCK_SIZE, (y + 3) * HALF_BLOCK_SIZE,
                       1 * HALF_BLOCK_SIZE, 1 * HALF_BLOCK_SIZE, Block_BG_DARK);
    break;
  case 9:
    graphics_fill_rect((x + 1) * HALF_BLOCK_SIZE, (y + 1) * HALF_BLOCK_SIZE,
                       1 * HALF_BLOCK_SIZE, 1 * HALF_BLOCK_SIZE, Block_BG_DARK);
    graphics_fill_rect((x)*HALF_BLOCK_SIZE, (y + 3) * HALF_BLOCK_SIZE,
                       2 * HALF_BLOCK_SIZE, 1 * HALF_BLOCK_SIZE, Block_BG_DARK);
    break;
  default:
    graphics_fill_rect((x + 1) * HALF_BLOCK_SIZE, (y + 1) * HALF_BLOCK_SIZE,
                       1 * HALF_BLOCK_SIZE, 3 * HALF_BLOCK_SIZE, Block_BG_DARK);
    break;
  }
}
void draw_apm(uint32_t apm) {
  uint8_t nums[3] = {0};
  uint8_t total_cnt = 0;
  for (total_cnt = 0; total_cnt < 4 && apm != 0; ++total_cnt) {
    nums[total_cnt] = apm % 10;
    apm /= 10;
  }
  for (int i = 0; i < 3; ++i) {
    if (i < total_cnt) {
      draw_number(APM_LEFT_MARGIN - i * 4, APM_TOP_MARGIN, nums[i], Block_SD);
    } else {
      draw_number(APM_LEFT_MARGIN - i * 4, APM_TOP_MARGIN, 0, Block_SD);
    }
  }
}
void draw_garbage_indicator(uint32_t garbage_cnt) {
  graphics_fill_rect((STAGE_LEFT_MARGIN - 1) * BLOCK_SIZE,
                     (STAGE_TOP_MARGIN)*BLOCK_SIZE, 1 * HALF_BLOCK_SIZE,
                     STAGE_H * BLOCK_SIZE, Block_BG_DARK);
  if (garbage_cnt == 0) {
    return;
  }
  if (garbage_cnt >= 22) {
    garbage_cnt = 22;
  }
  graphics_fill_rect((STAGE_LEFT_MARGIN - 1) * BLOCK_SIZE,
                     (STAGE_TOP_MARGIN + STAGE_H - garbage_cnt) * BLOCK_SIZE,
                     1 * HALF_BLOCK_SIZE, garbage_cnt * BLOCK_SIZE, Block_Z_L);
}
