#ifndef consts_h
#define consts_h

#define WIDTH  1000
#define HEIGHT 800
#define FPS    60
#define PLAYER_SIZE       30
#define TERMINAL_VELOCITY 20
#define JUMP_VELOCITY     20

typedef enum {
  Level_Select = 0,
  In_Level = 1,
  Quit = 2
} Screen_State;

#define IN_LEVEL     0
#define LEVEL_SELECT 1

static inline
int max(int x, int y)
{
  return x > y ? x : y;
}


#endif
