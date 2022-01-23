#include "tetris.h"
void spawn_falling_mino(struct FallingMino* current, enum MinoType type) {
  current->state = Original;
  current->type  = type;
  switch (current->type) {
  case IMino:
  case TMino:
  case OMino:
  case JMino:
  case LMino:
  case SMino:
  case ZMino:
    current->x = 20;
    current->y = 4;
    break;
  }
}
void get_cells(struct FallingMino* current, int* x_array, int* y_array) {
  switch (current->type) {
  case IMino:
    switch (current->state) {
    case Original: {
      int dx[] = {0, 0, 0, 0};
      int dy[] = {-1, 1, 1, 2};
      for (int i = 0; i < 4; ++i) {
        x_array[i] = dx[i] + current->x;
        y_array[i] = dy[i] + current->y;
      }
      break;
    }
    case R180: {
      int dx[] = {0, 0, 0, 0};
      int dy[] = {-2, -1, 0, 1};
      for (int i = 0; i < 4; ++i) {
        x_array[i] = dx[i] + current->x;
        y_array[i] = dy[i] + current->y;
      }
      break;
    }
    case Clockwise: {
      int dx[] = {1, 0, -1, -2};
      int dy[] = {0, 0, 0, 0};
      for (int i = 0; i < 4; ++i) {
        x_array[i] = dx[i] + current->x;
        y_array[i] = dy[i] + current->y;
      }
      break;
    }
    case CounterClockwise: {
      int dx[] = {-1, 0, 1, 2};
      int dy[] = {0, 0, 0, 0};
      for (int i = 0; i < 4; ++i) {
        x_array[i] = dx[i] + current->x;
        y_array[i] = dy[i] + current->y;
      }
      break;
    }
    }
    break;
  case TMino: {
    int dx[] = {1, 0, 0, -1};
    int dy[] = {0, 1, -1, 0};
    //            u  r  d   l
    x_array[0] = current->x;
    y_array[0] = current->y;
    for (int i = 0, j = 1; i < 4; ++i) {
      if (i == 0 && current->state == R180) {
        continue;
      }
      if (i == 1 && current->state == CounterClockwise) {
        continue;
      }
      if (i == 2 && current->state == Original) {
        continue;
      }
      if (i == 3 && current->state == Clockwise) {
        continue;
      }
      x_array[j] = current->x + dx[i];
      y_array[j] = current->y + dy[i];
      j++;
    }
    break;
  }
  case OMino: {
    int dx[] = {0, 1, 0, 1};
    int dy[] = {0, 0, 1, 1};
    for (int i = 0; i < 4; ++i) {
      x_array[i] = dx[i] + current->x;
      y_array[i] = dy[i] + current->y;
    }
    break;
  }
  case JMino: {
    switch (current->state) {
    case Original: {
      int dx[] = {0, 0, 0, -1};
      int dy[] = {-1, 0, 1, 1};
      for (int i = 0; i < 4; ++i) {
        x_array[i] = dx[i] + current->x;
        y_array[i] = dy[i] + current->y;
      }
      break;
    }
    case R180: {
      int dx[] = {0, 0, 0, -1};
      int dy[] = {-1, 0, 1, 1};
      for (int i = 0; i < 4; ++i) {
        x_array[i] = dx[i] + current->x;
        y_array[i] = dy[i] + current->y;
      }
      break;
    }
    case Clockwise: {
      int dx[] = {-1, 0, 1, 1};
      int dy[] = {0, 0, 0, 1};
      for (int i = 0; i < 4; ++i) {
        x_array[i] = dx[i] + current->x;
        y_array[i] = dy[i] + current->y;
      }
      break;
    }
    case CounterClockwise: {
      int dx[] = {-1, 0, 1, -1};
      int dy[] = {0, 0, 0, -1};
      for (int i = 0; i < 4; ++i) {
        x_array[i] = dx[i] + current->x;
        y_array[i] = dy[i] + current->y;
      }
      break;
    }
    }
    break;
  }
  case LMino: {
    switch (current->state) {
    case Original: {
      int dx[] = {0, 0, 0, 1};
      int dy[] = {-1, 0, 1, 1};
      for (int i = 0; i < 4; ++i) {
        x_array[i] = dx[i] + current->x;
        y_array[i] = dy[i] + current->y;
      }
      break;
    }
    case R180: {
      int dx[] = {0, 0, 0, -1};
      int dy[] = {-1, 0, 1, -1};
      for (int i = 0; i < 4; ++i) {
        x_array[i] = dx[i] + current->x;
        y_array[i] = dy[i] + current->y;
      }
      break;
    }
    case Clockwise: {
      int dx[] = {-1, 0, 1, -1};
      int dy[] = {0, 0, 0, 1};
      for (int i = 0; i < 4; ++i) {
        x_array[i] = dx[i] + current->x;
        y_array[i] = dy[i] + current->y;
      }
      break;
    }
    case CounterClockwise: {
      int dx[] = {-1, 0, 1, 1};
      int dy[] = {0, 0, 0, -1};
      for (int i = 0; i < 4; ++i) {
        x_array[i] = dx[i] + current->x;
        y_array[i] = dy[i] + current->y;
      }
      break;
    }
    }
    break;
  }
  case SMino: {
    switch (current->state) {
    case Original: {
      int dx[] = {0, 0, 1, 1};
      int dy[] = {0, -1, 0, 1};
      for (int i = 0; i < 4; ++i) {
        x_array[i] = dx[i] + current->x;
        y_array[i] = dy[i] + current->y;
      }
      break;
    }
    case R180: {
      int dx[] = {0, 0, -1, -1};
      int dy[] = {0, 1, 0, -1};
      for (int i = 0; i < 4; ++i) {
        x_array[i] = dx[i] + current->x;
        y_array[i] = dy[i] + current->y;
      }
      break;
    }
    case Clockwise: {
      int dx[] = {0, 1, 0, -1};
      int dy[] = {0, 0, 1, 1};
      for (int i = 0; i < 4; ++i) {
        x_array[i] = dx[i] + current->x;
        y_array[i] = dy[i] + current->y;
      }
      break;
    }
    case CounterClockwise: {
      int dx[] = {0, -1, 0, 1};
      int dy[] = {0, 0, -1, -1};
      for (int i = 0; i < 4; ++i) {
        x_array[i] = dx[i] + current->x;
        y_array[i] = dy[i] + current->y;
      }
      break;
    }
    }
    break;
  };
  case ZMino: {
    switch (current->state) {
    case Original: {
      int dx[] = {0, 0, 1, 1};
      int dy[] = {0, 1, 0, -1};
      for (int i = 0; i < 4; ++i) {
        x_array[i] = dx[i] + current->x;
        y_array[i] = dy[i] + current->y;
      }
      break;
    }
    case R180: {
      int dx[] = {0, 0, -1, -1};
      int dy[] = {0, -1, 0, 1};
      for (int i = 0; i < 4; ++i) {
        x_array[i] = dx[i] + current->x;
        y_array[i] = dy[i] + current->y;
      }
      break;
    }
    case Clockwise: {
      int dx[] = {0, -1, 0, 1};
      int dy[] = {0, 0, 1, 1};
      for (int i = 0; i < 4; ++i) {
        x_array[i] = dx[i] + current->x;
        y_array[i] = dy[i] + current->y;
      }
      break;
    }
    case CounterClockwise: {
      int dx[] = {0, 1, 0, -1};
      int dy[] = {0, 0, -1, -1};
      for (int i = 0; i < 4; ++i) {
        x_array[i] = dx[i] + current->x;
        y_array[i] = dy[i] + current->y;
      }
      break;
    }
    }
    break;
  }
  }
}
