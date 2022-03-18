#pragma once
#include <stdint.h>
#define CLOCK_ADDR ((volatile uint32_t*)0xfbadbedf)
#define LED_ADDR ((volatile uint32_t*)0xfbadc0fe)
#define VRAM_ADDR ((volatile uint8_t*)0xfbad0000)
#define KBD_READY_ADDR ((volatile uint8_t*)0xfbadbeee)
#define KBD_DATA_ADDR ((volatile uint8_t*)0xfbadbeef)
#define VRAM_X 60 // X is vertical
#define VRAM_Y 80 // Y is horizontal
uint32_t time();

void sleep(uint32_t us);

void set_led(uint32_t val);

void set_vram(int x, int y, uint8_t pixel);

uint8_t keyboard_ready();

uint8_t keyboard_data();

void* memset(void* dest, int c, unsigned long n);
