#include "input.h"
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

unsigned char* KEYBOARD_READY_ADDR;
unsigned char* KEYBOARD_DATA_ADDR;
#ifdef RISCV
#define KBD_READY_ADDR ((volatile unsigned char*)0xfbadbeee)
#define KBD_DATA_ADDR ((volatile unsigned char*)0xfbadbeef)
#else
#define BUF_SIZE 32
#define BUF_MASK (BUF_SIZE - 1)
unsigned char key_buffer[32];
unsigned char head = 0;
unsigned char tail = 0;
#define KBD_READY_ADDR KEYBOARD_READY_ADDR
#define KBD_DATA_ADDR KEYBOARD_DATA_ADDR
#endif

#ifndef RISCV
#endif

unsigned char kbd_ready() {
#ifdef RISCV
  return *KBD_READY_ADDR;
#else
  return head != tail;
#endif
}

unsigned char kbd_data() {
#ifdef RISCV
  return *KBD_DATA_ADDR;
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
  while (kbd_ready()) {
    unsigned char cur = kbd_data();
    if (!stop_flag) {
      if (cur == K_LEFT) {
        key->left = 1;
        break;
      }
      if (cur == K_RIGHT_0) {
        right_flag = 1;
        continue;
      }
      if (cur == K_RIGHT_1 && right_flag) {
        key->right = 1;
        right_flag = 0;
        break;
      }
      if (cur == K_UP) {
        key->up = 1;
        break;
      }
      if (cur == K_DOWN) {
        key->down = 1;
        break;
      }
      if (cur == K_Z) {
        key->z = 1;
        break;
      }
      if (cur == K_X) {
        key->x = 1;
        break;
      }
      if (cur == K_C) {
        key->c = 1;
        break;
      }
      if (cur == K_SPACE) {
        key->space = 1;
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
        key->left = 0;
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
        key->up    = 0;
        right_flag = 0;
        stop_flag  = 0;
        break;
      }
      if (cur == K_UP) {
        key->up   = 0;
        stop_flag = 0;
        break;
      }
      if (cur == K_DOWN) {
        key->down = 0;
        stop_flag = 0;
        break;
      }
      if (cur == K_Z) {
        key->z    = 0;
        stop_flag = 0;
        break;
      }
      if (cur == K_X) {
        key->x    = 0;
        stop_flag = 0;
        break;
      }
      if (cur == K_C) {
        key->c    = 0;
        stop_flag = 0;
        break;
      }
      if (cur == K_SPACE) {
        key->space = 0;
        stop_flag  = 0;
        break;
      }
      stop_flag = 0;
    }
  }
  return 0;
}
//#endif