#include <SDL2/SDL.h>
#include <stdlib.h>

#include "consts.h"
#include "rect.h"

int main(void)
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) goto init_err;

  printf("Initialized SDL.\n");
  SDL_Window* win = SDL_CreateWindow("Title",
                                     SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED,
                                     WIDTH,
                                     HEIGHT,
                                     SDL_WINDOW_OPENGL);

  if (!win) goto window_err;
  printf("Rendered window.\n");

  SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!renderer) goto renderer_err;

  struct Rect floor = create_rect(0, 50, WIDTH, 50, 0, 255, 0, 255);
  struct Rect rects[] = { floor };
  struct Rects rect_container = { rects, 1 };
  
  int running = 1;
  const int render_timer = roundf(1000.0f / (float) FPS);
  
  while (running)
  {
    SDL_Event event;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    const int start_frame_time = SDL_GetTicks();

    if (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
      {
        running = 0;
      }
    }

    SDL_Rect rect = { 400, 400, 100, 100 };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        
    SDL_RenderFillRect(renderer, &rect);

    const int end_frame_time = SDL_GetTicks();
    SDL_Delay(max(10, render_timer - (end_frame_time - start_frame_time)));
    SDL_RenderPresent(renderer);
  }

 renderer_err:
  if (renderer) SDL_DestroyRenderer(renderer);
 window_err:
  if (win) SDL_DestroyWindow(win);
 init_err:
  if (strlen(SDL_GetError()) > 1) printf("SDL Error: %s\n", SDL_GetError());
  SDL_Quit();
  return 1;
}
