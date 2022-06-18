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
      Select_Level_Result result = select_level(renderer);
      level = result.level;
      screen = result.next_screen;
    case In_Level:
      screen = play_level(renderer, level);
    default:
      printf("Unknown screen: %d (see src/consts.h to lookup enum name)\n", screen);
      break;
    }
  }
}
