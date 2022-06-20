#ifndef consts_h
#define consts_h

#define WIDTH  800
#define HEIGHT 640
#define FPS    60
#define PLAYER_SIZE       30
#define TERMINAL_VELOCITY 20
#define JUMP_VELOCITY     20

#define LEVEL_EDITOR "level_editor"

typedef enum {
  Level_Select = 0,
  In_Level = 1,
  Quit = 2,
  Level_Editor = 3,
} Screen_State;

static inline
int max(int x, int y)
{
  return x > y ? x : y;
}


#endif
