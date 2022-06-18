#ifndef file_h
#define file_h

#include "rect.h"
#include "player.h"

typedef struct {
  Color background_color;
  Player player;
  Rects rects;
} Load_File_Result;

typedef struct {
  char **names;
  int num_names;
} File_Names;

void read_file(char *file_path, Load_File_Result *lfr);
File_Names get_files(char *directory);

#endif