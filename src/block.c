#include "block.h"
#include "tetris.h"

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