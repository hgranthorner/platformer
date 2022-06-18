#include <SDL2/SDL.h>
#include <stdlib.h>

#include "consts.h"
#include "file.h"
#include "render.h"

void text_load_file(void)
{
  Load_File_Result lfr;
  read_file("01.txt", &lfr);
}

int main(void)
{
  SDL_Renderer *renderer = init_sdl();
  Screen_State screen = Level_Select;
  char *level;
  while (screen != Quit)
  {
    switch (screen)
    {
    case Level_Select:
      level = choose_level(renderer);
    case In_Level:
      screen = play_level(renderer, level);
    default:
      break;
    }
  }
}
