#include <SDL2/SDL_ttf.h>

#include "editor.h"
#include "file.h"

void initialize_default_level(Level *level)
{
  Player player = create_player(create_rect(10, 10, PLAYER_SIZE, PLAYER_SIZE, 0, 255, 255, 255, 0));
  Color black      = { .r = 0, .g = 0, .b = 0, .a = 255 };
  Rects rects	   = { .rects = malloc(sizeof(Rect)), .size = 0, .capacity = 1 };
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

  int editing            = 1;
  int player_selected    = 0;
  const int render_timer = roundf(1000.0f / (float) FPS);
  int platform_selected_index = -1;

  while (editing)
  {
    SDL_Event event;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    const int start_frame_time = SDL_GetTicks();

    const Uint8 *state = SDL_GetKeyboardState(NULL);

    if (state[level->player.controls.left])
      camera.x -= 5;

    if (state[level->player.controls.right])
      camera.x += 5;

    if (SDL_PollEvent(&event))
    {

      if (event.type == SDL_MOUSEBUTTONDOWN)
      {
        SDL_Point p = { .x = event.button.x + camera.x,
                        .y = event.button.y + camera.y};
        if (SDL_PointInRect(&p, &level->player.rect.shape))
        {
          printf("Clicked on player!\n");
          player_selected = 1;
        }

        for (int i = 0; i < level->rects.size; i++)
        {
          if (SDL_PointInRect(&p, &level->rects.rects[i].shape))
          {
            printf("Clicked on rect %d!\n", i);
            platform_selected_index = i;
          }
        }
      }

      if (event.type == SDL_MOUSEBUTTONUP)
      {
        player_selected = 0;
        platform_selected_index = -1;
      }

      if (event.type == SDL_MOUSEMOTION && event.motion.state & SDL_BUTTON_LMASK)
      {
        int new_x = event.motion.x + camera.x;
        int new_y = event.motion.y + camera.y;
        if (player_selected)
        {
          level->player.rect.shape.x = new_x - PLAYER_SIZE / 2;
          level->player.rect.shape.y = new_y - PLAYER_SIZE / 2;
        }

        if (platform_selected_index != -1)
        {
          SDL_Rect *r = &level->rects.rects[platform_selected_index].shape;
          r->x = new_x - r->w / 2;
          r->y = new_y - r->h / 2;
        }
      }

      if (event.type == SDL_KEYDOWN)
      {
        if (platform_selected_index != -1)
        {
          SDL_Rect *r = &level->rects.rects[platform_selected_index].shape;

          if (event.key.keysym.scancode == SDL_SCANCODE_W)
            r->h += 10, r->y -= 10;

          if (event.key.keysym.scancode == SDL_SCANCODE_A)
            r->w -= 10;

          if (event.key.keysym.scancode == SDL_SCANCODE_S)
            r->h -= 10;

          if (event.key.keysym.scancode == SDL_SCANCODE_D)
            r->w += 10;
        }
        else
        {
          if (event.key.keysym.scancode == SDL_SCANCODE_L)
            return Level_Select;

          if (event.key.keysym.scancode == SDL_SCANCODE_S)
          {
            write_level(level, "./levels/test.txt");
            return Level_Select;
          }

          if (event.key.keysym.scancode == SDL_SCANCODE_A)
          {
            Rect rect = create_rect(100, 200, 100, 100, 0, 255, 0, 255, 0);
            add_rect(&level->rects, rect);
          }

          if (event.key.keysym.scancode == SDL_SCANCODE_D)
          {
            Rect rect = create_rect(100, 200, 100, 100, 255, 0, 0, 255, 1);
            add_rect(&level->rects, rect);
          }

          if (event.key.keysym.scancode == SDL_SCANCODE_Q)
            return Quit;
        }
      }

      if (event.type == SDL_QUIT)
        return Quit;
    }

    for (int i = 0; i < level->rects.size; ++i)
      render_fill_rect(renderer, &camera, &level->rects.rects[i]);

    render_fill_rect(renderer, &camera, &level->player.rect);

    const int end_frame_time = SDL_GetTicks();
    SDL_Delay(max(10, render_timer - (end_frame_time - start_frame_time)));
    SDL_RenderPresent(renderer);
  }

  return Level_Select;
}
