#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>

#include "render.h"
#include "consts.h"
#include "rect.h"
#include "player.h"
#include "camera.h"
#include "file.h"

Screen_State play_level(SDL_Renderer *renderer, char *level)
{
  SDL_Rect camera = { .x = 0,
                      .y = 0,
                      .w = WIDTH,
                      .h = HEIGHT };

  Rect floor = create_rect(0, HEIGHT - 50,
                           4000, 50,
                           0, 255, 0, 255, 0);

  Load_File_Result lfr;
  char level_name[100];
  sprintf(level_name, "levels/%s", level);
  read_file(level_name, &lfr);
  printf("loaded level %s!\n", level);

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
        if (event.key.keysym.scancode == SDL_SCANCODE_L)
        {
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

  return Quit;
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

void render_text(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y, int is_selected)
{
  int w, h;
  TTF_SizeText(font, text, &w, &h);
  SDL_Color white = {255, 255, 255,255};
  SDL_Color black = {0, 0, 0,255};
  SDL_Surface *surface;
  if (!is_selected)
  {
    surface = TTF_RenderText_Shaded(font, text, white, black);
  }
  else
  {
    surface = TTF_RenderText_Shaded(font, text, black, white);
  }

  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_Rect rect = {.x = x, .y = y, .w = w, .h = h};
  SDL_RenderCopy(renderer, texture, NULL, &rect);

  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);
}

Select_Level_Result select_level(SDL_Renderer *renderer)
{
  File_Names files = get_files("./levels");
  int selected = 0;
  int choosing = 1;

  TTF_Font *font = TTF_OpenFont("assets/Arial.ttf", 24);
  Select_Level_Result result;

  // + 1 so that we can select the level editor
  int num_options = files.num_names + 1;

  if (font == NULL)
  {
    printf("No font!");
    exit(-1);
  }
  const int render_timer = roundf(1000.0f / (float) FPS);

  while (choosing)
  {
    SDL_Event event;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    const int start_frame_time = SDL_GetTicks();

    if (SDL_PollEvent(&event))
    {
      if (event.type == SDL_KEYDOWN)
      {
        if (event.key.keysym.scancode == SDL_SCANCODE_UP && selected > 0)
        {
          selected--;
        }

        if (event.key.keysym.scancode == SDL_SCANCODE_DOWN && selected < num_options)
        {
          selected++;
        }

        if (event.key.keysym.scancode == SDL_SCANCODE_RETURN)
        {
	  if (selected == num_options - 1)
	  {
	    result.next_screen = Level_Editor;
	    result.level = LEVEL_EDITOR;
	    return result;
	  }

	  result.next_screen = In_Level;
	  result.level = files.names[selected];
	  return result;
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

    render_text(renderer, font, "Select a level:", 100, 0, 0);
    for (int i = 0; i < files.num_names; i++)
    {
      render_text(renderer, font, files.names[i], 100, i * 50 + 50, i == selected);
    }
    render_text(renderer, font, "Level Editor", 100, num_options * 50 + 50, selected == num_options - 1);

    const int end_frame_time = SDL_GetTicks();
    SDL_Delay(max(10, render_timer - (end_frame_time - start_frame_time)));
    SDL_RenderPresent(renderer);
  }
  result.next_screen = Quit;
  result.level = "";
  return result;
}
