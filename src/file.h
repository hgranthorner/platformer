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

typedef struct {
  char **lines;
  int num_lines;
} Lines;

void load_file(char *file_path, Load_File_Result *out_lfr);
File_Names get_files(char *directory);

#endif
