#include "rect.h"
#include "player.h"

typedef struct {
  Color background_color;
  Player player;
  Rects rects;
} Load_File_Result;

void read_file(char *file_path, Load_File_Result *lfr);