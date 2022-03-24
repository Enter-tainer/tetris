#pragma once
enum __attribute__((__packed__)) BlockType {
  BlockI,
  BlockT,
  BlockO,
  BlockJ,
  BlockL,
  BlockS,
  BlockZ,
  // Block shadow
  BlockShadow,
  // Block garbage
  BlockGarbage,
  // Block background
  BlockBackground,
  BlockBackgroundDark,

  // Block with lighter than original color
  BlockILight,
  BlockTLight,
  BlockOLight,
  BlockJLight,
  BlockLLight,
  BlockSLight,
  BlockZLight,

  // Block top
  BlockITop,
  BlockTTop,
  BlockOTop,
  BlockJTop,
  BlockLTop,
  BlockSTop,
  BlockZTop,

  // border blocks with background color
  // block with background color and left border
  BlockBackgroundLeft,
  // block with background color and right border
  BlockBackgroundRight,
  // block with background color and top border
  BlockBackgroundTop,
  // block with background color and bottom border
  BlockBackgroundBottom,
  // block with background color and right top corner
  BlockBackgroundRightTop,
  // block with background color and right bottom corner
  BlockBackgroundRightBottom,
  // block with background color and left bottom corner
  BlockBackgroundLeftBottom,
  // block with background color and left top corner
  BlockBackgroundLeftTop
};

enum __attribute__((__packed__)) BlockPrototype {
  BlockBlock,
  BlockBlockLight,
  BlockBlockTop,
};

enum BlockType get_block(int c);
enum BlockType get_block_light(int c);
enum BlockType get_block_top(int c);
