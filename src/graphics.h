#ifndef TETRIS_GRAPHICS
#define TETRIS_GRAPHICS

#define COLOR_BACKGROUND  0xffffefff
#define COLOR_RED    0xef6f4fff
#define COLOR_GREEN  0x6fcf5fff
#define COLOR_BLUE   0x1f8fbfff
#define COLOR_CYAN   0x4fafdfff
#define COLOR_YELLOW 0xffdf3fff
#define COLOR_PURPLE 0xbf5fafff
#define COLOR_ORANGE 0xef9f3fff
#define COLOR_WHITE  0xFFFFFFFF
#define COLOR_SHADOW 0x6f6f6fFF
#define COLOR_GRAY_DARK 0xAAAAAAFF

void graphics_init(int x, int y);

void graphics_load_font(const char *filename);

void graphics_quit();

void graphics_flip();

void graphics_set_color(unsigned int color);

void graphics_draw_rect(int x, int y, int w, int h);

void graphics_fill_rect(int x, int y, int w, int h);

void graphics_draw_line(int x1, int y1, int x2, int y2);

void graphics_draw_string(char *string, int x, int y);

int graphics_string_width(char *string);

void graphics_draw_int(int n, int x, int y);

#endif
