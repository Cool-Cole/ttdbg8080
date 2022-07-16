//
// Created by cole on 11/16/21.
//

#ifndef DISASSEMBLE8080_H
#define DISASSEMBLE8080_H

#include "intDefs.h"

/*
 * memory: A pointer to a series of 8080 instructions. In the case of this debugger the pointer will always be to the debuggers memory.
 * currentInstruction: The location within the memory array where you want to start disassembling
 */
int disassemble(u8 *memory, u16 currentInstruction);

void disassembleMulti(u8 *memory, u16 currentInstruction, int steps);

#endif //DISASSEMBLE8080_H
