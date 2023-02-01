#include <SDL2/SDL.h>
#include <stdlib.h>

#include "player.h"
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
      // When the rectangles are overlapping on their sides
      if (res.h > res.w)
      {
        if (ps.x < r.x)
          player->rect.shape.x = r.x - ps.w;
        else
          player->rect.shape.x = r.x + r.w;

        player->x_velocity = 0;
      }
      // When the top and bottoms of the rectangles are overlapping
      else if (res.w > res.h)
      {
        // If res.y == ps.y, then the overlapping rectangle is the top part of the player
        // If not, then it's the bottom part of the player
        // When the player is above the intersecting rectangle
        if (res.y != ps.y && player->y_velocity >= 0)
        {
          landed = 1;
          player->rect.shape.y = r.y - player->rect.shape.h;
          player->y_velocity = 0;
        }
        // When hitting the rect from below and moving upward
        else if (res.y == ps.y && player->y_velocity < 0)
        {
          // printf("Hitting from below!\n");
          // printf("Rectangle y: %d, intersection y: %d, player y: %d, player y velocity: %d\n", r.y, res.y, ps.y, player->y_velocity);
          player->rect.shape.y = res.y + res.h;
          player->y_velocity = 0;
        }
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

Player create_player(Rect rect)
{
    Controls controls = { .right = SDL_SCANCODE_RIGHT,
                        .left = SDL_SCANCODE_LEFT,
                        .jump = SDL_SCANCODE_UP };
    Player player = { .rect = rect,
                      .controls = controls,
                      .x_velocity = 0,
                      .y_velocity = 0,
                      .jumps_remaining = 2};
  return player;
}
