#include "garbage.h"

void garbage_queue_init(struct GarbageQueue* t) {
  t->back_ptr = t->front_ptr = 0;
}
bool garbage_queue_empty(struct GarbageQueue* t) {
  return t->back_ptr == t->front_ptr;
}
bool garbage_queue_full(struct GarbageQueue* t) {
  return (t->back_ptr + 1) % GARBAGE_QUEUE_LENGTH == t->front_ptr;
}
void garbage_queue_push_back(struct GarbageQueue* t, struct GarbageInfo d) {
  t->data[t->back_ptr] = d;
  t->back_ptr = (t->back_ptr + 1) % GARBAGE_QUEUE_LENGTH;
}
void garbage_queue_push_front(struct GarbageQueue* t, struct GarbageInfo d) {
  t->front_ptr          = (t->front_ptr - 1 + GARBAGE_QUEUE_LENGTH) % GARBAGE_QUEUE_LENGTH;
  t->data[t->front_ptr] = d;
}
struct GarbageInfo garbage_queue_pop_back(struct GarbageQueue* t) {
  t->back_ptr = (t->back_ptr - 1 + GARBAGE_QUEUE_LENGTH) % GARBAGE_QUEUE_LENGTH;
  struct GarbageInfo res = t->data[t->back_ptr];
  return res;
}
struct GarbageInfo garbage_queue_pop_front(struct GarbageQueue* t) {
  struct GarbageInfo res = t->data[t->front_ptr];
  t->front_ptr           = (t->front_ptr + 1) % GARBAGE_QUEUE_LENGTH;
  return res;
}
