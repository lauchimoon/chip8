#include "chip8.h"
#include "opcodes.h"
#include <string.h>
#include <stdio.h>

void op_00E0(chip8 *c8) {
  memset(c8->video, 0, sizeof(c8->video));
  c8->pc += 2;
}

void op_00EE(chip8 *c8) {
  --c8->stackp;
  c8->pc = c8->stack[c8->stackp];
  c8->pc += 2;
}

void op_1nnn(chip8 *c8) {
  uint16_t addr = c8->opcode & 0x0FFFu;
  c8->pc = addr;
}

void op_2nnn(chip8 *c8) {
  uint16_t addr = c8->opcode & 0x0FFFu;
  c8->stack[(c8->stackp)++] = c8->pc;
  c8->pc = addr;
}

void op_3xkk(chip8 *c8) {
  uint8_t Vx = (c8->opcode & 0x0F00u) >> 8u;
  uint8_t byte = c8->opcode & 0x00FFu;

  c8->pc += (c8->registers[Vx] == byte)? 4 : 2;
}

void op_4xkk(chip8 *c8) {
  uint8_t Vx = (c8->opcode & 0x0F00u) >> 8u;
  uint8_t byte = c8->opcode & 0x00FFu;

  c8->pc += (c8->registers[Vx] != byte)? 4 : 2;
}

void op_5xy0(chip8 *c8) {
  uint8_t Vx = (c8->opcode & 0x0F00u) >> 8u;
  uint8_t Vy = (c8->opcode & 0x00F0u) >> 4u;

  c8->pc += (c8->registers[Vx] == c8->registers[Vy])? 4 : 2;
}

void op_6xkk(chip8 *c8) {
  uint8_t Vx = (c8->opcode & 0x0F00u) >> 8u;
  uint8_t byte = c8->opcode & 0x00FFu;

  c8->registers[Vx] = byte;
  c8->pc += 2;
}

void op_7xkk(chip8 *c8) {
  uint8_t Vx = (c8->opcode & 0x0F00u) >> 8u;
  uint8_t byte = c8->opcode & 0x00FFu;

  c8->registers[Vx] += byte;
  c8->pc += 2;
}

void op_8xy0(chip8 *c8) {
  uint8_t Vx = (c8->opcode & 0x0F00u) >> 8u;
  uint8_t Vy = (c8->opcode & 0x00F0u) >> 4u;

  c8->registers[Vx] = c8->registers[Vy];
  c8->pc += 2;
}

void op_8xy1(chip8 *c8) {
  uint8_t Vx = (c8->opcode & 0x0F00u) >> 8u;
  uint8_t Vy = (c8->opcode & 0x00F0u) >> 4u;

  c8->registers[Vx] |= c8->registers[Vy];
  c8->pc += 2;
}

void op_8xy2(chip8 *c8) {
  uint8_t Vx = (c8->opcode & 0x0F00u) >> 8u;
  uint8_t Vy = (c8->opcode & 0x00F0u) >> 4u;

  c8->registers[Vx] &= c8->registers[Vy];
  c8->pc += 2;
}

void op_8xy3(chip8 *c8) {
  uint8_t Vx = (c8->opcode & 0x0F00u) >> 8u;
  uint8_t Vy = (c8->opcode & 0x00F0u) >> 4u;

  c8->registers[Vx] ^= c8->registers[Vy];
  c8->pc += 2;
}

void op_8xy4(chip8 *c8) {
  uint8_t Vx = (c8->opcode & 0x0F00u) >> 8u;
  uint8_t Vy = (c8->opcode & 0x00F0u) >> 4u;
  uint16_t sum = c8->registers[Vx] + c8->registers[Vy];

  c8->registers[Vx] = (sum & 0xFFu);
  c8->registers[0xF] = (sum > 255U);
  c8->pc += 2;
}

void op_8xy5(chip8 *c8) {
  uint8_t Vx = (c8->opcode & 0x0F00u) >> 8u;
  uint8_t Vy = (c8->opcode & 0x00F0u) >> 4u;

  c8->registers[Vx] -= c8->registers[Vy];
  c8->registers[0xF] = c8->registers[Vx] < c8->registers[Vy]? 0 : 1;
  c8->pc += 2;
}

void op_8xy6(chip8 *c8) {
  uint8_t Vx = (c8->opcode & 0x0F00u) >> 8u;
  uint8_t Vy = (c8->opcode & 0x00F0u) >> 4u;

  c8->registers[0xF] = (c8->registers[Vx] & 0x1u);
  c8->registers[Vx] >>= 1;
  c8->pc += 2;
}

void op_8xy7(chip8 *c8) {
  uint8_t Vx = (c8->opcode & 0x0F00u) >> 8u;
  uint8_t Vy = (c8->opcode & 0x00F0u) >> 4u;

  c8->registers[Vx] = c8->registers[Vy] - c8->registers[Vx];
  c8->registers[0xF] = c8->registers[Vx] > c8->registers[Vy]? 0 : 1;
  c8->pc += 2;
}

void op_8xyE(chip8 *c8) {
  uint8_t Vx = (c8->opcode & 0x0F00u) >> 8u;
  c8->registers[Vx] <<= 1;
  c8->registers[0xF] = (c8->registers[Vx] & 0x80u) >> 7u;
  c8->pc += 2;
}

void op_9xy0(chip8 *c8) {
  uint8_t Vx = (c8->opcode & 0x0F00u) >> 8u;
  uint8_t Vy = (c8->opcode & 0x00F0u) >> 4u;

  c8->pc += (c8->registers[Vx] != c8->registers[Vy])? 4 : 2;
}

