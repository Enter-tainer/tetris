#ifndef TETRIS_GRAPHICS
#define TETRIS_GRAPHICS

void graphics_init(int x, int y);

void graphics_flip();

void graphics_draw_rect(int x, int y, int w, int h, unsigned char block_type);

void graphics_fill_rect(int x, int y, int w, int h, unsigned char block_type);

void graphics_draw_line(int x1, int y1, int x2, int y2,
                        unsigned char block_type);

#endif
