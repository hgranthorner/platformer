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

  char *level = choose_level(renderer);

  play_level(renderer, level);
}
