#pragma once
#include "random.h"
#include <stdbool.h>

enum MinoType { IMino, TMino, OMino, JMino, LMino, SMino, ZMino };
struct MinoQueue {
  enum MinoType data[14];
  int front, rear, size;
};

// init a queue, fill 2 bags in it
void init_queue(struct MinoQueue* queue);

// get a mino from queue front, refill the queue if one bag is used
enum MinoType pop_queue(struct MinoQueue* queue);

enum CellType {
  Clean,
  IBlock,
  TBlock,
  OBlock,
  JBlock,
  LBlock,
  SBlock,
  ZBlock,
  Shadow,
  Garbage,
};

enum RotationState { Original, Clockwise, CounterClockwise, R180 };
struct OptionMinoType {
  bool is_some;
  enum MinoType some;
};

struct FallingMino {
  int x, y; // the position of the center of gravity
  enum MinoType type;
  enum RotationState state;
};

// spawn a falling mino, setting its type and center position
void spawn_falling_mino(struct FallingMino* current, enum MinoType type);
// get the coordination of cells which are occupied by a falling mino
void get_cells(struct FallingMino* current, int* x_array, int* y_array);

struct Field {
  enum CellType field[40][10];
  struct FallingMino current;
  struct OptionMinoType hold;
  struct MinoQueue next;
  /*  x
   *  ▲
   *  │
   * 39──────────────────────────────┐
   *  │                              │
   *  │                              │
   *  │                              │
   *  │                              │
   *  │                              │
   *  │                              │
   *  │                              │
   *  │                              │
   *  │                              │
   *  │                              │
   *  │                              │
   *  │                              │
   *  │                              │
   *  │                              │
   *  │                              │
   *  │                              │
   *  │                              │
   *  │                              │
   *  │                              │
   *  │                              │
   *  │                              │
   *  │                              │
   *  │                              │
   *  └──────────────────────────────┴─► y
   * O                               9
   */
};
