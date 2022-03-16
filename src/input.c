#include "input.h"
#ifndef RISCV
#include "sdl_interface.h"
#endif

struct KeyMap key;

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

int input_update() {
#ifndef RISCV
  sdl_wait_key(&key_buffer,&head,&tail);
#endif
  // no block
  if (!kbd_ready()) {
    return -1;
  }
  unsigned char stop_flag  = 0;
  unsigned char right_flag = 0;
  unsigned char state[8]   = {0, 0, 0, 0, 0, 0, 0, 0};
  while (kbd_ready()) {
    unsigned char cur = kbd_data();
    if (!stop_flag) {
      if (cur == K_LEFT) {
        state[0] = 1;
        break;
      }
      if (cur == K_RIGHT_0) {
        right_flag = 1;
        continue;
      }
      if (cur == K_RIGHT_1 && right_flag) {
        state[1]   = 1;
        right_flag = 0;
        break;
      }
      if (cur == K_UP) {
        state[2] = 1;
        break;
      }
      if (cur == K_DOWN) {
        state[3] = 1;
        break;
      }
      if (cur == K_Z) {
        state[4] = 1;
        break;
      }
      if (cur == K_X) {
        state[5] = 1;
        break;
      }
      if (cur == K_C) {
        state[6] = 1;
        break;
      }
      if (cur == K_SPACE) {
        state[7] = 1;
        break;
      }
      if (cur == 0xf0) {
        stop_flag = 1;
        continue;
      }
    } else {
      if (cur == K_LEFT) {
        state[0]  = 0;
        stop_flag = 0;
        break;
      }
      if (cur == K_RIGHT_0) {
        right_flag = 1;
        continue;
      }
      if (cur == K_RIGHT_1 && right_flag) {
        state[1]   = 0;
        right_flag = 0;
        stop_flag  = 0;
        break;
      }
      if (cur == K_UP) {
        state[2]  = 0;
        stop_flag = 0;
        break;
      }
      if (cur == K_DOWN) {
        state[3]  = 0;
        stop_flag = 0;
        break;
      }
      if (cur == K_Z) {
        state[4]  = 0;
        stop_flag = 0;
        break;
      }
      if (cur == K_X) {
        state[5]  = 0;
        stop_flag = 0;
        break;
      }
      if (cur == K_C) {
        state[6]  = 0;
        stop_flag = 0;
        break;
      }
      if (cur == K_SPACE) {
        state[7]  = 0;
        stop_flag = 0;
        break;
      }
      stop_flag = 0;
    }
  }
  key.up    = state[2];
  key.down  = state[3];
  key.left  = state[0];
  key.right = state[1];
  key.z     = state[4];
  key.x     = state[5];
  key.c     = state[6];
  key.space = state[7];
  return 0;
}
//#endif