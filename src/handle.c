#include "handle.h"

void start_timer(struct GameTimer* timer) {
  timer->frames = 0;
  timer->is_started  = 1;
}

void reset_timer(struct GameTimer* timer) {
  timer->frames = 0;
  timer->is_started  = 0;
}

void increase_timer(struct GameTimer* timer) {
  if (timer->is_started)
    ++timer->frames;
}

void time_increase(struct GameHandling* gh) {
  increase_timer(&gh->auto_shift_timer_left);
  increase_timer(&gh->auto_shift_timer_right);
  increase_timer(&gh->before_shift_timer_left);
  increase_timer(&gh->before_shift_timer_right);
  increase_timer(&gh->soft_drop_timer);
  increase_timer(&gh->lock_timer);
}

void init_gh(struct GameHandling* gh) {
  reset_timer(&gh->auto_shift_timer_left);
  reset_timer(&gh->auto_shift_timer_right);
  reset_timer(&gh->before_shift_timer_left);
  reset_timer(&gh->before_shift_timer_right);
  reset_timer(&gh->soft_drop_timer);
  reset_timer(&gh->lock_timer);
}
