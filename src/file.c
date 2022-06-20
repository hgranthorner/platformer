#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "file.h"
#include "consts.h"

char *read_file(char *file_name)
{
  FILE *fp;
  long buffer_size;
  char *buffer;

  fp = fopen(file_name, "rb");
  if (!fp)
  {
    perror(file_name);
    exit(1);
  }

  fseek(fp , 0L, SEEK_END);
  buffer_size = ftell(fp);
  rewind(fp);

/* allocate memory for entire content */
  buffer = calloc(1, buffer_size+1);
  if (!buffer)
  {
    fclose(fp);
    fputs("memory alloc when reading file fails", stderr);
    exit(1);
  }

/* copy the file into the buffer */
  if (1 != fread(buffer, buffer_size, 1, fp))
  {
    fclose(fp);
    free(buffer);
    fputs("entire read fails", stderr);
    exit(1);
  }

  fclose(fp);
  return buffer;
}

Lines get_lines(char *buffer)
{
  char *buffer_copy = buffer;
  char *lines[100];
  char *line = strtok(buffer_copy, "\n");
  lines[0] = line;
  int counter = 1;

  while ((line = strtok(NULL, "\n")) != NULL)
  {
    lines[counter] = line;
    counter++;
  }

  for (int i = 0; i < counter; i++)
  {
    printf("%d: %s\n", i, lines[i]);
  }

  Lines lines_struct = {.lines = lines, .num_lines = counter};
  return lines_struct;
}

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

void load_file(char *file_path, Level *out_level)
{
  char *file_contents = read_file(file_path);
  Lines file_lines = get_lines(file_contents);
  int rect_counter = 0;

  /* printf("starting to read file!\n"); */
  for (int line_number = 0; line_number < file_lines.num_lines; line_number++) {
    char *line = file_lines.lines[line_number];
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

      out_level->background_color.r = vals[0];
      out_level->background_color.g = vals[1];
      out_level->background_color.b = vals[2];
      out_level->background_color.a = vals[3];
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

      out_level->player = create_player(create_rect(vals[0], vals[1],
                                  PLAYER_SIZE, PLAYER_SIZE,
                                  0, 255, 255, 255, 0));
    }
    else if (line_number == 2)
    {
      out_level->rects.size = atoi(strtok(line, " "));
      out_level->rects.rects = malloc(out_level->rects.size * sizeof(Rect));
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

      out_level->rects.rects[rect_counter] = rect;

      rect_counter++;
    }
  }
}

void write_level(Level *level, char *file_path)
{
  FILE *file = fopen(file_path, "w");

  fprintf(file, "0 0 0 255\n%d %d\n%d\n",
          level->player.rect.shape.x, level->player.rect.shape.y, level->rects.size);
  for (int i = 0; i < level->rects.size; i++)
  {
    Rect *r = &level->rects.rects[i];
    SDL_Rect s = r->shape;
    Color c = r->color;
    fprintf(file, "%d %d %d %d %d %d %d %d %d\n",
            s.x, s.y, s.w, s.h,
            c.r, c.g, c.b, c.a,
            r->damaging);
  }

  fclose(file);
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
