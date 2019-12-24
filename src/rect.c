#include <SDL2/SDL.h>
#include <stdlib.h>

#include "rect.h"

Rect create_rect(int x, int y, int w, int h, int r, int g, int b, int a)
{
  SDL_Rect sdl_rect = { x, y, w, h };
  Color c = { r, g, b, a };
  Rect rect = { sdl_rect, c };
  return rect;
}

void render_fill_rect (SDL_Renderer *renderer, const SDL_Rect *camera, const Rect *rect)
{
  if (SDL_HasIntersection(camera, &rect->shape) == SDL_TRUE)
  {
    Color c = rect->color;

    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
    SDL_RenderFillRect(renderer, &rect->shape);
  }
}

void render_line_rect (SDL_Renderer *renderer, const SDL_Rect *camera, const Rect *rect)
{
  if (SDL_HasIntersection(camera, &rect->shape) == SDL_TRUE)
  {
    Color c = rect->color;

    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
    SDL_RenderDrawRect(renderer, &rect->shape);
  }
}
