#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "file.h"
#include "consts.h"

// enables getline - needs to be enabled after SDL due to macOS
// framework issue.
#define  _POSIX_C_SOURCE 200809L

/*
-background color rgba-
0 0 0 255
-player xy-
50 50
-num of platforms-
5
-platforms: xywhrgbadamaging-
0 0 1000 50 255 0 0 255 0
50 300 50 50 255 0 0 255 1
300 20 50 50 255 0 0 255 1
400 20 50 50 255 0 0 255 1
800 200 500 50 255 0 0 255 0
*/

void read_file(char *file_path, Load_File_Result *out_lfr)
{
  FILE *file_pointer = fopen(file_path, "r");
  if (file_pointer == NULL) exit(EXIT_FAILURE);

  int rect_counter = 0;

  char *line = NULL;

  size_t line_number = 0;
  size_t len = 0;
  ssize_t read;

  /* printf("starting to read file!\n"); */
  while ((read = getline(&line, &len, file_pointer)) != -1) {

    char *original_pointer = line;
    /* printf("reading line %s\n", line); */
    int has_read = 0;
    if (line_number == 0)
    {
      int vals[4];
      for (int i = 0; i < 4; i++)
      {
	char *num_str;
	if (has_read)
	{
	  num_str = strtok(NULL, " ");
	}
	else
	{
	  num_str = strtok(line, " ");
	  has_read = 1;
	}
        vals[i] = atoi(num_str);
      }

      out_lfr->background_color.r = vals[0];
      out_lfr->background_color.g = vals[1];
      out_lfr->background_color.b = vals[2];
      out_lfr->background_color.a = vals[3];
    }
    else if (line_number == 1)
    {
      int vals[2];
      for (int i = 0; i < 2; i++)
      {
	char *num_str;
	if (has_read)
	{
	  num_str = strtok(NULL, " ");
	}
	else
	{
	  num_str = strtok(line, " ");
	  has_read = 1;
	}
        vals[i] = atoi(num_str);
      }

      out_lfr->player = create_player(create_rect(vals[0], vals[1],
                                  PLAYER_SIZE, PLAYER_SIZE,
                                  0, 255, 255, 255, 0));
    }
    else if (line_number == 2)
    {
      out_lfr->rects.size = atoi(strtok(line, " "));
      out_lfr->rects.rects = malloc(out_lfr->rects.size * sizeof(Rect));
    }
    else
    {
      int vals[9];
      for (int i = 0; i < 9; i++)
      {
	char *num_str;
	if (has_read)
	{
	  num_str = strtok(NULL, " ");
	}
	else
	{
	  num_str = strtok(line, " ");
	  has_read = 1;
	}
        vals[i] = atoi(num_str);
      }

      Rect rect = {
        .shape = {
          .x = vals[0],
          .y = vals[1],
          .w = vals[2],
          .h = vals[3],
        },
        .color = {
          .r = vals[4],
          .g = vals[5],
          .b = vals[6],
          .a = vals[7],
        },
        .damaging = vals[8],
      };

      out_lfr->rects.rects[rect_counter] = rect;

      rect_counter++;
    }

    line = original_pointer;
    line_number++;
  }

  fclose(file_pointer);
  if (line) free(line);
}


File_Names get_files(char *directory) {
  DIR *d;
  struct dirent *dir;
  d = opendir(directory);

  File_Names files;
  files.names = malloc(sizeof(char *) * 50);
  int file_counter = 0;

  if (d) {
    while ((dir = readdir(d)) != NULL) {
      if (dir->d_name[0] == '.') continue;
      if (file_counter > 49)
      {
        printf("Too many files in %s!\n", directory);
        exit(EXIT_FAILURE);
      }
      files.names[file_counter] = strdup(dir->d_name);
      file_counter++;
    }
    closedir(d);
  }

  files.num_names = file_counter;

  return files;
}
