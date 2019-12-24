#ifndef line_h
#define line_h

#include <SDL2/SDL.h>

#include "rect.h"

struct Line
{
  int x1;
  int y1;
  int x2;
  int y2;
};

static inline
struct Line get_bottom(const Rect rect)
{
  SDL_Rect r = rect.shape;
  struct Line line = { .x1 = r.x,
                       .y1 = r.y + r.h,
                       .x2 = r.x + r.w,
                       .y2 = r.y + r.h };
  return line;
}

#endif
