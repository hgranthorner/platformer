#include <SDL2/SDL.h>
#include <stdlib.h>

#include "rect.h"

Rect create_rect(int x, int y, int w, int h, int r, int g, int b, int a, int damaging)
{
  SDL_Rect sdl_rect = { x, y, w, h };
  Color c = { r, g, b, a };
  Rect rect = { sdl_rect, c, damaging };
  return rect;
}

void render_fill_rect(SDL_Renderer *renderer, const SDL_Rect *camera, const Rect *rect)
{
  if (SDL_HasIntersection(camera, &rect->shape) == SDL_TRUE)
  {
    Color c = rect->color;

    const SDL_Rect r = { .x = rect->shape.x - camera->x,
                         .y = rect->shape.y - camera->y,
                         .w = rect->shape.w,
                         .h = rect->shape.h };
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
    SDL_RenderFillRect(renderer, &r);
  }
}

void render_line_rect(SDL_Renderer *renderer, const SDL_Rect *camera, const Rect *rect)
{
  if (SDL_HasIntersection(camera, &rect->shape) == SDL_TRUE)
  {
    Color c = rect->color;

    const SDL_Rect r = { .x = rect->shape.x - camera->x,
                         .y = rect->shape.y - camera->y,
                         .w = rect->shape.w,
                         .h = rect->shape.h };
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
    SDL_RenderDrawRect(renderer, &r);
  }
}

void add_rect(Rects *rects, Rect rect)
{
  if (rects->size == rects->capacity)
  {
    rects->capacity = rects->capacity * 2;
    rects->rects = realloc(rects->rects, sizeof(Rect) * rects->capacity);
  }

  rects->rects[rects->size] = rect;
  rects->size++;
}
