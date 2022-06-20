#ifndef rect_h
#define rect_h

#include <SDL2/SDL.h>

typedef struct
{
  int r;
  int g;
  int b;
  int a;
} Color;

typedef struct
{
  SDL_Rect shape;
  Color color;
  int damaging;
} Rect;

typedef struct
{
  Rect *rects;
  int size;
  int capacity;
} Rects;

Rect create_rect(int x, int y, int w, int h, int r, int g, int b, int a, int damaging);
void render_fill_rect(SDL_Renderer *renderer, const SDL_Rect *camera, const Rect *rect);
void render_line_rect(SDL_Renderer *renderer, const SDL_Rect *camera, const Rect *rect);
void add_rect(Rects *rects, Rect rect);

#endif
