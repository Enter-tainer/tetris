#include "draw.h"
#include "graphics.h"
#include "handle.h"
#include "input.h"
#include "tetris.h"
#ifndef RISCV
#include <string.h>
#include <time.h>
#include <unistd.h>
#else
#include "libdevice.h"
#endif
char drop_then_lock(struct Field* f, struct GameHandling* gh,
                    struct KeyMap* key) {
  while (drop_step(f))
    ;
  stop_timer(&gh->lock_timer);
  stop_timer(&gh->auto_shift_timer_left);
  stop_timer(&gh->auto_shift_timer_right);
  stop_timer(&gh->before_shift_timer_left);
  stop_timer(&gh->before_shift_timer_right);
  key->left  = 0;
  key->right = 0;
  key->down  = 0;
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
  for (int i = 0; i < sizeof(struct KeyMap); ++i) {
    unsigned char* kh = (unsigned char*)key_history + i;
    unsigned char* ky = (unsigned char*)key + i;
    if (*ky == 0)
      *kh = 0;
    else
      *kh += 1;
  }
  if (key_history->left > gh->das && !(gh->auto_shift_timer_left.is_started)) {
    restart_timer(&gh->auto_shift_timer_left);
    stop_timer(&gh->before_shift_timer_left);
  } else if (!key_history->left) {
    stop_timer(&gh->auto_shift_timer_left);
  }
  if (key_history->right > gh->das &&
      !(gh->auto_shift_timer_right.is_started)) {
    restart_timer(&gh->auto_shift_timer_right);
    stop_timer(&gh->before_shift_timer_right);
  } else if (!key_history->right) {
    stop_timer(&gh->auto_shift_timer_right);
  }

  if (!key->down) {
    stop_timer(&gh->soft_drop_timer);
  }

  // key down, enable soft drop
  if (key->down) {
    restart_timer(&gh->soft_drop_timer);
  }
  // reset before timer if not holding
  if (!key->left) {
    stop_timer(&gh->before_shift_timer_left);
  }
  if (!key->right) {
    stop_timer(&gh->before_shift_timer_right);
  }

  // key left, check two timers
  if (key->left) {
    if (!(gh->auto_shift_timer_left.is_started)) {
      // start before timer or simply go left
      if (!(gh->before_shift_timer_left.is_started)) {
        restart_timer(&gh->before_shift_timer_left);
      }
      if (gh->before_shift_timer_left.frames % gh->move_rate == 0) {
        move_left_step(f);
      }
    } else {
      if (gh->auto_shift_timer_left.frames % gh->arr == 0) {
        bool res = move_left_step(f);
        if (!res) {
          stop_timer(&gh->auto_shift_timer_left);
          stop_timer(&gh->before_shift_timer_left);
          key->left = 0;
        }
      }
    }
    if (gh->lock_timer.is_started) {
      restart_timer(&gh->lock_timer);
    }
  }
  if (key->right) {
    if (!(gh->auto_shift_timer_right.is_started)) {
      // start before timer or simply go right
      if (!(gh->before_shift_timer_right.is_started)) {
        restart_timer(&gh->before_shift_timer_right);
      }
      if (gh->before_shift_timer_right.frames % gh->move_rate == 0) {
        move_right_step(f);
      }
    } else {
      if (gh->auto_shift_timer_right.frames % gh->arr == 0) {
        bool res = move_right_step(f);
        if (!res) {
          stop_timer(&gh->auto_shift_timer_right);
          stop_timer(&gh->before_shift_timer_right);
          key->right = 0;
        }
      }
    }
    if (gh->lock_timer.is_started) {
      restart_timer(&gh->lock_timer);
    }
  }

  // rotate, and restart lock timer
  if (key->z) {
    key->z = 0;
    if (!gh->z_debounce_timer.is_started) {
      rotate_counter_clockwise(f);
      restart_timer(&gh->z_debounce_timer);
      if (gh->lock_timer.is_started) {
        restart_timer(&gh->lock_timer);
      }
    }
  }

  if (key->x) {
    key->x = 0;
    if (!gh->x_debounce_timer.is_started) {
      rotate_clockwise(f);
      restart_timer(&gh->x_debounce_timer);
      if (gh->lock_timer.is_started) {
        restart_timer(&gh->lock_timer);
      }
    }
  }

  if (key->c) {
    key->c = 0;
    if (!gh->c_debounce_timer.is_started) {
      hold_mino(f);
      restart_timer(&gh->c_debounce_timer);
      stop_timer(&gh->lock_timer);
      stop_timer(&gh->auto_shift_timer_left);
      stop_timer(&gh->auto_shift_timer_right);
      stop_timer(&gh->before_shift_timer_left);
      stop_timer(&gh->before_shift_timer_right);
      key->left  = 0;
      key->right = 0;
    }
  }
  if (key->space) {
    key->space = 0;
    if (!gh->c_debounce_timer.is_started) {
      if (drop_then_lock(f, gh, key)) {
        return 1;
      }
      restart_timer(&gh->space_debounce_timer);
    }
  }

  // auto drop, or soft drop, drop after every input ok
  int current_frame_gravity = gh->gravity / (key->down ? gh->sdf : 1);
  if (!gh->soft_drop_timer.is_started) {
    if (frame_count % current_frame_gravity == 0) {
      // drop and check lock
      if (!drop_step(f) && !(gh->lock_timer.is_started)) {
        key->down = 0;
        restart_timer(&gh->lock_timer);
      }
    }
  } else {
    if (gh->soft_drop_timer.frames % current_frame_gravity == 0) {
      // drop and check lock
      if (!drop_step(f) && !(gh->lock_timer.is_started)) {
        key->down = 0;
        restart_timer(&gh->lock_timer);
      }
    }
  }

  if (check_dropable(f)) {
    stop_timer(&gh->lock_timer);
  }

  // check lock
  if (gh->lock_timer.is_started && gh->lock_timer.frames >= gh->lock_frame) {
    if (drop_then_lock(f, gh, key)) {
      return 1;
    }
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
    memset(&key, 0, sizeof(key));
    memset(&key_history, 0, sizeof(key_history));
    int frame_count = 0;
    draw_start_view();
    wait_any_key_down(&key);
    struct GameHandling gh = {
        .das = 8, // after 'das' frames holding, auto shift start
        .arr = 2, // when auto shift starts, mino will move 1 block every 'arr'
                  // frames
        .sdf        = 2,  // when holding 'down' key, 'gravity'/='sdf'
        .gravity    = 60, // mino drop 1 block every 'gravity' frames
        .lock_frame = 30, // after 'lock_frame' frames, mino will lock
        .move_rate  = 20,
        .block_op_frame =
            10, // block rotate, hold, space in 3 frames to avoid misdrop
    };
    init_gh(&gh);
#ifdef RISCV
    srand(time());
#else
    srand(clock());
#endif
    init_field(&f);
    while (true) {
#ifdef RISCV
      int64_t frame_start_timestamp = time();
#else
      int64_t frame_start_timestamp = clock();
#endif
      draw(&f);
      input_update(&key);
      if (field_update(&f, &gh, &key, &key_history, frame_count))
        break;
      increase_all_timers(&gh);
      ++frame_count;
      // 16666us per frame
#ifdef RISCV
      sleep(16666 - ((int64_t)time() - (int64_t)frame_start_timestamp) > 0
                ? 16666 - ((int64_t)time() - (int64_t)frame_start_timestamp)
                : 0);
#else
      usleep(16666 - ((int64_t)clock() - (int64_t)frame_start_timestamp) > 0
                 ? 16666 - ((int64_t)clock() - (int64_t)frame_start_timestamp)
                 : 0);
#endif
    }
    draw_end_view();
    wait_any_key_down(&key);
  }
  return 0;
}
