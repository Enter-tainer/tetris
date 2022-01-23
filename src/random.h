#pragma once
#include <inttypes.h>
#include <stdint.h>
#include "tetris.h"
struct XorShift {
  uint32_t a;
};
uint32_t xorshift32(struct XorShift* state);
uint32_t my_rand();
