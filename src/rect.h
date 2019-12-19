#ifndef rect_h
#define rect_h

#include <SDL2/SDL.h>

struct Color
{
  int r;
  int g;
  int b;
  int a;
};

struct Rect
{
  SDL_Rect shape;
  struct Color color;
};

struct Rects
{
  struct Rect *rects;
  int size;
};

struct Rect create_rect(int x, int y, int w, int h, int r, int g, int b, int a);
void render_fill_rect (SDL_Renderer *renderer, const struct Rect *rect);
void render_line_rect (SDL_Renderer *renderer, const struct Rect *rect);

#endif
