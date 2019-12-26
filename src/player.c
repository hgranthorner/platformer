#include <SDL2/SDL.h>
#include <stdlib.h>

#include "player.h"
#include "line.h"
#include "consts.h"

void apply_gravity(Player *player, const SDL_Rect *camera, const Rects *rects)
{
  int landed = 0;
  for (int i = 0; i < rects->size; ++i)
  {
    const SDL_Rect r = rects->rects[i].shape;
    const SDL_Rect ps = player->rect.shape;
    const SDL_Rect pr = { .x = ps.x,
                          .y = ps.y,
                          .w = ps.w,
                          .h = ps.h + 1 };
    SDL_Rect res;
    SDL_bool intersects = SDL_IntersectRect(&pr, &r, &res);
    if (intersects == SDL_TRUE)
    {
      if (res.h > res.w)
      {
        if (ps.x < r.x)
          player->rect.shape.x = r.x - ps.w;
        else
          player->rect.shape.x = r.x + r.w;
         
        player->x_velocity = 0;
      }
      else
      {
        landed = 1;
        player->rect.shape.y = r.y - player->rect.shape.h;
        player->y_velocity = 0;        
      }
      player->jumps_remaining = 2;
      if (rects->rects[i].damaging > 0)
      {
        reset_player_position(player, camera);
      }
    }
  }

  if (landed == 0)
  {
    player->y_velocity += player->y_velocity > TERMINAL_VELOCITY
      ? 0
      : 1;    
  }
}

void reset_player_position(Player *player, const SDL_Rect *camera)
{
  player->rect.shape.x = camera->w / 2 + camera->x;
  player->rect.shape.y = camera->h / 2 + camera->y;
}
