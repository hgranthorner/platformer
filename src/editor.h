#ifndef editor_h
#define editor_h

#include <SDL2/SDL.h>
#include "consts.h"
#include "file.h"

Screen_State edit_level(SDL_Renderer *renderer, Level *level);

#endif
