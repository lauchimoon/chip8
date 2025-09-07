#include "chip8.h"
#include "raylib.h"
#include <stdio.h>
#include <time.h>

#define SCALE 10

#define WINDOW_WIDTH  WIDTH*SCALE
#define WINDOW_HEIGHT HEIGHT*SCALE

#define CPU_HZ           700
#define TIMER_HZ         60
#define CYCLES_PER_FRAME (CPU_HZ/TIMER_HZ)

void update(chip8 *c8);
void input(chip8 *c8);

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("usage: ./c8 <rom>\n");
    return 1;
  }

  chip8 c8;
  init_chip8(&c8);
  int ret;
  if ((ret = load_rom(&c8, argv[1])) < 0) {
    switch (ret) {
      case -1: printf("c8: could not find file '%s'\n", argv[1]); break;
      case -2: printf("c8: error reading from rom\n");
      default: break;
    }

    return 1;
  }

  SetTraceLogLevel(LOG_NONE);
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Chip 8");

  double last_cycle = GetTime();

  while (!WindowShouldClose()) {
    for (int i = 0; i < CYCLES_PER_FRAME; ++i)
      cycle(&c8);

    double current_cycle = GetTime();
    if (current_cycle - last_cycle >= 1.0/TIMER_HZ) {
      c8.delaytimer -= (c8.delaytimer > 0);
      c8.soundtimer -= (c8.soundtimer > 0);
      last_cycle = current_cycle;
    }

    input(&c8);
    update(&c8);
  }

  CloseWindow();
  return 0;
}

void update(chip8 *c8) {
  BeginDrawing();
  ClearBackground(BLACK);

  for (int y = 0; y < HEIGHT; ++y)
    for (int x = 0; x < WIDTH; ++x)
      DrawRectangle(x*SCALE, y*SCALE,
          SCALE, SCALE, GetColor(c8->video[y*WIDTH + x]));

  EndDrawing();
}

void input(chip8 *c8) {
  int keys[16] = {
    KEY_X, KEY_ONE, KEY_TWO, KEY_THREE,
    KEY_Q, KEY_W, KEY_E, KEY_A,
    KEY_S, KEY_D, KEY_Z, KEY_C,
    KEY_FOUR, KEY_R, KEY_F, KEY_V
  };

  for (int i = 0; i < 16; ++i)
    c8->keys[i] = IsKeyDown(keys[i]);
}
