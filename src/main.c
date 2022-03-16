#include "draw.h"
#include "graphics.h"
#include "input.h"
#include "tetris.h"
#include <stddef.h>
#include <stdio.h>
#include <time.h>
#ifdef __linux__
#include <unistd.h>
#endif

void field_update(struct Field* f, struct KeyMap* key) {
  if (key->down) {
    key->down = 0;
    drop_step(f);
  }
  if (key->left) {
    key->left = 0;
    move_left_step(f);
  }
  if (key->right) {
    key->right = 0;
    move_right_step(f);
  }
  if (key->z) {
    key->z = 0;
    rotate_counter_clockwise(f);
  }
  if (key->x) {
    key->x = 0;
    rotate_clockwise(f);
  }
  if (key->c) {
    key->c = 0;
    hold_mino(f);
  }
  if (key->space) {
    key->space = 0;
    while (drop_step(f))
      ;
    lock_mino(f);
    struct OptionMinoType tmp = {.is_some = false};
    bool res = spawn_mino(f, tmp); // test failure here to indicate game over
    if (!res) {
      exit(1);
    }
  }
}

#ifdef __linux__
#define MAIN main
#else
#define MAIN WinMain
#endif
int MAIN(int argc, char* args[]) {
  struct Field f;
  struct KeyMap key;
  graphics_init(SCREEN_W, SCREEN_H);
  init_field(&f);
  while (true) {
    draw(&f);
    if (input_update(&key) == 1)
      return 0;
    field_update(&f, &key);
  }
  return 0;
}
