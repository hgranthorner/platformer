#ifndef camera_h
#define camera_h

#include <SDL2/SDL.h>
#include "player.h"

void move_camera(const Player *player, SDL_Rect *camera);

#endif
