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

static
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


// level_name must be no more than MAX_FILE_PATH_LENGTH in length
void input_level_name(char **level_name)
{
  (void)level_name;
  SDL_Window *window = SDL_CreateWindow("Enter File Name",
                                     SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED,
                                     400,
                                     200,
                                     SDL_WINDOW_OPENGL);

  if (!window) return;

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!renderer) return;

  TTF_Font *font = TTF_OpenFont("assets/Arial.ttf", 24);
  if (!font)
  {
    printf("No font!");
    exit(-1);
  }

  int texting = 1;
  const int render_timer = roundf(1000.0f / (float) FPS);

  SDL_Rect input_rect = { .x = 0, .y = 0, .w = 200, .h = 100 };
  SDL_SetTextInputRect(&input_rect);
  SDL_StartTextInput();

  char composition[MAX_FILE_PATH_LENGTH] = {0};
  int char_index = 0;
  int first_event = 1;

  while (texting)
  {
    SDL_Event event;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    const int start_frame_time = SDL_GetTicks();

    if (SDL_PollEvent(&event))
    {
      SDL_Keymod mod = SDL_GetModState();

      if (event.type == SDL_KEYDOWN)
      {
        if ((event.key.keysym.scancode == SDL_SCANCODE_Q && (mod & KMOD_CTRL)) ||
            event.key.keysym.scancode == SDL_SCANCODE_RETURN)
          break;

        if (event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE)
        {
          char_index--;
          composition[char_index] = 0;
          printf("backspace: %s\n", composition);
        }
      }

      if (event.type == SDL_TEXTINPUT)
      {
        strcat(composition, event.text.text);
        char_index++;
      }

      if (event.type == SDL_QUIT)
        break;

      if (first_event)
      {
        first_event = 0;
        composition[0] = 0;
        char_index = 0;
      }
    }


    render_text(renderer, font, "Enter text (RET to save):", 10, 10, 0);
    render_text(renderer, font, composition, 10, 50, 1);

    SDL_RenderDrawRect(renderer, &input_rect);

    const int end_frame_time = SDL_GetTicks();
    SDL_Delay(max(10, render_timer - (end_frame_time - start_frame_time)));
    SDL_RenderPresent(renderer);
  }

  TTF_CloseFont(font);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  strcpy(*level_name, composition);
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
            char level_name[MAX_FILE_PATH_LENGTH] = {0};
            char *level_name_p = (char *)level_name;
            input_level_name(&level_name_p);
            printf("level_name: %s\n", level_name);
            char file_path[MAX_FILE_PATH_LENGTH] = {0};
            strcat(file_path, "./levels/");
            strcat(file_path, level_name);
            write_level(level, file_path);
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
