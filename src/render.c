#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>

#include "consts.h"
#include "rect.h"
#include "player.h"
#include "camera.h"
#include "file.h"

void play_level(SDL_Renderer *renderer)
{
  SDL_Rect camera = { .x = 0,
                      .y = 0,
                      .w = WIDTH,
                      .h = HEIGHT };

  Rect floor = create_rect(0, HEIGHT - 50,
                           4000, 50,
                           0, 255, 0, 255, 0);

  Load_File_Result lfr;
  read_file("levels/01.txt", &lfr);
  printf("loaded level!\n");

  Rects rect_container = lfr.rects;
  Player player = lfr.player;
  rect_container.rects = realloc(rect_container.rects, (rect_container.size + 1) * sizeof(Rect));
  rect_container.size = rect_container.size + 1;
  rect_container.rects[rect_container.size - 1] = floor;

  const Controls controls = player.controls;
  int running = 1;
  const int render_timer = roundf(1000.0f / (float) FPS);

  while (running)
  {
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    if (state[controls.left])
    {
      player.rect.shape.x -= 8;
    }
    if (state[controls.right])
    {
      player.rect.shape.x += 8;
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
          if (player.jumps_remaining > 0)
          {
            player.jumps_remaining -= 1;
            player.y_velocity = -JUMP_VELOCITY;
          }
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_R)
        {
          reset_player_position(&player, &camera);
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_Q)
        {
          running = 0;
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
    apply_gravity(&player, &camera, &rect_container);
    render_fill_rect(renderer, &camera, &player.rect);

    const int end_frame_time = SDL_GetTicks();
    SDL_Delay(max(10, render_timer - (end_frame_time - start_frame_time)));
    SDL_RenderPresent(renderer);
  }
}

SDL_Renderer *init_sdl()
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) return NULL;

  printf("Initialized SDL.\n");
  SDL_Window* win = SDL_CreateWindow("Platformer",
                                     SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED,
                                     WIDTH,
                                     HEIGHT,
                                     SDL_WINDOW_OPENGL);

  if (!win) return NULL;
  printf("Rendered window.\n");
  SDL_Renderer *renderer = SDL_CreateRenderer(win, -1,
                                              SDL_RENDERER_ACCELERATED
                                              | SDL_RENDERER_PRESENTVSYNC);
  if (!renderer) return NULL;

  if (TTF_Init() != 0) return NULL;

  return renderer;
}

void render_text(SDL_Renderer *renderer, TTF_Font* font, char *text, SDL_Rect rect)
{
  SDL_Color white = {255, 255, 255};
  SDL_Surface *surface = TTF_RenderText_Solid(font, text, white); 

  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_RenderCopy(renderer, texture, NULL, &rect);

  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);
}

char *choose_level(SDL_Renderer *renderer)
{
  File_Names files = get_files("./levels");
  int selected = 0;
  int choosing = 1;

  const int render_timer = roundf(1000.0f / (float) FPS);  

  while (choosing)
  {
    SDL_Event event;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    const int start_frame_time = SDL_GetTicks();
    TTF_Font* font = TTF_OpenFont("assets/Arial.ttf", 24);

    if (font == NULL) printf("No font!");
    
    if (SDL_PollEvent(&event))
    {
      if (event.type == SDL_KEYDOWN)
      {
        if (event.key.keysym.scancode == SDL_SCANCODE_UP && selected > 0)
        {
          selected--;
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_DOWN && selected < files.num_names)
        {
          selected++;
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_Q)
        {
          choosing = 0;
        }
      }

      if (event.type == SDL_QUIT)
      {
        choosing = 0;
      }
    }

    for (int i = 0; i < files.num_names; i++)
    {
      printf("%s\n", files.names[i]);
      SDL_Rect rect = {.x = 100, .y = 100 * i + 100, .w = 100, .h = 100};
      render_text(renderer, font, files.names[i], rect);
    }

    const int end_frame_time = SDL_GetTicks();
    SDL_Delay(max(10, render_timer - (end_frame_time - start_frame_time)));
    SDL_RenderPresent(renderer);
  }
  return "";
}
