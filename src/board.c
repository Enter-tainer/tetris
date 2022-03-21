#include "random.h"
#include "tetris.h"
#include <stdbool.h>
#include <stdint.h>
void get_field(struct Field* f, enum CellType field[40][10]) {
  for (int i = 0; i < 40; ++i) {
    for (int j = 0; j < 10; ++j) {
      field[i][j] = f->field[i][j];
    }
  }
  int cell_x[4], cell_y[4];
  get_cells(&f->ghost, cell_x, cell_y);
  for (int i = 0; i < 4; ++i) {
    field[cell_x[i]][cell_y[i]] = Shadow;
  }
  get_cells(&f->current, cell_x, cell_y);
  for (int i = 0; i < 4; ++i) {
    field[cell_x[i]][cell_y[i]] = to_cell_type(f->current.type);
  }
}
void init_field(struct Field* f) {
  for (int i = 0; i < 40; ++i) {
    for (int j = 0; j < 10; ++j) {
      f->field[i][j] = Clean;
    }
  }
  f->ren_cnt = 0;
  f->b2b_cnt = 0;
  init_queue(&f->next);
  f->allow_hold = true;
  f->last_spin_is_t_spin = false;
  struct OptionMinoType tmp = {.is_some = false};
  f->hold                   = tmp;
  spawn_mino(f, tmp);
}
void set_ghost_piece(struct Field* f) {
  struct FallingMino current_back = f->current;
  while (drop_step(f))
    ;
  f->ghost   = f->current;
  f->current = current_back;
}

struct OptionMinoType get_hold(struct Field* f) {
  return f->hold;
}

