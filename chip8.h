#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>

#define NUM_REGISTERS 16
#define NUM_PCS       16
#define NUM_KEYS      16
#define MEMORY_CAP  4096

#define FONT_START 0x50

#define WIDTH  64
#define HEIGHT 32

typedef struct chip8 chip8;

typedef void (*C8Func)(chip8 *);

struct chip8 {
  uint8_t registers[NUM_REGISTERS];
  uint8_t memory[MEMORY_CAP];
  int16_t I;
  int16_t pc;
  int16_t stack[NUM_PCS];
  uint8_t stackp;
  int8_t delaytimer;
  int8_t soundtimer;
  uint8_t keys[NUM_KEYS];
  uint32_t video[WIDTH*HEIGHT];
  uint16_t opcode;
  uint8_t randbyte;

  C8Func table[0xF+1];
};

int init_chip8(chip8 *c8);
int load_rom(chip8 *c8, const char *filename);
void cycle(chip8 *c8);

#endif // CHIP8_H