void op_Annn(chip8 *c8) {
  uint16_t addr = c8->opcode & 0x0FFFu;
  c8->I = addr;
  c8->pc += 2;
}

void op_Bnnn(chip8 *c8) {
  uint16_t addr = c8->opcode & 0x0FFFu;
  c8->pc = c8->registers[0] + addr;
}

void op_Cxkk(chip8 *c8) {
  uint8_t Vx = (c8->opcode & 0x0F00u) >> 8u;
  uint8_t byte = c8->opcode & 0x00FFu;

  c8->registers[Vx] = c8->randbyte & byte;
  c8->pc += 2;
}

void op_Dxyn(chip8 *c8) {
  uint8_t Vx = (c8->opcode & 0x0F00u) >> 8u;
  uint8_t Vy = (c8->opcode & 0x00F0u) >> 4u;
  uint8_t height = c8->opcode & 0x000Fu;

  uint16_t x = c8->registers[Vx] % WIDTH;
  uint16_t y = c8->registers[Vy] % HEIGHT;

  c8->registers[0xF] = 0;

  for (unsigned int row = 0; row < height; ++row) {
    uint8_t sprite_byte = c8->memory[c8->I + row];

    for (unsigned int col = 0; col < 8; ++col) {
      uint8_t sprite_pixel = sprite_byte & (0x80u >> col);
      uint32_t *screen_pixel = &c8->video[(y+row)*WIDTH + (x+col)];

      if (sprite_pixel) {
        if (*screen_pixel == 0xFFFFFFFF)
          c8->registers[0xF] = 1;

        *screen_pixel ^= 0xFFFFFFFF;
      }
    }
  }

  c8->pc += 2;
}

void op_Ex9E(chip8 *c8) {
  uint8_t Vx = (c8->opcode & 0x0F00u) >> 8u;
  uint8_t key = c8->registers[Vx];

  c8->pc += (c8->keys[key])? 4 : 2;
}

void op_ExA1(chip8 *c8) {
  uint8_t Vx = (c8->opcode & 0x0F00u) >> 8u;
  uint8_t key = c8->registers[Vx];

  c8->pc += (!c8->keys[key])? 4 : 2;
}

void op_Fx07(chip8 *c8) {
  uint8_t Vx = (c8->opcode & 0x0F00u) >> 8u;
  c8->registers[Vx] = c8->delaytimer;
  c8->pc += 2;
}

void op_Fx0A(chip8 *c8) {
  uint8_t Vx = (c8->opcode & 0x0F00u) >> 8u;

  if (c8->keys[0]) c8->registers[Vx] = 0;
  else if (c8->keys[1]) c8->registers[Vx] = 1;
  else if (c8->keys[2]) c8->registers[Vx] = 2;
  else if (c8->keys[3]) c8->registers[Vx] = 3;
  else if (c8->keys[4]) c8->registers[Vx] = 4;
  else if (c8->keys[5]) c8->registers[Vx] = 5;
  else if (c8->keys[6]) c8->registers[Vx] = 6;
  else if (c8->keys[7]) c8->registers[Vx] = 7;
  else if (c8->keys[8]) c8->registers[Vx] = 8;
  else if (c8->keys[9]) c8->registers[Vx] = 9;
  else if (c8->keys[10]) c8->registers[Vx] = 10;
  else if (c8->keys[11]) c8->registers[Vx] = 11;
  else if (c8->keys[12]) c8->registers[Vx] = 12;
  else if (c8->keys[13]) c8->registers[Vx] = 13;
  else if (c8->keys[14]) c8->registers[Vx] = 14;
  else if (c8->keys[15]) c8->registers[Vx] = 15;
  else return;
}

void op_Fx15(chip8 *c8) {
  uint8_t Vx = (c8->opcode & 0x0F00u) >> 8u;
  c8->delaytimer = c8->registers[Vx];
  c8->pc += 2;
}

void op_Fx18(chip8 *c8) {
  uint8_t Vx = (c8->opcode & 0x0F00u) >> 8u;
  c8->soundtimer = c8->registers[Vx];
  c8->pc += 2;
}

void op_Fx1E(chip8 *c8) {
  uint8_t Vx = (c8->opcode & 0x0F00u) >> 8u;
  c8->I += c8->registers[Vx];
  c8->pc += 2;
}

void op_Fx29(chip8 *c8) {
  uint8_t Vx = (c8->opcode & 0x0F00u) >> 8u;
  uint8_t digit = c8->registers[Vx];

  c8->I = FONT_START + (5*digit);
  c8->pc += 2;
}

void op_Fx33(chip8 *c8) {
  uint8_t Vx = (c8->opcode & 0x0F00u) >> 8u;
  uint8_t value = c8->registers[Vx];

  c8->memory[c8->I+2] = value%10;
  value /= 10;

  c8->memory[c8->I+1] = value%10;
  value /= 10;

  c8->memory[c8->I] = value%10;
  c8->pc += 2;
}

void op_Fx55(chip8 *c8) {
  uint8_t Vx = (c8->opcode & 0x0F00u) >> 8u;
  for (uint8_t i = 0; i <= Vx; ++i)
    c8->memory[c8->I + i] = c8->registers[i];
  c8->pc += 2;
}

void op_Fx65(chip8 *c8) {
  uint8_t Vx = (c8->opcode & 0x0F00u) >> 8u;
  for (uint8_t i = 0; i <= Vx; ++i)
    c8->registers[i] = c8->memory[c8->I + i];
  c8->pc += 2;
}

void op_null(chip8 *c8) {}
