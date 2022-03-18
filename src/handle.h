#pragma once
// for soft drop, auto shift,lock delay, time recorder
struct GameTimer {
  int frames; // frames elapsed ater start
  char is_started; // whether start
};

struct GameHandling {
  int das, arr, sdf, gravity;
  int lock_frame, move_rate;
  struct GameTimer auto_shift_timer_left, auto_shift_timer_right,
      before_shift_timer_left, before_shift_timer_right, soft_drop_timer,
      lock_timer;
};

void init_gh(struct GameHandling* gh);

// start timer
void start_timer(struct GameTimer* timer);
// reset timer
void reset_timer(struct GameTimer* timer);
// increase everything in GH
void time_increase(struct GameHandling* gh);
