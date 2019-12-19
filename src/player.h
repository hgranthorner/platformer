#ifndef player_h
#define player_h

#include "rect.h"

struct Controls
{
  SDL_Scancode right;
  SDL_Scancode left;
  SDL_Scancode jump;
};

struct Player
{
  struct Rect rect;
  struct Controls controls;
  int x_velocity;
  int y_velocity;
};

static inline
void move_player_position(struct Player *player)
{
  player->rect.shape.x += player->x_velocity;
  player->rect.shape.y += player->y_velocity;
}

void apply_gravity(struct Player *player, const struct Rects *rects);

#endif