struct MinoQueue* get_preview(struct Field* f) {
  return &f->next;
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
    if (!is_valid(cell_x[i], cell_y[i]) ||
        is_occupied(f, cell_x[i], cell_y[i])) {
      return true;
    }
  }
  return false;
}
bool move_left_step(struct Field* f) {
  f->last_spin_is_t_spin   = false;
  struct FallingMino piece = f->current;
  piece.y -= 1;
  if (!is_obstructed(f, &piece)) {
    f->current = piece;
    set_ghost_piece(f);
    return true;
  }
  return false;
}
bool move_right_step(struct Field* f) {
  f->last_spin_is_t_spin   = false;
  struct FallingMino piece = f->current;
  piece.y += 1;
  if (!is_obstructed(f, &piece)) {
    f->current = piece;
    set_ghost_piece(f);
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
bool check_dropable(struct Field* f) {
  struct FallingMino piece = f->current;
  piece.x -= 1;
  if (!is_obstructed(f, &piece))
    return true;
  return false;
}

bool rotate_clockwise(struct Field* f) {
  f->last_spin_is_t_spin          = false;
  int test_cnt                    = 0;
  struct FallingMino current_back = f->current;
  for (; test_cnt < 5; ++test_cnt) {
    rotate_falling_mino_clockwise(&f->current, test_cnt);
    if (!is_obstructed(f, &f->current)) {
      break;
    } else {
      f->current = current_back;
    }
  }
  if (f->current.type == TMino) {
    struct FallingMino tmp = f->current;
    bool cannot_move       = true;
    tmp.x++; // up
    cannot_move &= is_obstructed(f, &tmp);
    tmp.x--;
    tmp.y++;
    cannot_move &= is_obstructed(f, &tmp);
    tmp.y -= 2;
    cannot_move &= is_obstructed(f, &tmp);
    f->last_spin_is_t_spin = cannot_move;
  }
  set_ghost_piece(f);
  return test_cnt <= 5;
}

bool rotate_counter_clockwise(struct Field* f) {
  f->last_spin_is_t_spin          = false;
  int test_cnt                    = 0;
  struct FallingMino current_back = f->current;
  for (; test_cnt < 5; ++test_cnt) {
    rotate_falling_mino_counter_clockwise(&f->current, test_cnt);
    if (!is_obstructed(f, &f->current)) {
      break;
    } else {
      f->current = current_back;
    }
  }
  if (f->current.type == TMino) {
    struct FallingMino tmp = f->current;
    bool cannot_move       = true;
    tmp.x++; // up
    cannot_move &= is_obstructed(f, &tmp);
    tmp.x--;
    tmp.y++;
    cannot_move &= is_obstructed(f, &tmp);
    tmp.y -= 2;
    cannot_move &= is_obstructed(f, &tmp);
    f->last_spin_is_t_spin = cannot_move;
  }
  set_ghost_piece(f);
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
    set_ghost_piece(f);
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
  f->last_spin_is_t_spin = false;
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

struct GameStatus lock_mino(struct Field* f) {
  int cell_x[4], cell_y[4];
  struct GameStatus res = {0};
  get_cells(&f->current, cell_x, cell_y);
  for (int i = 0; i < 4; ++i) {
    int x = cell_x[i], y = cell_y[i];
    f->field[x][y] = to_cell_type(f->current.type);
  }
  // clean lines
  enum CellType new_field[40][10];
  for (int i = 0; i < 40; ++i) {
    for (int j = 0; j < 10; ++j) {
      new_field[i][j] = Clean;
    }
  }
  for (int i = 0, k = 0; i < 40; ++i) {
    bool filled = true;
    for (int j = 0; j < 10; ++j) {
      filled &= is_occupied(f, i, j);
    }
    if (!filled) {
      for (int j = 0; j < 10; ++j) {
        new_field[k][j] = f->field[i][j];
      }
      k++;
    } else {
      res.lines_cleared++;
    }
  }
  res.is_pc = 1;
  for (int i = 0; i < 40; ++i) {
    for (int j = 0; j < 10; ++j) {
      f->field[i][j] = new_field[i][j];
      res.is_pc &= (f->field[i][j] == Clean);
    }
  }
  res.is_t_spin = f->last_spin_is_t_spin;
  f->allow_hold = true;
  if (f->last_spin_is_t_spin || res.lines_cleared == 4) {
    f->b2b_cnt++;
  } else {
    f->b2b_cnt = 0;
  }
  if (res.lines_cleared != 0) {
    f->ren_cnt++;
  } else {
    f->ren_cnt = 0;
  }
  return res;
}

void add_garbage_to_field(struct Field* f, struct GarbageInfo* g) {
  enum CellType new_field[40][10];
  for (int i = 0; i < g->lines; ++i) {
    for (int j = 0; j < 10; ++j) {
      if (j == g->slot_y) {
        new_field[i][j] = Clean;
      } else {
        new_field[i][j] = Garbage;
      }
    }
  }
  for (int i = 0; i < 40; ++i) {
    for (int j = 0; j < 10; ++j) {
      if (i + g->lines < 40) {
        new_field[i + g->lines][j] = f->field[i][j];
      }
    }
  }
  for (int i = 0; i < 40; ++i) {
    for (int j = 0; j < 10; ++j) {
      f->field[i][j] = new_field[i][j];
    }
  }
}

struct GarbageInfo calculate_garbage(struct Field* f, struct GameStatus* g) {
  uint8_t combo_garbage[] = {
      0, 0,
      0, // 0, 1 combo
      1,
      1, // 2, 3 combo
      2,
      2, // 4, 5 combo
      3,
      3, // 6, 7 combo
      4, 4,
      4, // 8, 9, 10 combo
      5, // 11+ combo
  };
  uint8_t combo_bonus = combo_garbage[f->ren_cnt >= 12 ? 12 : f->ren_cnt];
  uint8_t b2b_bonus   = f->b2b_cnt > 2;
  uint8_t pc_bonus    = g->is_pc ? 10 : 0;
  uint8_t attack      = 0;
  if (g->is_t_spin) {
    attack = combo_bonus + b2b_bonus + pc_bonus + g->lines_cleared * 2;
  } else if (g->lines_cleared == 4) {
    attack = combo_bonus + b2b_bonus + pc_bonus + 4;
  } else {
    attack = combo_bonus + b2b_bonus + pc_bonus +
             (g->lines_cleared > 1 ? g->lines_cleared - 1 : 0);
  }
  if (attack > 15) {
    attack = 15;
  }
  struct GarbageInfo res = {0};
  res.slot_y             = my_rand() % 10;
  res.lines              = attack;
  return res;
}
