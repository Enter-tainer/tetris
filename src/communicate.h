#pragma once

#include <stdint.h>
#ifdef RISCV
#include "libdevice.h"
#else
#include "garbage.h"
#include <stdio.h>
extern struct GarbageInfo backfire_g;
#endif

static inline void send_data(uint8_t c) {
#ifdef RISCV
  uart_send(c);
#else
  backfire_g = garbage_from_byte(c);
  fprintf(stderr, "send lines: %d at %d\n", backfire_g.lines,
          backfire_g.slot_y);
#endif
}

static inline uint8_t recv_data() {
#ifdef RISCV
  return uart_recv();
#else
  // fprintf(stderr, "%x\n", c);
  uint8_t t = garbage_to_byte(backfire_g);
  fprintf(stderr, "recv %d lines at %d\n", backfire_g.lines, backfire_g.slot_y);
  backfire_g.lines  = 0;
  backfire_g.slot_y = 0;
  return t;
#endif
}

static inline uint8_t send_ready() {
#ifdef RISCV
  return uart_send_ready();
#else
  return 1;
#endif
}
static inline uint8_t recv_ready() {
#ifdef RISCV
  return uart_recv_ready();
#else
  return backfire_g.lines != 0;
#endif
}
