#ifndef render_h
#define render_h

#include <SDL2/SDL.h>
#include "consts.h"

Screen_State play_level(SDL_Renderer *renderer, char *level);
char *choose_level(SDL_Renderer *renderer);
SDL_Renderer *init_sdl();


#endif
