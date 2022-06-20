#include <SDL2/SDL_ttf.h>

#include "editor.h"
#include "file.h"

void initialize_default_level(Level *level)
{
  Player player = create_player(create_rect(10, 10, PLAYER_SIZE, PLAYER_SIZE, 0, 255, 255, 255, 0));
  Color black      = { .r = 0, .g = 0, .b = 0, .a = 255 };
  Rects rects	   = { .rects    = NULL, .size = 0 };
  level->background_color = black;
  level->player           = player;
  level->rects            = rects;
}

Screen_State edit_level(SDL_Renderer *renderer, Level *level)
{
  if (level->rects.size == 0)
  {
    printf("Initializing default level...\n");
    initialize_default_level(level);
  }

  SDL_Rect camera = { .x = 0,
                      .y = 0,
                      .w = WIDTH,
                      .h = HEIGHT };

  TTF_Font *font = TTF_OpenFont("assets/Arial.ttf", 24);
  if (!font)
  {
    printf("No font!");
    exit(-1);
  }

  int editing = 1;
  int player_selected = 0;
  const int render_timer = roundf(1000.0f / (float) FPS);

  while (editing)
  {
    SDL_Event event;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    const int start_frame_time = SDL_GetTicks();

    if (SDL_PollEvent(&event))
    {

      if (event.type == SDL_MOUSEBUTTONDOWN)
      {
        SDL_Point p = { .x = event.button.x, .y = event.button.y };
        if (SDL_PointInRect(&p, &level->player.rect.shape))
        {
          printf("Clicked on player!\n");
          player_selected = 1;
        }
      }

      if (event.type == SDL_MOUSEBUTTONUP)
      {
        player_selected = 0;
      }

      if (event.type == SDL_MOUSEMOTION && event.motion.state & SDL_BUTTON_LMASK)
      {
        if (player_selected)
        {
          level->player.rect.shape.x = event.motion.x - PLAYER_SIZE / 2;
          level->player.rect.shape.y = event.motion.y - PLAYER_SIZE / 2;
        }
      }

      if (event.type == SDL_KEYDOWN)
      {
        if (event.key.keysym.scancode == SDL_SCANCODE_L)
        {
	  return Level_Select;
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_S)
        {
          write_level(level, "./levels/test.txt");
	  return Level_Select;
        }

        if (event.key.keysym.scancode == SDL_SCANCODE_Q)
        {
	  return Quit;
        }
      }

      if (event.type == SDL_QUIT)
      {
        return Quit;
      }
    }

    for (int i = 0; i < level->rects.size; ++i)
    {
      render_fill_rect(renderer, &camera, &level->rects.rects[i]);
    }

    render_fill_rect(renderer, &camera, &level->player.rect);

    const int end_frame_time = SDL_GetTicks();
    SDL_Delay(max(10, render_timer - (end_frame_time - start_frame_time)));
    SDL_RenderPresent(renderer);
  }

  return Level_Select;
}