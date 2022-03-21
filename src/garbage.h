#pragma once
#include <stdbool.h>
#include <stdint.h>
struct __attribute__((__packed__)) GarbageInfo {
  unsigned char slot_y : 4;
  unsigned char lines : 4;
};

static inline uint8_t garbage_to_byte(struct GarbageInfo t) {
  return t.lines | (t.slot_y << 4);
}

static inline struct GarbageInfo garbage_from_byte(uint8_t t) {
  struct GarbageInfo tmp = {
      .lines  = t & 0x0f,
      .slot_y = ((t & 0xf0) >> 4),
  };
  return tmp;
}

#define GARBAGE_QUEUE_LENGTH 32
struct GarbageQueue {
  struct GarbageInfo data[GARBAGE_QUEUE_LENGTH];
  uint8_t back_ptr, front_ptr;
  // [front_ptr, back_ptr)
};

void garbage_queue_init(struct GarbageQueue* t);
bool garbage_queue_empty(struct GarbageQueue* t);
bool garbage_queue_full(struct GarbageQueue* t);
// remember to check if queue is full before pushing data in queue
void garbage_queue_push_back(struct GarbageQueue* t, struct GarbageInfo d);
// remember to check if queue is full before pushing data in queue
void garbage_queue_push_front(struct GarbageQueue* t, struct GarbageInfo d);
// remember to check if queue is empty before poping data from queue
struct GarbageInfo garbage_queue_pop_back(struct GarbageQueue* t);
// remember to check if queue is empty before poping data from queue
struct GarbageInfo garbage_queue_pop_front(struct GarbageQueue* t);
