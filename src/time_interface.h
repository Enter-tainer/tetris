#include <stdint.h>
#pragma once
#ifdef RISCV
#include "libdevice.h"
#else
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#endif

inline static uint64_t get_time_in_us() {
#ifdef RISCV
  return time();
#else
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * (uint64_t)1000000 + tv.tv_usec;
#endif
}

inline static void my_sleep(uint32_t us) {
#ifdef RISCV
  sleep(us);
#else
  usleep(us);
#endif
}
