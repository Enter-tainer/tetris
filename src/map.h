#pragma once

#define MAP_WIDTH 80
#define MAP_HEIGHT 60
#define MAP_SIZE (MAP_WIDTH * MAP_HEIGHT)
#define idx(x, y) ((y)*MAP_WIDTH + (x))

extern unsigned char* map;