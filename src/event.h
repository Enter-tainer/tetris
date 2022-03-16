#pragma once
#include "tetris.h"

// left arrow pressed -> while(is pressing) { if is first time && press_time >
// DAS move_left_step, wait ARR  }

// similar for right arrow

// down arrow pressed then released -> drop_step
// down arrow pressed -> while(is pressing) {
// if is first time && press_time > DAS move_left_step, wait SDF, reset
// gravity_timer  }

// space pressed -> harddrop -> while(1) { move_down_step } then [lock_mino,
// disable lock_timer], [spawn_mino, reset down_timer]

// z pressed -> rotate_counter_clock_wise
// x pressed -> rotate_clock_wise
// c pressed -> hold_mino -> [spawn_mino, reset down_timer]

// when gravity_timer arrived, reset it, and move down 1 step
// if mino cannot move down, disable gravity_timer and fire lock_timer

// when mino if moved, reset lock_timer
// when lock_timer arrives, [lock_mino, disable lock_timer], [spawn_mino, reset
// down_timer]

// basic structure: event loop
// while (1) {
//   check events
//   if event arrived, process event
//   sleep 1 frame time
// }
//
//
