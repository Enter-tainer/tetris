#ifndef TETRIS_INPUT
#define TETRIS_INPUT

// A couple structs that contain key/mouse button status
// There is a "current" state and an "old" state (previous frame)
struct KeyMap {
  unsigned char up;
  unsigned char down;
  unsigned char left;
  unsigned char right;
  unsigned char z;
  unsigned char x;
  unsigned char c;
  unsigned char shift;
  unsigned char space;
  unsigned char enter;
  unsigned char esc;
};

// Updates the input structs to new values, and also handles SDL events
int input_update();

#endif
