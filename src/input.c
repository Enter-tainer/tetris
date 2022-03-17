#include "input.h"
#include "libdevice.h"
#ifndef RISCV
#include "sdl_interface.h"
#endif

#define K_LEFT 0x6b
#define K_RIGHT_0 0xe0
#define K_RIGHT_1 0x74
#define K_UP 0x75
#define K_DOWN 0x72
#define K_Z 0x1a
#define K_X 0x22
#define K_C 0x21
#define K_SPACE 0x29

#ifndef RISCV
#define BUF_SIZE 32
#define BUF_MASK (BUF_SIZE - 1)
unsigned char key_buffer[32];
unsigned char head = 0;
unsigned char tail = 0;
#endif

#ifndef RISCV
#endif

unsigned char kbd_ready() {
#ifdef RISCV
  return keyboard_ready();
#else
  return head != tail;
#endif
}

unsigned char kbd_data() {
#ifdef RISCV
  return keyboard_data();
#else
  unsigned char ret = key_buffer[head];

  head = (head + 1) & BUF_MASK;
  return ret;
#endif
}

int input_update(struct KeyMap* key) {
#ifndef RISCV
  sdl_wait_key(&key_buffer, &head, &tail);
#endif
  // no block
  if (!kbd_ready()) {
    return -1;
  }
  unsigned char stop_flag  = 0;
  unsigned char right_flag = 0;
  int ret;
  while (kbd_ready()) {
    unsigned char cur = kbd_data();
    if (!stop_flag) {
      if (cur == K_LEFT) {
        if (key)
          key->left = 1;
        ret |= 1;
        break;
      }
      if (cur == K_RIGHT_0) {
        right_flag = 1;
        continue;
      }
      if (cur == K_RIGHT_1 && right_flag) {
        if (key)
          key->right = 1;
        ret |= 1;
        right_flag = 0;
        break;
      }
      if (cur == K_UP) {
        if (key)
          key->up = 1;
        ret |= 1;
        break;
      }
      if (cur == K_DOWN) {
        if (key)
          key->down = 1;
        ret |= 1;
        break;
      }
      if (cur == K_Z) {
        if (key)
          key->z = 1;
        ret |= 1;
        break;
      }
      if (cur == K_X) {
        if (key)
          key->x = 1;
        ret |= 1;
        break;
      }
      if (cur == K_C) {
        if (key)
          key->c = 1;
        ret |= 1;
        break;
      }
      if (cur == K_SPACE) {
        if (key)
          key->space = 1;
        ret |= 1;
        break;
      }
      if (cur == 0xf0) {
        if (right_flag == 1) {
          right_flag = 0;
        }
        stop_flag = 1;
        continue;
      }
    } else {
      if (cur == K_LEFT) {
        if (key)
          key->left = 0;
        ret |= 2;
        stop_flag = 0;
        break;
      }
      if (cur == K_RIGHT_0) {
        if (right_flag == 1) {
          stop_flag = 0;
        }
        right_flag = 1;
        continue;
      }
      if (cur == K_RIGHT_1 && right_flag) {
        if (key)
          key->up = 0;
        ret |= 2;
        right_flag = 0;
        stop_flag  = 0;
        break;
      }
      if (cur == K_UP) {
        if (key)
          key->up = 0;
        ret |= 2;
        stop_flag = 0;
        break;
      }
      if (cur == K_DOWN) {
        if (key)
          key->down = 0;
        ret |= 2;
        stop_flag = 0;
        break;
      }
      if (cur == K_Z) {
        if (key)
          key->z = 0;
        ret |= 2;
        stop_flag = 0;
        break;
      }
      if (cur == K_X) {
        if (key)
          key->x = 0;
        ret |= 2;
        stop_flag = 0;
        break;
      }
      if (cur == K_C) {
        if (key)
          key->c = 0;
        ret |= 2;
        stop_flag = 0;
        break;
      }
      if (cur == K_SPACE) {
        if (key)
          key->space = 0;
        ret |= 2;
        stop_flag = 0;
        break;
      }
      stop_flag = 0;
    }
  }
  return ret;
}

void wait_any_key_down() {
  while (1) {
    int ret = input_update(0);
    if (ret != -1 && (ret & 1))
      return;
  }
}