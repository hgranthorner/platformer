#include "rect.h"

typedef struct {
  Color background_color;
  SDL_Rect player_rect;
  int num_platforms;
  Rect *rects;
} Load_File_Result;

void read_file(char *file_path, Load_File_Result *lfr);