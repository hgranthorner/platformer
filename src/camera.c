#include <SDL2/SDL.h>
#include "player.h"

void move_camera(const Player *player, SDL_Rect *camera)
{
  SDL_Rect r = player->rect.shape;
  if (r.x > camera->x + camera->w * 4/5)
    camera->x = r.x - camera->w * 4/5;

  if (r.x < camera->x + camera->w * 1/5)
    camera->x = r.x - camera->w * 1/5;
}
