#ifndef player_h
#define player_h

#include "rect.h"

typedef struct
{
  SDL_Scancode right;
  SDL_Scancode left;
  SDL_Scancode jump;
} Controls;

typedef struct
{
  Rect rect;
  Controls controls;
  int x_velocity;
  int y_velocity;
} Player;

static inline
void move_player_position(Player *player)
{
  player->rect.shape.x += player->x_velocity;
  player->rect.shape.y += player->y_velocity;
}

void apply_gravity(Player *player, const Rects *rects);

#endif
