#include <SDL2/SDL.h>
#include <stdlib.h>

#include "consts.h"
#include "rect.h"
#include "player.h"
#include "camera.h"

int main(void)
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) goto init_err;

  printf("Initialized SDL.\n");
  SDL_Window* win = SDL_CreateWindow("Platformer",
                                     SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED,
                                     WIDTH,
                                     HEIGHT,
                                     SDL_WINDOW_OPENGL);

  if (!win) goto window_err;
  printf("Rendered window.\n");

  SDL_Renderer *renderer = SDL_CreateRenderer(win, -1,
                                              SDL_RENDERER_ACCELERATED
                                              | SDL_RENDERER_PRESENTVSYNC);
  if (!renderer) goto renderer_err;

  SDL_Rect camera = { 0, 0, WIDTH, HEIGHT };

  Rect floor = create_rect(0, HEIGHT - 50,
                                  WIDTH, 50,
                                  0, 255, 0, 255);

  Rect platform = create_rect(500, 300, 300, 50, 0, 255, 0, 255);
  Rect rects[] = { floor, platform };
  Rects rect_container = { .rects = rects,
                                  .size = 2 };

  Controls controls = { .right = SDL_SCANCODE_RIGHT,
                               .left = SDL_SCANCODE_LEFT,
                               .jump = SDL_SCANCODE_SPACE };
  Player player = { .rect = create_rect(10, HEIGHT - 100,
                                       PLAYER_SIZE, PLAYER_SIZE,
                                       0, 255, 255, 255),
                           .controls = controls,
                           .x_velocity = 0,
                           .y_velocity = 0 };
  
  int running = 1;
  const int render_timer = roundf(1000.0f / (float) FPS);
  
  while (running)
  {
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    if (state[controls.left])
    {
      player.rect.shape.x -= 5;
    }
    if (state[controls.right])
    {
      player.rect.shape.x += 5;
    }

    SDL_Event event;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    const int start_frame_time = SDL_GetTicks();

    if (SDL_PollEvent(&event))
    {
      if (event.type == SDL_KEYDOWN)
      {
        if (event.key.keysym.scancode == controls.jump)
        {
          player.y_velocity -= 30;
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_R)
        {
          player.rect.shape.x = camera.w / 2 + camera.x;
          player.rect.shape.y = camera.h / 2 + camera.y;
        }
      }
      if (event.type == SDL_QUIT)
      {
        running = 0;
      }
    }

    for (int i = 0; i < rect_container.size; ++i)
    {
      render_fill_rect(renderer, &camera, &rect_container.rects[i]);
    }

    move_player_position(&player);
    move_camera(&player, &camera);
    apply_gravity(&player, &rect_container);
    render_fill_rect(renderer, &camera, &player.rect);

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
