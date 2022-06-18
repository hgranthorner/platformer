#ifndef render_h
#define render_h

#include <SDL2/SDL.h>

void play_level(SDL_Renderer *renderer);
char *choose_level(SDL_Renderer *renderer);
SDL_Renderer *init_sdl();


#endif
