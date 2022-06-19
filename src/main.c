#include <SDL2/SDL.h>
#include <stdlib.h>

#include "consts.h"
#include "render.h"

int main(void)
{
  SDL_Renderer *renderer = init_sdl();
  Screen_State screen = Level_Select;
  char *level;
  Select_Level_Result result;
  while (screen != Quit)
  {
    switch (screen)
    {
    case Level_Select:
      result = select_level(renderer);
      level = result.level;
      screen = result.next_screen;
      break;
    case In_Level:
      screen = play_level(renderer, level);
      break;
    default:
      printf("Unknown screen: %d (see src/consts.h to lookup enum name)\n", screen);
      return -1;
      break;
    }
  }
}
