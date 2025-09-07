#ifndef OPCODES_H
#define OPCODES_H

#include "chip8.h"

void op_00E0(chip8 *c8);
void op_00EE(chip8 *c8);
void op_1nnn(chip8 *c8);
void op_2nnn(chip8 *c8);
void op_3xkk(chip8 *c8);
void op_4xkk(chip8 *c8);
void op_5xy0(chip8 *c8);
void op_6xkk(chip8 *c8);
void op_7xkk(chip8 *c8);
void op_8xy0(chip8 *c8);
void op_8xy1(chip8 *c8);
void op_8xy2(chip8 *c8);
void op_8xy3(chip8 *c8);
void op_8xy4(chip8 *c8);
void op_8xy5(chip8 *c8);
void op_8xy6(chip8 *c8);
void op_8xy7(chip8 *c8);
void op_8xyE(chip8 *c8);
void op_9xy0(chip8 *c8);
void op_Annn(chip8 *c8);
void op_Bnnn(chip8 *c8);
void op_Cxkk(chip8 *c8);
void op_Dxyn(chip8 *c8);
void op_Ex9E(chip8 *c8);
void op_ExA1(chip8 *c8);
void op_Fx07(chip8 *c8);
void op_Fx0A(chip8 *c8);
void op_Fx15(chip8 *c8);
void op_Fx18(chip8 *c8);
void op_Fx1E(chip8 *c8);
void op_Fx29(chip8 *c8);
void op_Fx33(chip8 *c8);
void op_Fx55(chip8 *c8);
void op_Fx65(chip8 *c8);
void op_null(chip8 *c8);

#endif // OPCODES_H
