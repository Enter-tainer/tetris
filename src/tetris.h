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
  IBlock,
  TBlock,
  OBlock,
  JBlock,
  LBlock,
  SBlock,
  ZBlock,
  Clean,
  Shadow,
  Garbage,
};

enum RotationState { Original, Clockwise, R180, CounterClockwise };
static inline enum RotationState rotate_state_clockwise(enum RotationState s) {
  return (enum RotationState)((int)s + 1) % 4;
};

static inline enum RotationState
rotate_state_counter_clockwise(enum RotationState s) {
  return (enum RotationState)((int)s - 1 + 4) % 4;
};
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
// rotate the given mino, with considering its kick tests
void rotate_falling_mino_clockwise(struct FallingMino* current, int test_cnt);
void rotate_falling_mino_counter_clockwise(struct FallingMino* current, int test_cnt);

struct Field {
  enum CellType field[40][10];
  struct FallingMino current;
  struct FallingMino ghost;
  struct OptionMinoType hold;
  struct MinoQueue next;
  bool allow_hold;
  /*  x
   *  ▲
   *  │
   * 39────────┐
   *  │        │
   *  │        │
   *  └────────┴─► y
   * O         9
   */
};

void init_field(struct Field* f);

// compute the ghost piece, and set it to f->ghost
void set_ghost_piece(struct Field* f);

// return false if the given cell is already occupied
bool is_occupied(struct Field* f, int x, int y);
// return false if the given position is out of the field
bool is_valid(int x, int y);
// return false if the piece if obstructed in this position
bool is_obstructed(struct Field* f, struct FallingMino* piece);

// move current to left for one step
// return false if move failed
bool move_left_step(struct Field* f);
// move current to right for one step
// return false if move failed
bool move_right_step(struct Field* f);
// drop current for one step
// return false if move failed (already on the ground)
bool drop_step(struct Field* f);

// rotate the current teramino clockwise
// return false if rotate failed
bool rotate_clockwise(struct Field* f);
// rotate the current teramino counter clockwise
// return false if rotate failed
bool rotate_counter_clockwise(struct Field* f);

// spawn a new mino in field, set current, with optional type argument
// if type if specified, next queue will remain untouched
// return false if new mino cannot be spawned, i.e. failed
bool spawn_mino(struct Field* f, struct OptionMinoType type);

// swap hold and current mino
bool hold_mino(struct Field* f);

static inline enum CellType to_cell_type(enum MinoType t) {
  // CellType and Minotype has the same order, it's OK to perform conversion
  return (enum CellType)t;
}

// lock a mino, and perform line clean
// TODO: when to judge T-spin(?)
int lock_mino(struct Field* f);
