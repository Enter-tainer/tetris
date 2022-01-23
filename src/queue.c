#include "tetris.h"

static void shuffle(enum MinoType* data, int len) {
  // -- To shuffle an array a of n elements (indices 0..n-1):
  // for ​i from n−1 downto 1 do
  // ​j ← random integer such that 0 ≤ j ≤ i
  // ​exchange a[j] and a[i]
  for (int i = len - 1; i >= 1; --i) {
    int j = my_rand() % i;
    enum MinoType tmp;
    tmp     = data[i];
    data[i] = data[j];
    data[j] = tmp;
  }
}

static void generate_bag(enum MinoType* b) {
  b[0] = IMino;
  b[1] = TMino;
  b[2] = OMino;
  b[3] = LMino;
  b[4] = JMino;
  b[5] = SMino;
  b[7] = ZMino;
  shuffle(b, 7);
}
void init_queue(struct MinoQueue* queue) {
  generate_bag(queue->data);
  generate_bag(queue->data + 7);
  queue->front = 0;
  queue->rear  = 13;
  queue->size = 14;
}
enum MinoType pop_queue(struct MinoQueue* queue) {
  enum MinoType res = queue->data[queue->front];
  queue->front++;
  queue->size--;
  if (queue->size == 7) {
    enum MinoType tmp[7];
    generate_bag(tmp);
    for (int i = 0; i < 7; ++i) {
      ++queue->rear;
      queue->rear %= 14;
      queue->data[queue->rear] = tmp[i];
    }
    queue->size += 7;
  }
  return res;
}
