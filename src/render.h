#ifndef render_h
#define render_h

#include <SDL2/SDL.h>
#include "consts.h"

typedef struct {
  Screen_State next_screen;
  char *level;
} Select_Level_Result;

Screen_State play_level(SDL_Renderer *renderer, char *level);
Select_Level_Result select_level(SDL_Renderer *renderer);
SDL_Renderer *init_sdl();


#endif
