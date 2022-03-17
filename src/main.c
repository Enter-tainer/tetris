#include "draw.h"
#include "graphics.h"
#include "input.h"
#include "tetris.h"
#include <stddef.h>
#include <stdio.h>
#ifdef __linux__
#include <time.h>
#include <unistd.h>
#else
#include "libdevice.h"
#endif
char handle_lock(struct Field* f) {
  lock_mino(f);
  struct OptionMinoType tmp = {.is_some = false};
  bool res = spawn_mino(f, tmp); // test failure here to indicate game over
  if (!res) {
    return 1;
  }
  return 0;
}

char field_update(struct Field* f, struct GameHandling* gh, struct KeyMap* key,
                  struct KeyMap* key_history, int frame_count) {
  if (key->left && key->right)
    key->right = 0;
  for (int i = 0; i < 11; ++i) {
    unsigned char* kh = (unsigned char*)key_history + i;
    unsigned char* ky = (unsigned char*)key + i;
    if (*ky == 0)
      *kh = 1;
    else
      *kh += 1;
  }
  if (key_history->left > gh->das)
    gh->is_as_l = 1;
  if (key_history->right > gh->das)
    gh->is_as_r = 1;
  // fprintf(stderr, "%d %d\n", key->left, key->right);
  int current_frame_gravity = gh->gravity / (key->down ? gh->sdf : 1);
  if (frame_count % current_frame_gravity == 0) {
    if (!drop_step(f) && !(gh->is_lock)) {
      gh->is_lock    = 1;
      gh->lock_timer = frame_count;
    }
  }
  if (key->down) {
    key->down = 0;
    if (!drop_step(f) && !(gh->is_lock)) {
      gh->is_lock    = 1;
      gh->lock_timer = frame_count;
    }
  }
  if (key->left) {
    key->left = 0;
    if (!gh->is_as_l)
      move_left_step(f);
    if (gh->is_lock)
      gh->lock_timer = frame_count;
  }
  if (key->right) {
    key->right = 0;
    if (!gh->is_as_r)
      move_right_step(f);
    if (gh->is_lock)
      gh->lock_timer = frame_count;
  }
  if (gh->is_as_l) {
    if (frame_count % gh->arr == 0) {
      move_left_step(f);
    }
  }
  if (gh->is_as_r) {
    if (frame_count % gh->arr == 0) {
      move_right_step(f);
    }
  }
  if (key->z) {
    key->z = 0;
    gh->is_as_l = gh->is_as_r = 0;
    rotate_counter_clockwise(f);
    if (gh->is_lock)
      gh->lock_timer = frame_count;
  }
  if (key->x) {
    key->x = 0;
    gh->is_as_l = gh->is_as_r = 0;
    rotate_clockwise(f);
    if (gh->is_lock)
      gh->lock_timer = frame_count;
  }
  if (key->c) {
    key->c = 0;
    hold_mino(f);
    gh->is_as_l = gh->is_as_r = 0;
    if (gh->is_lock)
      gh->lock_timer = frame_count;
  }
  if (key->space) {
    key->space = 0;
    gh->is_as_l = gh->is_as_r = 0;
    while (drop_step(f))
      ;
    gh->is_lock = 0;
    if (handle_lock(f)) {
      return 1;
    }
  }
  if (gh->is_lock && frame_count - gh->lock_timer >= gh->lock_frame) {
    gh->is_lock = 0;
    if (handle_lock(f)) {
      return 1;
    }
  }
  return 0;
}

#ifdef __linux__
#define MAIN main
#else
#define MAIN WinMain
#endif
int MAIN(int argc, char* args[]) {
  struct Field f;
  graphics_init(SCREEN_W, SCREEN_H);
  while (true) {
    struct KeyMap key, key_history;
    for (int i = 0; i < 11; ++i) {
      *((unsigned char*)&key_history + i) = 0;
      *((unsigned char*)&key + i)         = 0;
    }
    init_field(&f);
    int frame_count = 0;
    draw_start_view();
    wait_any_key_down(&key);
    struct GameHandling gh = {
        .das        = 3,
        .arr        = 10,
        .sdf        = 2,
        .gravity    = 30,
        .is_as_l    = 0,
        .is_as_r    = 0,
        .lock_frame = 30,
        .is_lock    = 0,
        .lock_timer = 0,
    };
    while (true) {
#ifndef __linux__
      uint32_t tim = time();
#else
      uint32_t tim = clock();
#endif
      draw(&f);
      input_update(&key);
      if (field_update(&f, &gh, &key, &key_history, frame_count))
        break;
      ++frame_count;
      // 16666us per frame
#ifndef __linux__
      sleep(16666 - (time() - tim));
#else
      usleep(16666 - (clock() - tim));
#endif
      // fprintf(stderr, "fc = %d\n", frame_count);
    }
    draw_end_view();
#ifndef __linux__
    sleep(3000000);
#else
    usleep(3000000);
#endif
    wait_any_key_down(&key);
  }

  return 0;
}
