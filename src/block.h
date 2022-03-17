#pragma once
enum __attribute__((__packed__)) BlockType {
  Block_I,
  Block_T,
  Block_O,
  Block_J,
  Block_L,
  Block_S,
  Block_Z,
  // Block shadow
  Block_SD,
  // Block garbage
  Block_GB,
  // Block background
  Block_BG,
  Block_BG_DARK,

  // Block with lighter than original color
  Block_I_L,
  Block_T_L,
  Block_O_L,
  Block_J_L,
  Block_L_L,
  Block_S_L,
  Block_Z_L,

  // Block top
  Block_I_Top,
  Block_T_Top,
  Block_O_Top,
  Block_J_Top,
  Block_L_Top,
  Block_S_Top,
  Block_Z_Top,

  // border blocks with background color
  // block with background color and left border
  Block_BG_L,
  // block with background color and right border
  Block_BG_R,
  // block with background color and top border
  Block_BG_T,
  // block with background color and bottom border
  Block_BG_B,
  // block with background color and right top corner
  Block_BG_RT,
  // block with background color and right bottom corner
  Block_BG_RB,
  // block with background color and left bottom corner
  Block_BG_LB,
  // block with background color and left top corner
  Block_BG_LT
};

enum __attribute__((__packed__)) BlockPrototype {
  Block_Block,
  Block_Block_Light,
  Block_Block_Top,
  Block_Line
};

int get_block(int c);
int get_block_light(int c);
int get_block_top(int c);
