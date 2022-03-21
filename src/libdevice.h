#pragma once
#include <stdint.h>
#define CLOCK_ADDR ((volatile uint32_t*)0xfbadbedf)
#define LED_ADDR ((volatile uint32_t*)0xfbadc0fe)
#define VRAM_ADDR ((volatile uint8_t*)0xfbad0000)
#define VRAM_COMMIT_ADDR ((volatile uint8_t*)0xfbadf000)
#define KBD_READY_ADDR ((volatile uint8_t*)0xfbadbeee)
#define KBD_DATA_ADDR ((volatile uint8_t*)0xfbadbeef)
#define UART_RECV_READY_ADDR ((volatile uint8_t*)0xfbada000)
#define UART_SEND_READY_ADDR ((volatile uint8_t*)0xfbada001)
#define UART_RECV_DATA_ADDR ((volatile uint8_t*)0xfbada002)
#define UART_SEND_DATA_ADDR ((volatile uint8_t*)0xfbada003)
#define VRAM_X 60 // X is vertical
#define VRAM_Y 80 // Y is horizontal
uint32_t time();

void sleep(uint32_t us);

void set_led(uint32_t val);

void set_vram(int x, int y, uint8_t pixel);

void commit_vram();

uint8_t keyboard_ready();

uint8_t keyboard_data();

uint8_t uart_recv_ready();
uint8_t uart_send_ready();
uint8_t uart_recv();
void uart_send(uint8_t data);

void* memset(void* dest, int c, unsigned long n);
void* memcpy(void* dest, const void* src, unsigned long n);
