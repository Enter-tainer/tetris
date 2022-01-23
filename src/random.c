#include "random.h"
#include "tetris.h"
#include <stdint.h>
uint32_t xorshift32(struct XorShift* state) {
  /* Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs" */
  uint32_t x = state->a;
  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 5;
  return state->a = x;
}

uint32_t my_rand() {
  static struct XorShift t = {.a = 123};
  return xorshift32(&t);
}
