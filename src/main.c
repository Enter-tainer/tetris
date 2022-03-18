#include "draw.h"
#include "graphics.h"
#include "handle.h"
#include "input.h"
#include "tetris.h"
#ifndef RISCV
#include <time.h>
#include <unistd.h>
#else
#include "libdevice.h"
#endif
char handle_lock(struct Field* f, struct GameHandling* gh) {
  while (drop_step(f))
    ;
  reset_timer(&gh->lock_timer);
  reset_timer(&gh->auto_shift_timer_left);
  reset_timer(&gh->auto_shift_timer_right);
  reset_timer(&gh->before_shift_timer_left);
  reset_timer(&gh->before_shift_timer_right);
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
      *kh = 0;
    else
      *kh += 1;
  }
  if (key_history->left > gh->das && !(gh->auto_shift_timer_left.start))
    start_timer(&gh->auto_shift_timer_left),
        reset_timer(&gh->before_shift_timer_left);
  else if (!key_history->left)
    reset_timer(&gh->auto_shift_timer_left);
  if (key_history->right > gh->das && !(gh->auto_shift_timer_right.start))
    start_timer(&gh->auto_shift_timer_right),
        reset_timer(&gh->before_shift_timer_right);
  else if (!key_history->right)
    reset_timer(&gh->auto_shift_timer_right);

  // key down, enable soft drop
  if (key->down) {
    start_timer(&gh->soft_drop_timer);
    if (!(gh->lock_timer.start)) {
      start_timer(&gh->lock_timer);
    }
  }
  // reset before timer if not holding
  if (!key->left) {
    reset_timer(&gh->before_shift_timer_left);
  }
  if (!key->right) {
    reset_timer(&gh->before_shift_timer_right);
  }

  // key left, check two timers
  if (key->left) {
    if (!(gh->auto_shift_timer_left.start)) {
      // start before timer or simply go left
      if (!(gh->before_shift_timer_left.start)) {
        start_timer(&gh->before_shift_timer_left);
      }
      if (gh->before_shift_timer_left.frames % gh->move_rate == 0) {
        move_left_step(f);
      }
    } else {
      if (gh->auto_shift_timer_left.frames % gh->arr == 0) {
        move_left_step(f);
      }
    }
    if (gh->lock_timer.start)
      start_timer(&gh->lock_timer);
  }
  if (key->right) {
    if (!(gh->auto_shift_timer_right.start)) {
      // start before timer or simply go right
      if (!(gh->before_shift_timer_right.start)) {
        start_timer(&gh->before_shift_timer_right);
      }
      if (gh->before_shift_timer_right.frames % gh->move_rate == 0) {
        move_right_step(f);
      }
    } else {
      if (gh->auto_shift_timer_right.frames % gh->arr == 0) {
        move_right_step(f);
      }
    }
      if (gh->lock_timer.start)
        start_timer(&gh->lock_timer);
  }

  // rotate, and restart lock timer
  if (key->z) {
    key->z = 0;
    rotate_counter_clockwise(f);
    if (gh->lock_timer.start)
      start_timer(&gh->lock_timer);
  }

  if (key->x) {
    key->x = 0;
    rotate_clockwise(f);
    if (gh->lock_timer.start)
      start_timer(&gh->lock_timer);
  }

  if (key->c) {
    key->c = 0;
    hold_mino(f);
    if (gh->lock_timer.start)
      start_timer(&gh->lock_timer);
  }
  if (key->space) {
    key->space = 0;
    if (handle_lock(f, gh))
      return 1;
  }

  // auto drop, or soft drop, drop after every input ok
  int current_frame_gravity = gh->gravity / (key->down ? gh->sdf : 1);
  if (!gh->soft_drop_timer.start) {
    if (frame_count % current_frame_gravity == 0) {
      // drop and check lock
      if (!drop_step(f) && !(gh->lock_timer.start)) {
        start_timer(&gh->lock_timer);
      }
    }
  } else {
    if (gh->soft_drop_timer.frames % current_frame_gravity == 0) {
      // drop and check lock
      if (!drop_step(f) && !(gh->lock_timer.start)) {
        start_timer(&gh->lock_timer);
      }
    }
  }

  // check lock
  if (gh->lock_timer.start && gh->lock_timer.frames >= gh->lock_frame) {
    if (handle_lock(f, gh))
      return 1;
  }
  return 0;
}
#ifdef __linux__
#define MAIN main
#elif RISCV
#define MAIN main
#else
#define MAIN WinMain
#endif
int MAIN(int argc, char* args[]) {
  struct Field f;
  graphics_init(SCREEN_W, SCREEN_H);
  while (true) {
    struct KeyMap key, key_history;
    for (int i = 0; i < sizeof(struct KeyMap); ++i) {
      *((unsigned char*)&key_history + i) = 0;
      *((unsigned char*)&key + i)         = 0;
    }
    init_field(&f);
    int frame_count = 0;
    draw_start_view();
    wait_any_key_down(&key);
    struct GameHandling gh = {
        .das = 10, // after 'das' frames holding, auto shift start
        .arr = 5,  // when auto shift starts, mino will move 1 block every 'arr'
                   // frames
        .sdf        = 2,  // when holding 'down' key, 'gravity'/='sdf'
        .gravity    = 60, // mino drop 1 block every 'gravity' frames
        .lock_frame = 30, // after 'lock_frame' frames, mino will lock
        .lock_timer = 0,  // lock_timer
        .move_rate  = 20};
    init_gh(&gh);
    while (true) {
#ifdef RISCV
      int64_t tim = time();
#else
      int64_t tim = clock();
#endif
      draw(&f);
      input_update(&key);
      if (field_update(&f, &gh, &key, &key_history, frame_count))
        break;
      time_increase(&gh);
      ++frame_count;
      // 16666us per frame
#ifdef RISCV
      sleep(16666 - ((int64_t)time() - tim) > 0
                ? 16666 - ((int64_t)time() - tim)
                : 0);
#else
      usleep(16666 - ((int64_t)clock() - tim) > 0
                 ? 16666 - ((int64_t)clock() - tim)
                 : 0);
#endif
    }
    draw_end_view();
    wait_any_key_down(&key);
  }
  return 0;
}
