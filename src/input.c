#include "input.h"
#include <stdio.h>
struct KeyMap key, oldKey;

#ifdef X86
const int K_LEFT   = SDL_SCANCODE_LEFT;
const int K_RIGHT  = SDL_SCANCODE_RIGHT;
const int K_UP     = SDL_SCANCODE_UP;
const int K_DOWN   = SDL_SCANCODE_DOWN;
const int K_Z      = SDL_SCANCODE_Z;
const int K_X      = SDL_SCANCODE_X;
const int K_C      = SDL_SCANCODE_C;
const int K_SHIFT  = SDL_SCANCODE_LSHIFT;
const int K_SPACE  = SDL_SCANCODE_SPACE;
const int K_RETURN = SDL_SCANCODE_RETURN;
const int K_ESC    = SDL_SCANCODE_ESCAPE;

int input_update() {
  SDL_Event event;
  while (SDL_WaitEvent(&event)) {
	  if (event.type >= SDL_KEYDOWN && event.type <= SDL_KEYUP) {
		  break;
	  }
	  if (event.type == SDL_QUIT) {
		  return 1;
	  }
  }
	oldKey.left = key.left;
	oldKey.right = key.right;
	oldKey.up = key.up;
	oldKey.down = key.down;
	oldKey.z = key.z;
	oldKey.x = key.x;
	oldKey.c = key.c;
	oldKey.shift = key.shift;
	oldKey.space = key.space;
	oldKey.enter = key.enter;
	oldKey.esc = key.esc;
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	key.up = state[K_UP];
	key.down = state[K_DOWN];
	key.left = state[K_LEFT];
	key.right = state[K_RIGHT];
	key.z = state[K_Z];
	key.x = state[K_X];
	key.c = state[K_C];
	key.shift = state[K_SHIFT];
	key.space = state[K_SPACE];
	key.enter = state[K_RETURN];
	key.esc = state[K_ESC];
	return 0;
}
#endif

#ifdef RV

const uint8_t K_LEFT   = 0x6b;
const uint8_t K_RIGHT[2]  = {0xe0, 0x74};
const uint8_t K_UP     = 0x75;
const uint8_t K_DOWN   = 0x72;
const uint8_t K_Z      = 0x1a;
const uint8_t K_X      = 0x22;
const uint8_t K_C      = 0x21;
const uint8_t K_SPACE  = 0x29;


int input_update_rv() {
  SDL_Event event;
  while (!*KEYBOARD_READY_ADDR) {
  }
	oldKey.left = key.left;
	oldKey.right = key.right;
	oldKey.up = key.up;
	oldKey.down = key.down;
	oldKey.z = key.z;
	oldKey.x = key.x;
	oldKey.c = key.c;
	oldKey.shift = key.shift;
	oldKey.space = key.space;
	oldKey.enter = key.enter;
	oldKey.esc = key.esc;
	bool stop_flag=0;
	bool right_flag=0;
	bool state[8]={0,0,0,0,0,0,0,0}; 
	while(*KEYBOARD_READY_ADDR){
		cur=*KEYBOARD_READY_ADDR;
		if(!stop_flag){
			if(cur==K_LEFT){state[0]=1;break;}
			if(cur==K_RIGHT[0]){right_flag=1;continue;}
			if(cur==K_RIGHT[1]&&right_flag){state[1]=1;right_flag=0;break;}
			if(cur==K_UP){state[2]=1;break;}
			if(cur==K_DOWN){state[3]=1;break;}
			if(cur==K_Z){state[4]=1;break;}
			if(cur==K_X){state[5]=1;break;}
			if(cur==K_C){state[6]=1;break;}
			if(cur==K_SPACE){state[7]=1;break;}
			if(cur==0xf0){stop_flag=1;continue;}
		}
		else{
			if(cur==K_LEFT){state[0]=0;stop_flag=0;break;}
			if(cur==K_RIGHT[0]){right_flag=1;continue;}
			if(cur==K_RIGHT[1]&&right_flag){state[1]=1;right_flag=0;stop_flag=0;break;}
			if(cur==K_UP){state[2]=0;stop_flag=0;break;}
			if(cur==K_DOWN){state[3]=0;stop_flag=0;break;}
			if(cur==K_Z){state[4]=0;stop_flag=0;break;}
			if(cur==K_X){state[5]=0;stop_flag=0;break;}
			if(cur==K_C){state[6]=0;stop_flag=0;break;}
			if(cur==K_SPACE){state[7]=0;stop_flag=0;break;}
			stop_flag=0;
		}
	}
	key.up = state[2];
	key.down = state[3];
	key.left = state[0];
	key.right = state[2];
	key.z = state[4];
	key.x = state[5];
	key.c = state[6];
	key.space = state[7];
	return 0;
}
//#endif