#include <SDL2/SDL.h>
#include "player.h"
#include "consts.h"

void move_camera(const Player *player, SDL_Rect *camera)
{
  SDL_Rect r = player->rect.shape;
  if (r.x > camera->x + camera->w * 3/4)
    camera->x = r.x - camera->w * 3/4;

  if (r.x < camera->x + camera->w * 1/4)
    camera->x = r.x - camera->w * 1/4;

  if (r.y > camera->y + camera->h * 7/8)
    camera->y = r.y - camera->h * 7/8;

  if (r.y < camera->y + camera->h * 1/8)
    camera->y = r.y - camera->h * 1/8;
}
