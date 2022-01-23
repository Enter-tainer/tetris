#include "tetris.h"
#include <stdbool.h>

void init_field(struct Field* f) {
  for (int i = 0; i < 40; ++i) {
    for (int j = 0; j < 0; ++j) {
      f->field[i][j] = Clean;
    }
  }
  init_queue(&f->next);
  f->allow_hold = true;
  struct OptionMinoType tmp = {.is_some = false};
  spawn_mino(f, tmp);
  set_ghost_piece(f);
}
void set_ghost_piece(struct Field* f) {
  struct FallingMino current_back = f->current;
  while (drop_step(f))
    ;
  f->ghost   = f->current;
  f->current = current_back;
}

bool is_valid(int x, int y) { return x >= 0 && y >= 0 && x < 40 && y < 10; }
bool is_occupied(struct Field* f, int x, int y) {
  switch (f->field[x][y]) {
  case IBlock:
  case TBlock:
  case OBlock:
  case JBlock:
  case LBlock:
  case SBlock:
  case ZBlock:
  case Garbage:
    return true;
  case Clean:
  case Shadow:
    return false;
  }
}

bool is_obstructed(struct Field* f, struct FallingMino* piece) {
  int cell_x[4], cell_y[4];
  get_cells(piece, cell_x, cell_y);
  for (int i = 0; i < 4; ++i) {
    if (is_valid(cell_x[i], cell_y[i]) &&
        is_occupied(f, cell_x[i], cell_y[i])) {
      return true;
    }
  }
  return false;
}
bool move_left_step(struct Field* f) {
  struct FallingMino piece = f->current;
  piece.y -= 1;
  if (!is_obstructed(f, &piece)) {
    f->current = piece;
    return true;
  }
  return false;
}
bool move_right_step(struct Field* f) {
  struct FallingMino piece = f->current;
  piece.y += 1;
  if (!is_obstructed(f, &piece)) {
    f->current = piece;
    return true;
  }
  return false;
}
bool drop_step(struct Field* f) {
  struct FallingMino piece = f->current;
  piece.x -= 1;
  if (!is_obstructed(f, &piece)) {
    f->current = piece;
    return true;
  }
  return false;
}

bool rotate_clockwise(struct Field* f) {
  int test_cnt = 1;
  struct FallingMino current_back = f->current;
  for (; test_cnt <= 5; ++test_cnt) {
    rotate_falling_mino_clockwise(&f->current, test_cnt);
    if (!is_obstructed(f, &f->current)) {
      break;
    } else {
      f->current = current_back;
    }
  }
  return test_cnt <= 5;
}

bool rotate_counter_clockwise(struct Field* f) {
  int test_cnt                    = 1;
  struct FallingMino current_back = f->current;
  for (; test_cnt <= 5; ++test_cnt) {
    rotate_falling_mino_counter_clockwise(&f->current, test_cnt);
    if (!is_obstructed(f, &f->current)) {
      break;
    } else {
      f->current = current_back;
    }
  }
  // if all tests are failed, current will remain untouched
  // otherwise, current has been rotated
  return test_cnt <= 5;
}

bool spawn_mino(struct Field* f, struct OptionMinoType type) {
  enum MinoType next_type;
  if (type.is_some) {
    // use hold mino
    next_type = type.some;
  } else {
    next_type = pop_queue(&f->next);
  }
  struct FallingMino next;
  spawn_falling_mino(&next, next_type);
  if (!is_obstructed(f, &next)) {
    // OK
    f->current = next;
    if (!type.is_some) {
      // if current mino is not from hold, allow hold
      f->allow_hold = true;
    }
    return true;
  }
  return false;
}
bool hold_mino(struct Field* f) {
  // TODO:
  if (!f->allow_hold) {
    return false;
  }
  // can only hold 1 time
  f->allow_hold         = false; // remember to reset allow_hold when mino locks
  enum MinoType current = f->current.type;
  struct OptionMinoType tmp = {.is_some = true, .some = current};
  spawn_mino(f, f->hold);
  f->hold = tmp;
  return true;
}

int lock_mino(struct Field* f) {
  int cell_x[4], cell_y[4];
  get_cells(&f->current, cell_x, cell_y);
  for (int i = 0; i < 4; ++i) {
    int x = cell_x[i], y = cell_y[i];
    f->field[x][y] = to_cell_type(f->current.type);
  }
  // clean lines
  enum CellType new_field[40][10];
  for (int i = 0; i < 40; ++i) {
    for (int j = 0; j < 0; ++j) {
      new_field[i][j] = Clean;
    }
  }
  int lines_cleared = 0;
  for (int i = 0; i < 40; ++i) {
    bool filled = true;
    for (int j = 0; j < 10; ++j) {
      filled &= is_occupied(f, i, j);
    }
    if (!filled) {
      for (int j = 0; j < 10; ++j) {
        new_field[i][j] = f->field[i][j];
      }
    } else {
      lines_cleared++;
    }
  }
  for (int i = 0; i < 40; ++i) {
    for (int j = 0; j < 0; ++j) {
      f->field[i][j] = new_field[i][j];
    }
  }
  f->allow_hold = true;
  return lines_cleared;
}
