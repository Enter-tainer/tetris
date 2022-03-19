#include "input.h"
#include "libdevice.h"
#include <stdint.h>
#ifndef RISCV
#include "sdl_interface.h"
#endif
#define K_ARROW_PREFIX 0xe0
#define K_RELEASE_PREFIX 0xf0

#define K_LEFT 0x6b
#define K_RIGHT 0x74
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
    return 0;
  }
  int ret = 0;
  enum __attribute__((__packed__)) AutomataStatus {
    Start,
    BeforeReleaseKeys,
    Arrows,
    BeforeReleaseArrows,
  };
  enum AutomataStatus status = Start;
  while (kbd_ready()) {
    uint8_t cur = kbd_data();
    switch (status) {
    case Start:
      switch (cur) {
      case K_ARROW_PREFIX:
        status = Arrows;
        break;
      case K_RELEASE_PREFIX:
        status = BeforeReleaseKeys;
        break;
      case K_Z:
      case K_X:
      case K_C:
      case K_SPACE:
      case K_UP:
      case K_DOWN:
      case K_LEFT:
      case K_RIGHT:
        ret |= 1;
        if (cur == K_Z) {
          key->z = 1;
        }
        if (cur == K_X) {
          key->x = 1;
        }
        if (cur == K_C) {
          key->c = 1;
        }
        if (cur == K_SPACE) {
          key->space = 1;
        }
        if (cur == K_UP) {
          key->up = 1;
        }
        if (cur == K_DOWN) {
          key->down = 1;
        }
        if (cur == K_LEFT) {
          key->left = 1;
          key->right = 0;
        }
        if (cur == K_RIGHT) {
          key->right = 1;
          key->left  = 0;
        }
        status = Start;
        break;
      default:
        status = Start;
        break;
      }
      break;
    case BeforeReleaseKeys:
      switch (cur) {
      case K_Z:
      case K_X:
      case K_C:
      case K_SPACE:
      case K_UP:
      case K_DOWN:
      case K_LEFT:
      case K_RIGHT:
        ret |= 2;
        if (cur == K_Z) {
          key->z = 0;
        }
        if (cur == K_X) {
          key->x = 0;
        }
        if (cur == K_C) {
          key->c = 0;
        }
        if (cur == K_SPACE) {
          key->space = 0;
        }
        if (cur == K_UP) {
          key->up = 0;
        }
        if (cur == K_DOWN) {
          key->down = 0;
        }
        if (cur == K_LEFT) {
          key->left = 0;
        }
        if (cur == K_RIGHT) {
          key->right = 0;
        }
        status = Start;
        break;
      }
      break;
    case Arrows:
      switch (cur) {
      case K_RELEASE_PREFIX:
        status = BeforeReleaseArrows;
        break;
      case K_LEFT:
      case K_RIGHT:
      case K_UP:
      case K_DOWN:
        ret |= 1;
        if (cur == K_UP) {
          key->up = 1;
        }
        if (cur == K_DOWN) {
          key->down = 1;
        }
        if (cur == K_LEFT) {
          key->left  = 1;
          key->right = 0;
        }
        if (cur == K_RIGHT) {
          key->right = 1;
          key->left  = 0;
        }
        status = Start;
        break;
      default:
        status = Start;
        break;
      }
      break;
    case BeforeReleaseArrows:
      switch (cur) {
      case K_LEFT:
      case K_RIGHT:
      case K_UP:
      case K_DOWN:
        ret |= 2;
        if (cur == K_UP) {
          key->up = 0;
        }
        if (cur == K_DOWN) {
          key->down = 0;
        }
        if (cur == K_LEFT) {
          key->left = 0;
        }
        if (cur == K_RIGHT) {
          key->right = 0;
        }
        status = Start;
        break;
      default:
        status = Start;
        break;
      }
      break;
    }
  }
  return ret;
}

void wait_any_key_down(struct KeyMap* key) {
  while (1) {
    int res = input_update(key);
    memset(key, 0, sizeof(struct KeyMap));
    if ((res & 1) == 1) {
      return;
    }
  }
}
