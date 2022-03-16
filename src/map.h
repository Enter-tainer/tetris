#pragma once

#define MAP_WIDTH 160
#define MAP_HEIGHT 120
#define MAP_SIZE (MAP_WIDTH * MAP_HEIGHT)
#define idx(x, y) ((y) * MAP_WIDTH + (x))

extern unsigned char* map;