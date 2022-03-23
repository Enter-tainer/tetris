#pragma once
#include "block.h"
#include "tetris.h"
#include <stdint.h>

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

#define APM_LEFT_MARGIN 20
#define APM_TOP_MARGIN 48

#define SCREEN_W                                                               \
  ((STAGE_LEFT_MARGIN + STAGE_W + STAGE_RIGHT_MARGIN) * BLOCK_SIZE)
#define SCREEN_H (STAGE_H * BLOCK_SIZE)

#define MAX_PREVIEW 5

void draw(struct Field* f, uint32_t garbage_cnt, uint32_t apm);
void draw_start_view();
void draw_end_view();
void draw_number(int x, int y, uint8_t num, enum BlockType b);
void draw_apm(uint32_t apm);
void draw_garbage_indicator(uint32_t garbage_cnt);
