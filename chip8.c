#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <string.h>

#include "chip8.h"
#include "opcodes.h"

#define INSTRUCTION_START 0x200
#define FONTSET_SIZE 80

uint8_t fontset[FONTSET_SIZE] = {
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

C8Func table0[0xE + 1];
C8Func table8[0xE + 1];
C8Func tableE[0xE + 1];
C8Func tableF[0x65 + 1];

void Table0(chip8 *c8);
void Table8(chip8 *c8);
void TableE(chip8 *c8);
void TableF(chip8 *c8);

int init_chip8(chip8 *c8) {
  c8->pc = INSTRUCTION_START;
  c8->I = 0;
  c8->stackp = 0;
  c8->delaytimer = 0;
  c8->soundtimer = 0;
  c8->opcode = 0;

  memset(c8->memory, 0, sizeof(c8->memory));
  for (unsigned int i = 0; i < FONTSET_SIZE; ++i)
    c8->memory[FONT_START + i] = fontset[i];

  srand(time(NULL));
  c8->randbyte = rand() % 0x100;

  memset(c8->registers, 0, sizeof(c8->registers));
  memset(c8->stack, 0, sizeof(c8->stack));
  memset(c8->keys, 0, sizeof(c8->keys));
  memset(c8->video, 0, sizeof(c8->video));

  // Populate specific tables
  table0[0x0] = op_00E0;
  table0[0xE] = op_00EE;

  table8[0x0] = op_8xy0;
  table8[0x1] = op_8xy1;
  table8[0x2] = op_8xy2;
  table8[0x3] = op_8xy3;
  table8[0x4] = op_8xy4;
  table8[0x5] = op_8xy5;
  table8[0x6] = op_8xy6;
  table8[0x7] = op_8xy7;
  table8[0xE] = op_8xyE;

  tableE[0x1] = op_ExA1;
  tableE[0xE] = op_Ex9E;

  for (int i = 0; i <= 0x65; ++i)
    tableF[i] = op_null;

  tableF[0x07] = op_Fx07;
  tableF[0x0A] = op_Fx0A;
  tableF[0x15] = op_Fx15;
  tableF[0x18] = op_Fx18;
  tableF[0x1E] = op_Fx1E;
  tableF[0x29] = op_Fx29;
  tableF[0x33] = op_Fx33;
  tableF[0x55] = op_Fx55;
  tableF[0x65] = op_Fx65;

  // Populate general table
  c8->table[0x0] = Table0;
  c8->table[0x1] = op_1nnn;
  c8->table[0x2] = op_2nnn;
  c8->table[0x3] = op_3xkk;
  c8->table[0x4] = op_4xkk;
  c8->table[0x5] = op_5xy0;
  c8->table[0x6] = op_6xkk;
  c8->table[0x7] = op_7xkk;
  c8->table[0x8] = Table8;
  c8->table[0x9] = op_9xy0;
  c8->table[0xA] = op_Annn;
  c8->table[0xB] = op_Bnnn;
  c8->table[0xC] = op_Cxkk;
  c8->table[0xD] = op_Dxyn;
  c8->table[0xE] = TableE;
  c8->table[0xF] = TableF;
}

int load_rom(chip8 *c8, const char *filename) {
  FILE *f = fopen(filename, "rb");
  if (!f)
    return -1;

  fseek(f, 0L, SEEK_END);
  size_t fsize = ftell(f);
  char *buffer = malloc(sizeof(char)*fsize);

  fseek(f, 0L, SEEK_SET);
  if (fread(buffer, sizeof(char), fsize, f) != fsize) {
    fclose(f);
    return -2;
  }

  fclose(f);
  for (long i = 0; i < fsize; ++i)
    c8->memory[INSTRUCTION_START + i] = buffer[i];

  free(buffer);
  return 0;
}

void cycle(chip8 *c8) {
  // Fetch instruction
  c8->opcode = (c8->memory[c8->pc] << 8u) | c8->memory[c8->pc + 1];

  // Decode & Execute
  c8->table[(c8->opcode & 0xF000u) >> 12u](c8);
}

void Table0(chip8 *c8) {
  table0[c8->opcode & 0x000Fu](c8);
}

void Table8(chip8 *c8) {
  table8[c8->opcode & 0x000Fu](c8);
}

void TableE(chip8 *c8) {
  tableE[c8->opcode & 0x000Fu](c8);
}

void TableF(chip8 *c8) {
  tableF[c8->opcode & 0x00FFu](c8);
}
