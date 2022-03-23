#include "communicate.h"
#include "draw.h"
#include "garbage.h"
#include "graphics.h"
#include "handle.h"
#include "input.h"
#include "tetris.h"
#include "time_interface.h"
#include <stdint.h>
#ifndef RISCV
#include <string.h>
#include <time.h>
#include <unistd.h>
#else
#include "libdevice.h"
#endif
char drop_then_lock(struct Field* f, struct GameHandling* gh,
                    struct GarbageQueue* recv_queue) {
  while (drop_step(f))
    ;
  stop_timer(&gh->lock_timer);
  stop_timer(&gh->auto_shift_timer_left);
  stop_timer(&gh->auto_shift_timer_right);
  stop_timer(&gh->before_shift_timer_left);
  stop_timer(&gh->before_shift_timer_right);
  struct LockStatus g   = lock_mino(f);
  struct GarbageInfo gb = calculate_garbage(f, &g);
  f->stat.total_attack += gb.lines;
  if (gb.lines == 0) {
    while (!garbage_queue_empty(recv_queue)) {
      struct GarbageInfo tmp = garbage_queue_pop_front(recv_queue);
      add_garbage_to_field(f, &tmp);
    }
  } else {
    while (!garbage_queue_empty(recv_queue)) {
      struct GarbageInfo tmp = garbage_queue_pop_front(recv_queue);
      if (gb.lines >= tmp.lines) {
        gb.lines -= tmp.lines;
      } else {
        tmp.lines -= gb.lines;
        add_garbage_to_field(f, &tmp);
        garbage_queue_push_front(recv_queue, tmp);
        break;
      }
    }
    if (garbage_queue_empty(recv_queue) && gb.lines > 0) {
      send_data(garbage_to_byte(gb));
    }
  }
  // backfire, for test purpose only
  // add_garbage_to_field(f, &gb);
  struct OptionMinoType tmp = {.is_some = false};
  bool res = spawn_mino(f, tmp); // test failure here to indicate game over
  if (!res) {
    return 1;
  }
  return 0;
}

char field_update(struct Field* f, struct GameHandling* gh, struct KeyMap* key,
                  struct KeyMap* key_history, struct GarbageQueue* recv_queue,
                  int frame_count) {
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
        move_left_step(f);
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
        move_right_step(f);
      }
    }
    if (gh->lock_timer.is_started) {
      restart_timer(&gh->lock_timer);
    }
  }

  // rotate, and restart lock timer
  if (key->z) {
    key->z = 0;
    rotate_counter_clockwise(f);
    if (gh->lock_timer.is_started) {
      restart_timer(&gh->lock_timer);
    }
  }

  if (key->x) {
    key->x = 0;
    rotate_clockwise(f);
    if (gh->lock_timer.is_started) {
      restart_timer(&gh->lock_timer);
    }
  }

  if (key->c) {
    key->c = 0;
    hold_mino(f);
    if (gh->lock_timer.is_started) {
      restart_timer(&gh->lock_timer);
    }
  }
  if (key->space) {
    key->space = 0;
    if (drop_then_lock(f, gh, recv_queue)) {
      return 1;
    }
  }

  // auto drop, or soft drop, drop after every input ok
  int current_frame_gravity = gh->gravity / (key->down ? gh->sdf : 1);
  if (!gh->soft_drop_timer.is_started) {
    if (frame_count % current_frame_gravity == 0) {
      // drop and check lock
      if (!drop_step(f) && !(gh->lock_timer.is_started)) {
        restart_timer(&gh->lock_timer);
      }
    }
  } else {
    if (gh->soft_drop_timer.frames % current_frame_gravity == 0) {
      // drop and check lock
      if (!drop_step(f) && !(gh->lock_timer.is_started)) {
        restart_timer(&gh->lock_timer);
      }
    }
  }

  if (check_dropable(f)) {
    stop_timer(&gh->lock_timer);
  }

  // check lock
  if (gh->lock_timer.is_started && gh->lock_timer.frames >= gh->lock_frame) {
    if (drop_then_lock(f, gh, recv_queue)) {
      return 1;
    }
  }
  return 0;
}

void recv_queue_update(struct GarbageQueue* q) {
  while (recv_ready()) {
    uint8_t res = recv_data();
    if (!garbage_queue_full(q)) {
      garbage_queue_push_back(q, garbage_from_byte(res));
    }
  }
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
  struct GarbageQueue recv;
  graphics_init(SCREEN_W, SCREEN_H);
  while (true) {
    struct KeyMap key, key_history;
    memset(&key, 0, sizeof(key));
    memset(&key_history, 0, sizeof(key_history));
    draw_start_view();
    wait_any_key_down(&key);
    struct GameHandling gh = {
        .das = 10, // after 'das' frames holding, auto shift start
        .arr = 2,  // when auto shift starts, mino will move 1 block every 'arr'
                   // frames
        .sdf        = 2,  // when holding 'down' key, 'gravity'/='sdf'
        .gravity    = 60, // mino drop 1 block every 'gravity' frames
        .lock_frame = 30, // after 'lock_frame' frames, mino will lock
        .move_rate  = 20};
    init_gh(&gh);
    int64_t game_start_time = get_time_in_us();
    srand(game_start_time);
    init_field(&f, game_start_time);
    garbage_queue_init(&recv);
    int frame_count = 0;
    while (true) {
      int64_t frame_start_timestamp = get_time_in_us();
      draw(&f, recv.sum, get_apm(&f.stat, frame_start_timestamp));
      input_update(&key);
      recv_queue_update(&recv);
      if (field_update(&f, &gh, &key, &key_history, &recv, frame_count))
        break;
      increase_all_timers(&gh);
      ++frame_count;
      int64_t frame_end_timestamp = get_time_in_us();
      int64_t frame_time          = frame_end_timestamp - frame_start_timestamp;
      // 16666us per frame
      int64_t sleep_time = 16666 - frame_time;
      if (sleep_time > 0) {
        my_sleep(sleep_time);
      }
    }
    draw_end_view();
    wait_any_key_down(&key);
  }
  return 0;
}
