#pragma once
#include "tetris.h"
#include <stdint.h>
struct XorShift {
  uint32_t a;
};
uint32_t xorshift32(struct XorShift* state);
uint32_t my_rand();
