#include "handle.h"

void restart_timer(struct GameTimer* timer) {
  timer->frames = 0;
  timer->is_started  = 1;
}

void stop_timer(struct GameTimer* timer) {
  timer->frames = 0;
  timer->is_started  = 0;
}

void increase_timer(struct GameTimer* timer) {
  if (timer->is_started)
    ++timer->frames;
}

void increase_all_timers(struct GameHandling* gh) {
  increase_timer(&gh->auto_shift_timer_left);
  increase_timer(&gh->auto_shift_timer_right);
  increase_timer(&gh->before_shift_timer_left);
  increase_timer(&gh->before_shift_timer_right);
  increase_timer(&gh->soft_drop_timer);
  increase_timer(&gh->lock_timer);
  increase_timer(&gh->z_debounce_timer);
  if (gh->z_debounce_timer.frames > gh->block_op_frame) {
    stop_timer(&gh->z_debounce_timer);
  }
  increase_timer(&gh->x_debounce_timer);
  if (gh->x_debounce_timer.frames > gh->block_op_frame) {
    stop_timer(&gh->x_debounce_timer);
  }
  increase_timer(&gh->c_debounce_timer);
  if (gh->c_debounce_timer.frames > gh->block_op_frame) {
    stop_timer(&gh->c_debounce_timer);
  }
  increase_timer(&gh->space_debounce_timer);
  if (gh->space_debounce_timer.frames > gh->block_op_frame) {
    stop_timer(&gh->space_debounce_timer);
  }
}

void init_gh(struct GameHandling* gh) {
  stop_timer(&gh->auto_shift_timer_left);
  stop_timer(&gh->auto_shift_timer_right);
  stop_timer(&gh->before_shift_timer_left);
  stop_timer(&gh->before_shift_timer_right);
  stop_timer(&gh->soft_drop_timer);
  stop_timer(&gh->lock_timer);
  stop_timer(&gh->z_debounce_timer);
  stop_timer(&gh->x_debounce_timer);
  stop_timer(&gh->c_debounce_timer);
  stop_timer(&gh->space_debounce_timer);
}
