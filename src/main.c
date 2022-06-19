#include <SDL2/SDL.h>
#include <stdlib.h>

#include "consts.h"
#include "render.h"
#include "editor.h"

int main(void)
{
  SDL_Renderer *renderer = init_sdl();
  Screen_State screen = Level_Select;
  Level level = {0};
  char *level_name;
  Select_Level_Result result;
  while (screen != Quit)
  {
    switch (screen)
    {
    case Level_Select:
      result = select_level(renderer);
      level_name = result.level_name;
      screen = result.next_screen;
      break;
    case In_Level:
      screen = play_level(renderer, level_name);
      break;
    case Level_Editor:
      // if level_name is assigned, we load the level.
      // otherwise, we leave it null.
      if (level_name)
	load_file(level_name, &level);
      screen = edit_level(renderer, &level);
    case Quit:
      printf("Quitting gracefully...\n");
      return 0;
    default:
      printf("Unknown screen: %d (see src/consts.h to lookup enum name)\n", screen);
      return -1;
    }
  }
}
