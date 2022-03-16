#include "libdevice.h"
// top of stack
extern unsigned __stacktop;
// initial stack pointer is first address of program
#ifdef RISCV
__attribute__((section(".stack"), used)) unsigned* __stack_init = &__stacktop;

int main();

__attribute__((section(".text.start"))) __attribute__((naked)) void _start() {
  asm("mv sp, %0\n\t" ::"r"(&__stacktop));
  asm("j %0\n\t" ::"i"(&main));
}
#endif

uint32_t time() { return *CLOCK_ADDR; }

void sleep(uint32_t us) {
  uint32_t start = *CLOCK_ADDR;
  while (1) {
    if (*CLOCK_ADDR >= start + us) {
      break;
    }
  }
}

void set_led(uint32_t val) { *LED_ADDR = val; }

void set_vram(int x, int y, uint8_t pixel) {
  VRAM_ADDR[x * VRAM_Y + y] = pixel;
}

uint8_t kbd_ready() { return *KBD_READY_ADDR; }

uint8_t kbd_get() { return *KBD_DATA_ADDR; }
