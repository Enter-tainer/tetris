#include "block.h"
#include "tetris.h"

enum BlockType get_block(int c) {
  switch (c) {
  case IBlock:
    return BlockI;
  case TBlock:
    return BlockT;
  case OBlock:
    return BlockO;
  case JBlock:
    return BlockJ;
  case LBlock:
    return BlockL;
  case SBlock:
    return BlockS;
  case ZBlock:
    return BlockZ;
  case Clean:
    return BlockBackground;
  case Shadow:
    return BlockShadow;
  case Garbage:
    return BlockShadow;
  }
  return BlockBackground;
}

enum BlockType get_block_light(int c) {
  switch (c) {
  case IBlock:
    return BlockILight;
  case TBlock:
    return BlockTLight;
  case OBlock:
    return BlockOLight;
  case JBlock:
    return BlockJLight;
  case LBlock:
    return BlockLLight;
  case SBlock:
    return BlockSLight;
  case ZBlock:
    return BlockZLight;
  }
  return BlockBackground;
}

enum BlockType get_block_top(int c) {
  switch (c) {
  case IBlock:
    return BlockITop;
  case TBlock:
    return BlockTTop;
  case OBlock:
    return BlockOTop;
  case JBlock:
    return BlockJTop;
  case LBlock:
    return BlockLTop;
  case SBlock:
    return BlockSTop;
  case ZBlock:
    return BlockZTop;
  }
  return BlockBackground;
}
