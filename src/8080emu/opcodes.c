//
// Created by cole on 11/28/21.
//

#include "opcodes.h"

#include <stdio.h>

// TODO: Get an actual C programmers opinion on the two read/write functions below.

static inline u16 readShort(const u8 *mem, const u16 address) {
    // Do the pointer addition while mem is of type u8* then cast mem to type u16* in order to read the value
    return *(u16 *) (mem + address);
}

static inline void writeShort(u8 *mem, const u16 address, const u16 data) {
    // Do the pointer addition while mem is of type u8* then cast mem to type u16* in order to write the value
    *(u16 *) (mem + address) = data;
}

// 0x00
void NOP(cpuState *state) {
    state->PC += 1;
}

// 0x01
void LXI_BC(cpuState *state) {
    state->BC = readShort(state->memory, state->PC + 1);

    state->PC += 3;
}

// 0x02
void STAX_B(cpuState *state) {
    state->memory[state->BC] = state->A;

    state->PC += 1;
}

// 0x03
void INX_BC(cpuState *state) {
    state->BC += 1;

    state->PC += 1;
}

// 0x04
void INR_B(cpuState *state) {
    state->B = state->B + 1;

    // If the 7th bit is 1 then set the sign flag
    state->flags.sign = state->B >> 7;

    // If there are an even number of 1s set the parity flag to 1
    state->flags.parity = !(__builtin_popcount(state->B) & 1);

    // If the arithmetic operation results in 0 then set the zero flag to 1
    state->flags.zero = (0 == state->B);

    state->PC += 1;
}

// 0x05
void DCR_B(cpuState *state) {
    u16 answer = state->B - 1;
    state->B = answer & 0xff;

    state->flags.sign = state->B >> 7;
    state->flags.parity = !(__builtin_popcount(state->B) & 1);
    state->flags.zero = (0 == state->B);

    state->PC += 1;
}

// 0x06
void MVI_B(cpuState *state) {
    state->B = state->memory[state->PC + 1];

    state->PC += 2;
}

// 0x07
void RLC(cpuState *state) {
    state->flags.carry = state->A >> 7;

    u16 answer = state->A << 1 | state->A >> 7;

    state->A = answer & 0xff;

    state->PC += 1;
}

// 0x09
void DAD_BC(cpuState *state) {
    u32 answer = state->HL + state->BC;

    state->HL = answer & 0xffff;

    state->flags.carry = answer > 0xffff;

    state->PC += 1;
}

// 0x0a
void LDAX_BC(cpuState *state) {
    state->A = state->memory[state->BC];

    state->PC += 1;
}

// 0x0b
void DCX_BC(cpuState *state) {
    state->BC -= 1;

    state->PC += 1;
}

// 0x0c
void INR_C(cpuState *state) {
    state->C = state->C + 1;

    state->flags.sign = state->C >> 7;
    state->flags.parity = !(__builtin_popcount(state->C) & 1);
    state->flags.zero = (0 == state->C);

    state->PC += 1;
}

// 0x0d
void DCR_C(cpuState *state) {
    u16 answer = state->C - 1;
    state->C = answer & 0xff;

    state->flags.sign = state->C >> 7;
    state->flags.parity = !(__builtin_popcount(state->C) & 1);
    state->flags.zero = (0 == state->C);

    state->PC += 1;
}

// 0x0e
void MVI_C(cpuState *state) {
    state->C = state->memory[state->PC + 1];

    state->PC += 2;
}

// 0x0f
void RRC(cpuState *state) {
    state->flags.carry = state->A & 1;

    u16 answer = state->A >> 1;

    state->A = answer | (state->A << 7);

    state->PC += 1;
}

// 0x11
void LXI_DE(cpuState *state) {
    state->DE = readShort(state->memory, state->PC + 1);

    state->PC += 3;
}

// 0x12
void STAX_D(cpuState *state) {
    state->memory[state->DE] = state->A;

    state->PC += 1;
}

// 0x13
void INX_DE(cpuState *state) {
    state->DE += 1;

    state->PC += 1;
}

// 0x14
void INR_D(cpuState *state) {
    state->D = state->D + 1;

    state->flags.sign = state->D >> 7;
    state->flags.parity = !(__builtin_popcount(state->D) & 1);
    state->flags.zero = (0 == state->D);

    state->PC += 1;
}

// 0x15
void DCR_D(cpuState *state) {
    u16 answer = state->D - 1;
    state->D = answer & 0xff;

    state->flags.sign = state->D >> 7;
    state->flags.parity = !(__builtin_popcount(state->D) & 1);
    state->flags.zero = (0 == state->D);

    state->PC += 1;
}

// 0x16
void MVI_D(cpuState *state) {
    state->D = state->memory[state->PC + 1];

    state->PC += 2;
}

// 0x17
void RAL(cpuState *state) {
    u16 answer = state->A << 1;

    state->A = (answer & 0xff) | state->flags.carry;

    state->flags.carry = answer >> 8;

    state->PC += 1;
}

void DAD_DE(cpuState *state) {
    u32 answer = state->HL + state->DE;

    state->HL = answer & 0xffff;

    state->flags.carry = answer > 0xffff;

    state->PC += 1;
}

// 0x1a
void LDAX_DE(cpuState *state) {
    state->A = state->memory[state->DE];

    state->PC += 1;
}

// 0x1b
void DCX_DE(cpuState *state) {
    state->DE -= 1;

    state->PC += 1;
}

// 0x1c
void INR_E(cpuState *state) {
    state->E = state->E + 1;

    state->flags.sign = state->E >> 7;
    state->flags.parity = !(__builtin_popcount(state->E) & 1);
    state->flags.zero = (0 == state->E);

    state->PC += 1;
}

// 0x1d
void DCR_E(cpuState *state) {
    u16 answer = state->E - 1;
    state->E = answer & 0xff;

    state->flags.sign = state->E >> 7;
    state->flags.parity = !(__builtin_popcount(state->E) & 1);
    state->flags.zero = (0 == state->E);

    state->PC += 1;
}

// 0x1e
void MVI_E(cpuState *state) {
    state->E = state->memory[state->PC + 1];

    state->PC += 2;
}

// https://github.com/kpmiller/emulator101/issues/23
// 0x1f
void RAR(cpuState *state) {
    u8 answer = state->A >> 1;

    answer = answer | (state->flags.carry << 7);

    state->flags.carry = state->A & 1;

    state->A = answer;

    state->PC += 1;
}

// 0x21
void LXI_HL(cpuState *state) {
    state->HL = readShort(state->memory, state->PC + 1);

    state->PC += 3;
}

// 0x22
void SHLD(cpuState *state) {
    // Store the HL register at the place in memory pointed to by the address following the opcode
    writeShort(state->memory, readShort(state->memory, state->PC + 1), state->HL);

    state->PC += 3;
}

// 0x23
void INX_HL(cpuState *state) {
    state->HL += 1;

    state->PC += 1;
}

// 0x24
void INR_H(cpuState *state) {
    state->H = state->H + 1;

    state->flags.sign = state->H >> 7;
    state->flags.parity = !(__builtin_popcount(state->H) & 1);
    state->flags.zero = (0 == state->H);

    state->PC += 1;
}

// 0x25
void DCR_H(cpuState *state) {
    u16 answer = state->H - 1;
    state->H = answer & 0xff;

    state->flags.sign = state->H >> 7;
    state->flags.parity = !(__builtin_popcount(state->H) & 1);
    state->flags.zero = (0 == state->H);

    state->PC += 1;
}

// 0x26
void MVI_H(cpuState *state) {
    state->H = state->memory[state->PC + 1];

    state->PC += 2;
}

// 0x29
void DAD_HL(cpuState *state) {
    u32 answer = state->HL + state->HL;

    state->HL = answer & 0xffff;

    state->flags.carry = answer > 0xffff;

    state->PC += 1;
}

// 0x2a
void LHLD(cpuState *state) {
    state->HL = readShort(state->memory, readShort(state->memory, state->PC + 1));

    state->PC += 3;
}

// 0x2b
void DCX_HL(cpuState *state) {
    state->HL -= 1;

    state->PC += 1;
}

// 0x2c
void INR_L(cpuState *state) {
    state->L = state->L + 1;

    state->flags.sign = state->L >> 7;
    state->flags.parity = !(__builtin_popcount(state->L) & 1);
    state->flags.zero = (0 == state->L);

    state->PC += 1;
}

// 0x2d
void DCR_L(cpuState *state) {
    u16 answer = state->L - 1;
    state->L = answer & 0xff;

    state->flags.sign = state->L >> 7;
    state->flags.parity = !(__builtin_popcount(state->L) & 1);
    state->flags.zero = (0 == state->L);

    state->PC += 1;
}

// 0x2e
void MVI_L(cpuState *state) {
    state->L = state->memory[state->PC + 1];

    state->PC += 2;
}

// 0x2f
void CMA(cpuState *state) {
    state->A = ~(state->A);

    state->PC += 1;
}

// 0x31
void LXI_SP(cpuState *state) {
    state->SP = readShort(state->memory, state->PC + 1);

    state->PC += 3;
}

// 0x32
void STA(cpuState *state) {
    state->memory[readShort(state->memory, state->PC + 1)] = state->A;

    state->PC += 3;
}

// 0x33
void INX_SP(cpuState *state) {
    state->SP += 1;

    state->PC += 1;
}

// 0x34
void INR_M(cpuState *state) {
    state->memory[state->HL] = state->memory[state->HL] + 1;

    state->flags.sign = state->memory[state->HL] >> 7;
    state->flags.parity = !(__builtin_popcount(state->memory[state->HL]) & 1);
    state->flags.zero = (0 == state->memory[state->HL]);

    state->PC += 1;
}

// 0x35
void DCR_M(cpuState *state) {
    state->memory[state->HL] = state->memory[state->HL] - 1;

    state->flags.sign = state->memory[state->HL] >> 7;
    state->flags.parity = !(__builtin_popcount(state->memory[state->HL]) & 1);
    state->flags.zero = (0 == state->memory[state->HL]);

    state->PC += 1;
}

// 0x36
void MVI_M(cpuState *state) {
    state->memory[state->HL] = state->memory[state->PC + 1];

    state->PC += 2;
}

// 0x37
void STC(cpuState *state) {
    state->flags.carry = 1;

    state->PC += 1;
}

void DAD_SP(cpuState *state) {
    u32 answer = state->HL + state->SP;

    state->HL = answer & 0xffff;

    state->flags.carry = answer > 0xffff;

    state->PC += 1;
}

// 0x3a
void LDA(cpuState *state) {
    state->A = state->memory[readShort(state->memory, state->PC + 1)];

    state->PC += 3;
}
// 0x3b
void DCX_SP(cpuState *state) {
    state->SP -= 1;

    state->PC += 1;
}

// 0x3c
void INR_A(cpuState *state) {
    state->A = state->A + 1;

    state->flags.zero = !(state->A);
    state->flags.sign = state->A >> 7;
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0x3d
void DCR_A(cpuState *state) {
    // TODO: Implement AuxCarry

    u16 answer = state->A - 1;

    state->A = answer & 0xff;

    state->flags.zero = !(state->A);
    state->flags.sign = state->A >> 7;
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0x3e
void MVI_A(cpuState *state) {
    state->A = state->memory[state->PC + 1];

    state->PC += 2;
}

// 0x3f
void CMC(cpuState *state) {
    state->flags.carry = !state->flags.carry;

    state->PC += 1;
}

// 0x40
void MOV_B_B(cpuState *state) {
    state->B = state->B;

    state->PC += 1;
}

// 0x41
void MOV_B_C(cpuState *state) {
    state->B = state->C;

    state->PC += 1;
}

// 0x42
void MOV_B_D(cpuState *state) {
    state->B = state->D;

    state->PC += 1;
}

// 0x43
void MOV_B_E(cpuState *state) {
    state->B = state->E;

    state->PC += 1;
}

// 0x44
void MOV_B_H(cpuState *state) {
    state->B = state->H;

    state->PC += 1;
}

// 0x45
void MOV_B_L(cpuState *state) {
    state->B = state->L;

    state->PC += 1;
}

// 0x46
void MOV_B_M(cpuState *state) {
    state->B = state->memory[state->HL];

    state->PC += 1;
}

// 0x47
void MOV_B_A(cpuState *state) {
    state->B = state->A;

    state->PC += 1;
}

// 0x48
void MOV_C_B(cpuState *state) {
    state->C = state->B;

    state->PC += 1;
}

// 0x49
void MOV_C_C(cpuState *state) {
    state->C = state->C;

    state->PC += 1;
}

// 0x4a
void MOV_C_D(cpuState *state) {
    state->C = state->D;

    state->PC += 1;
}

// 0x4b
void MOV_C_E(cpuState *state) {
    state->C = state->E;

    state->PC += 1;
}

// 0x4c
void MOV_C_H(cpuState *state) {
    state->C = state->H;

    state->PC += 1;
}

// 0x4d
void MOV_C_L(cpuState *state) {
    state->C = state->L;

    state->PC += 1;
}

// 0x4e
void MOV_C_M(cpuState *state) {
    state->C = state->memory[state->HL];

    state->PC += 1;
}

// 0x4f
void MOV_C_A(cpuState *state) {
    state->C = state->A;

    state->PC += 1;
}

// 0x50
void MOV_D_B(cpuState *state) {
    state->D = state->B;

    state->PC += 1;
}

// 0x51
void MOV_D_C(cpuState *state) {
    state->D = state->C;

    state->PC += 1;
}

// 0x52
void MOV_D_D(cpuState *state) {
    state->D = state->D;

    state->PC += 1;
}

// 0x53
void MOV_D_E(cpuState *state) {
    state->D = state->E;

    state->PC += 1;
}

// 0x54
void MOV_D_H(cpuState *state) {
    state->D = state->H;

    state->PC += 1;
}

// 0x55
void MOV_D_L(cpuState *state) {
    state->D = state->L;

    state->PC += 1;
}

// 0x56
void MOV_D_M(cpuState *state) {
    state->D = state->memory[state->HL];

    state->PC += 1;
}

// 0x57
void MOV_D_A(cpuState *state) {
    state->D = state->A;

    state->PC += 1;
}

// 0x58
void MOV_E_B(cpuState *state) {
    state->E = state->B;

    state->PC += 1;
}

// 0x59
void MOV_E_C(cpuState *state) {
    state->E = state->C;

    state->PC += 1;
}

// 0x5a
void MOV_E_D(cpuState *state) {
    state->E = state->D;

    state->PC += 1;
}

// 0x5b
void MOV_E_E(cpuState *state) {
    state->E = state->E;

    state->PC += 1;
}

// 0x5c
void MOV_E_H(cpuState *state) {
    state->E = state->H;

    state->PC += 1;
}

// 0x5d
void MOV_E_L(cpuState *state) {
    state->E = state->L;

    state->PC += 1;
}

// 0x5e
void MOV_E_M(cpuState *state) {
    state->E = state->memory[state->HL];

    state->PC += 1;
}

// 0x5f
void MOV_E_A(cpuState *state) {
    state->E = state->A;

    state->PC += 1;
}

// 0x60
void MOV_H_B(cpuState *state) {
    state->H = state->B;

    state->PC += 1;
}

// 0x61
void MOV_H_C(cpuState *state) {
    state->H = state->C;

    state->PC += 1;
}

// 0x62
void MOV_H_D(cpuState *state) {
    state->H = state->D;

    state->PC += 1;
}

// 0x63
void MOV_H_E(cpuState *state) {
    state->H = state->E;

    state->PC += 1;
}

// 0x64
void MOV_H_H(cpuState *state) {
    state->H = state->H;

    state->PC += 1;
}

// 0x65
void MOV_H_L(cpuState *state) {
    state->H = state->L;

    state->PC += 1;
}

// 0x66
void MOV_H_M(cpuState *state) {
    state->H = state->memory[state->HL];

    state->PC += 1;
}

// 0x67
void MOV_H_A(cpuState *state) {
    state->H = state->A;

    state->PC += 1;
}

// 0x68
void MOV_L_B(cpuState *state) {
    state->L = state->B;

    state->PC += 1;
}

// 0x69
void MOV_L_C(cpuState *state) {
    state->L = state->C;

    state->PC += 1;
}

// 0x6a
void MOV_L_D(cpuState *state) {
    state->L = state->D;

    state->PC += 1;
}

// 0x6b
void MOV_L_E(cpuState *state) {
    state->L = state->E;

    state->PC += 1;
}

// 0x6c
void MOV_L_H(cpuState *state) {
    state->L = state->H;

    state->PC += 1;
}

// 0x6d
void MOV_L_L(cpuState *state) {
    state->L = state->L;

    state->PC += 1;
}

// 0x6e
void MOV_L_M(cpuState *state) {
    state->L = state->memory[state->HL];

    state->PC += 1;
}

// 0x6f
void MOV_L_A(cpuState *state) {
    state->L = state->A;

    state->PC += 1;
}

// 0x70
void MOV_M_B(cpuState *state) {
    state->memory[state->HL] = state->B;

    state->PC += 1;
}

// 0x71
void MOV_M_C(cpuState *state) {
    state->memory[state->HL] = state->C;

    state->PC += 1;
}

// 0x72
void MOV_M_D(cpuState *state) {
    state->memory[state->HL] = state->D;

    state->PC += 1;
}

// 0x73
void MOV_M_E(cpuState *state) {
    state->memory[state->HL] = state->E;

    state->PC += 1;
}

// 0x74
void MOV_M_H(cpuState *state) {
    state->memory[state->HL] = state->H;

    state->PC += 1;
}

// 0x75
void MOV_M_L(cpuState *state) {
    state->memory[state->HL] = state->L;

    state->PC += 1;
}

// 0x77
void MOV_M_A(cpuState *state) {
    state->memory[state->HL] = state->A;

    state->PC += 1;
}

// 0x78
void MOV_A_B(cpuState *state) {
    state->A = state->B;

    state->PC += 1;
}

// 0x79
void MOV_A_C(cpuState *state) {
    state->A = state->C;

    state->PC += 1;
}

// 0x7a
void MOV_A_D(cpuState *state) {
    state->A = state->D;

    state->PC += 1;
}

// 0x7b
void MOV_A_E(cpuState *state) {
    state->A = state->E;

    state->PC += 1;
}

// 0x7c
void MOV_A_H(cpuState *state) {
    state->A = state->H;

    state->PC += 1;
}

// 0x7d
void MOV_A_L(cpuState *state) {
    state->A = state->L;

    state->PC += 1;
}

// 0x7e
void MOV_A_M(cpuState *state) {
    state->A = state->memory[state->HL];

    state->PC += 1;
}

// 0x7f
void MOV_A_A(cpuState *state) {
    state->A = state->A;

    state->PC += 1;
}

// 0x80
void ADD_B(cpuState *state) {
    u16 answer = state->A + state->B;

    state->A = answer & 0xff;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = (0xff < answer);
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0x81
void ADD_C(cpuState *state) {
    u16 answer = state->A + state->C;

    state->A = answer & 0xff;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = (0xff < answer);
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0x82
void ADD_D(cpuState *state) {
    u16 answer = state->A + state->D;

    state->A = answer & 0xff;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = (0xff < answer);
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0x83
void ADD_E(cpuState *state) {
    u16 answer = state->A + state->E;

    state->A = answer & 0xff;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = (0xff < answer);
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0x84
void ADD_H(cpuState *state) {
    u16 answer = state->A + state->H;

    state->A = answer & 0xff;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = (0xff < answer);
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0x85
void ADD_L(cpuState *state) {
    u16 answer = state->A + state->L;

    state->A = answer & 0xff;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = (0xff < answer);
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0x86
void ADD_M(cpuState *state) {
    u16 answer = state->A + state->memory[state->HL];

    state->A = answer & 0xff;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = (0xff < answer);
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0x87
void ADD_A(cpuState *state) {
    u16 answer = state->A + state->A;

    state->A = answer & 0xff;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = (0xff < answer);
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0x88
void ADC_B(cpuState *state) {
    u16 answer = state->A + state->B + state->flags.carry;

    state->A = answer & 0xff;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = (0xff < answer);
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0x89
void ADC_C(cpuState *state) {
    u16 answer = state->A + state->C + state->flags.carry;

    state->A = answer & 0xff;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = (0xff < answer);
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0x8a
void ADC_D(cpuState *state) {
    u16 answer = state->A + state->D + state->flags.carry;

    state->A = answer & 0xff;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = (0xff < answer);
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0x8b
void ADC_E(cpuState *state) {
    u16 answer = state->A + state->E + state->flags.carry;

    state->A = answer & 0xff;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = (0xff < answer);
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0x8c
void ADC_H(cpuState *state) {
    u16 answer = state->A + state->H + state->flags.carry;

    state->A = answer & 0xff;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = (0xff < answer);
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0x8d
void ADC_L(cpuState *state) {
    u16 answer = state->A + state->L + state->flags.carry;

    state->A = answer & 0xff;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = (0xff < answer);
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0x8e
void ADC_M(cpuState *state) {
    u16 answer = state->A + state->memory[state->HL] + state->flags.carry;

    state->A = answer & 0xff;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = (0xff < answer);
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0x8f
void ADC_A(cpuState *state) {
    u16 answer = state->A + state->A + state->flags.carry;

    state->A = answer & 0xff;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = (0xff < answer);
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0x90
void SUB_B(cpuState *state) {
    u16 answer = state->A - state->B;

    state->A = answer & 0xff;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = (0xff < answer);
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0x91
void SUB_C(cpuState *state) {
    u16 answer = state->A - state->C;

    state->A = answer & 0xff;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = (0xff < answer);
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0x92
void SUB_D(cpuState *state) {
    u16 answer = state->A - state->D;

    state->A = answer & 0xff;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = (0xff < answer);
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0x93
void SUB_E(cpuState *state) {
    u16 answer = state->A - state->E;

    state->A = answer & 0xff;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = (0xff < answer);
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0x94
void SUB_H(cpuState *state) {
    u16 answer = state->A - state->H;

    state->A = answer & 0xff;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = (0xff < answer);
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0x95
void SUB_L(cpuState *state) {
    u16 answer = state->A - state->L;

    state->A = answer & 0xff;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = (0xff < answer);
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0x96
void SUB_M(cpuState *state) {
    u16 answer = state->A - state->memory[state->HL];

    state->A = answer & 0xff;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = (0xff < answer);
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0x97
void SUB_A(cpuState *state) {
    u16 answer = state->A - state->A;

    state->A = answer;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = (0xff < answer);
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0x98
void SBB_B(cpuState *state) {
    u16 answer = state->A - state->B - state->flags.carry;

    state->A = answer & 0xff;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = (0xff < answer);
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0x99
void SBB_C(cpuState *state) {
    u16 answer = state->A - state->C - state->flags.carry;

    state->A = answer & 0xff;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = (0xff < answer);
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0x9a
void SBB_D(cpuState *state) {
    u16 answer = state->A - state->D - state->flags.carry;

    state->A = answer & 0xff;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = (0xff < answer);
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0x9b
void SBB_E(cpuState *state) {
    u16 answer = state->A - state->E - state->flags.carry;

    state->A = answer & 0xff;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = (0xff < answer);
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0x9c
void SBB_H(cpuState *state) {
    u16 answer = state->A - state->H - state->flags.carry;

    state->A = answer & 0xff;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = (0xff < answer);
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0x9d
void SBB_L(cpuState *state) {
    u16 answer = state->A - state->L - state->flags.carry;

    state->A = answer & 0xff;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = (0xff < answer);
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0x9e
void SBB_M(cpuState *state) {
    u16 answer = state->A - state->memory[state->HL] - state->flags.carry;

    state->A = answer & 0xff;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = (0xff < answer);
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0x9f
void SBB_A(cpuState *state) {
    u16 answer = state->A - state->A - state->flags.carry;

    state->A = answer & 0xff;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = (0xff < answer);
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0xa0
void ANA_B(cpuState *state) {
    state->A = state->A & state->B;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = 0;
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0xa1
void ANA_C(cpuState *state) {
    state->A = state->A & state->C;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = 0;
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0xa2
void ANA_D(cpuState *state) {
    state->A = state->A & state->D;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = 0;
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0xa3
void ANA_E(cpuState *state) {
    state->A = state->A & state->E;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = 0;
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0xa4
void ANA_H(cpuState *state) {
    state->A = state->A & state->H;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = 0;
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0xa5
void ANA_L(cpuState *state) {
    state->A = state->A & state->L;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = 0;
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0xa6
void ANA_M(cpuState *state) {
    state->A = state->A & state->memory[state->HL];

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = 0;
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0xa7
void ANA_A(cpuState *state) {
    state->A = state->A & state->A;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = 0;
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}


// 0xa8
void XRA_B(cpuState *state) {
    state->A = state->A ^ state->B;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = 0;
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0xa9
void XRA_C(cpuState *state) {
    state->A = state->A ^ state->C;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = 0;
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0xaa
void XRA_D(cpuState *state) {
    state->A = state->A ^ state->D;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = 0;
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0xab
void XRA_E(cpuState *state) {
    state->A = state->A ^ state->E;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = 0;
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0xac
void XRA_H(cpuState *state) {
    state->A = state->A ^ state->H;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = 0;
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0xad
void XRA_L(cpuState *state) {
    state->A = state->A ^ state->L;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = 0;
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0xae
void XRA_M(cpuState *state) {
    state->A = state->A ^ state->memory[state->HL];

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = 0;
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0xaf
void XRA_A(cpuState *state) {
    state->A = state->A ^ state->A;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = 0;
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0xb0
void ORA_B(cpuState *state) {
    state->A = state->A | state->B;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = 0;
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0xb1
void ORA_C(cpuState *state) {
    state->A = state->A | state->C;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = 0;
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0xb2
void ORA_D(cpuState *state) {
    state->A = state->A | state->D;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = 0;
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0xb3
void ORA_E(cpuState *state) {
    state->A = state->A | state->E;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = 0;
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0xb4
void ORA_H(cpuState *state) {
    state->A = state->A | state->H;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = 0;
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0xb5
void ORA_L(cpuState *state) {
    state->A = state->A | state->L;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = 0;
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0xb6
void ORA_M(cpuState *state) {
    state->A = state->A | state->memory[state->HL];

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = 0;
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0xb7
void ORA_A(cpuState *state) {
    state->A = state->A | state->A;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = 0;
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 1;
}

// 0xb8
void CMP_B(cpuState *state) {
    u16 answer = state->A - state->B;

    state->flags.carry = (0xff < answer);

    answer = answer & 0xff;

    state->flags.sign = answer >> 7;
    state->flags.zero = (0 == answer);
    state->flags.parity = !(__builtin_popcount(answer) & 1);

    state->PC += 1;
}

// 0xb9
void CMP_C(cpuState *state) {
    u16 answer = state->A - state->C;

    state->flags.carry = (0xff < answer);

    answer = answer & 0xff;

    state->flags.sign = answer >> 7;
    state->flags.zero = (0 == answer);
    state->flags.parity = !(__builtin_popcount(answer) & 1);

    state->PC += 1;
}

// 0xba
void CMP_D(cpuState *state) {
    u16 answer = state->A - state->D;

    state->flags.carry = (0xff < answer);

    answer = answer & 0xff;

    state->flags.sign = answer >> 7;
    state->flags.zero = (0 == answer);
    state->flags.parity = !(__builtin_popcount(answer) & 1);

    state->PC += 1;
}

// 0xbb
void CMP_E(cpuState *state) {
    u16 answer = state->A - state->E;

    state->flags.carry = (0xff < answer);

    answer = answer & 0xff;

    state->flags.sign = answer >> 7;
    state->flags.zero = (0 == answer);
    state->flags.parity = !(__builtin_popcount(answer) & 1);

    state->PC += 1;
}

// 0xbc
void CMP_H(cpuState *state) {
    u16 answer = state->A - state->H;

    state->flags.carry = (0xff < answer);

    answer = answer & 0xff;

    state->flags.sign = answer >> 7;
    state->flags.zero = (0 == answer);
    state->flags.parity = !(__builtin_popcount(answer) & 1);

    state->PC += 1;
}

// 0xbd
void CMP_L(cpuState *state) {
    u16 answer = state->A - state->L;

    state->flags.carry = (0xff < answer);

    answer = answer & 0xff;

    state->flags.sign = answer >> 7;
    state->flags.zero = (0 == answer);
    state->flags.parity = !(__builtin_popcount(answer) & 1);

    state->PC += 1;
}

// 0xbe
void CMP_M(cpuState *state) {
    u16 answer = state->A - state->memory[state->HL];

    state->flags.carry = (0xff < answer);

    answer = answer & 0xff;

    state->flags.sign = answer >> 7;
    state->flags.zero = (0 == answer);
    state->flags.parity = !(__builtin_popcount(answer) & 1);

    state->PC += 1;
}

// 0xbf
void CMP_A(cpuState *state) {
    u16 answer = state->A - state->A;

    state->flags.carry = (0xff < answer);

    answer = answer & 0xff;

    state->flags.sign = answer >> 7;
    state->flags.zero = (0 == answer);
    state->flags.parity = !(__builtin_popcount(answer) & 1);

    state->PC += 1;
}

// 0xc0
void RNZ(cpuState *state) {
    if (!state->flags.zero) {
        state->PC = readShort(state->memory, state->SP - 1);
        state->SP += 2;
    } else {
        state->PC += 1;
    }
}

// 0xc1
void POP_B(cpuState *state) {
    state->BC = readShort(state->memory, state->SP);
    state->SP += 2;

    state->PC += 1;
}

// 0xc2
void JNZ(cpuState *state) {
    if (0 == state->flags.zero) {
        state->PC = readShort(state->memory, state->PC + 1);
    } else {
        state->PC += 3;
    }
}

// 0xc3
void JMP(cpuState *state) {


    if (0x0000 == readShort(state->memory, state->PC + 1)) {
        printf("\nWBOOT addressed jumped to,\nQuitting...");
        exit(EXIT_FAILURE);
    }


    state->PC = readShort(state->memory, state->PC + 1);
}

// 0xc4
void CNZ(cpuState *state) {
    state->PC += 3;

    if (0 == state->flags.zero) {
        writeShort(state->memory, state->SP - 3, state->PC);
        state->PC = readShort(state->memory, state->PC - 2);
        state->SP -= 2;
    }
}

// 0xc5
void PUSH_B(cpuState *state) {
    state->SP -= 2;
    writeShort(state->memory, state->SP, state->BC);

    state->PC += 1;
}

// 0xc6
void ADI(cpuState *state) {
    u16 answer = state->A + state->memory[state->PC + 1];

    state->A = answer & 0xff;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = (0xff < answer);
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 2;
}

// 0xc7
void RST_0(cpuState *state) {
    state->SP -= 2;
    writeShort(state->memory, state->SP - 1, state->PC);
    state->PC = 0x0000;
}

// 0xc8
void RZ(cpuState *state) {
    state->PC += 1;

    if (state->flags.zero) {
        state->PC = readShort(state->memory, state->SP - 1);
        state->SP += 2;
    }
}

// 0xc9
void RET(cpuState *state) {
    state->PC = readShort(state->memory, state->SP - 1);
    state->SP += 2;
}

// 0xca
void JZ(cpuState *state) {
    state->PC += 3;

    if (state->flags.zero) {
        state->PC = readShort(state->memory, state->PC - 2);
    }
}

// 0xcb
void CZ(cpuState *state) {
    state->PC += 3;

    if (state->flags.zero) {
        writeShort(state->memory, state->SP - 3, state->PC);
        state->PC = readShort(state->memory, state->PC - 2);
        state->SP -= 2;
    }
}

// 0xcd
void CALL(cpuState *state) {

    state->PC += 3;

    //state->SP -= 2;
    //writeShort(state->memory, state->SP - 1, state->PC);
    //state->PC = readShort(state->memory, state->PC - 2);


    // Code courtesy of http://www.emulator101.com/full-8080-emulation.html
    if (5 == readShort(state->memory, state->PC - 2)) {
        if (state->C == 9) {
            u16 offset = state->DE;
            char *str = &state->memory[offset + 3];//skip the prefix bytes
            printf("\n");
            while (*str != '$')
                printf("%c", *str++);
        } else if (state->C == 2) {

            printf("%c", state->E);
            printf("print char routine called\n");
        }
    } else if (0 == readShort(state->memory, state->PC - 2)) {
        exit(0);
    } else {

        state->SP -= 2;
        writeShort(state->memory, state->SP - 1, state->PC);
        state->PC = readShort(state->memory, state->PC - 2);
    }

}

// 0xce
void ACI(cpuState *state) {
    u16 answer = state->A + state->memory[state->PC + 1] + state->flags.carry;

    state->A = answer & 0xff;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = (0xff < answer);
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 2;
}

// 0xcf
void RST_1(cpuState *state) {
    state->SP -= 2;
    writeShort(state->memory, state->SP - 1, state->PC);
    state->PC = 0x0008;
}

// 0xd0
void RNC(cpuState *state) {
    if (!state->flags.carry) {
        state->PC = readShort(state->memory, state->SP - 1);
        state->SP += 2;
    } else {
        state->PC += 1;
    }
}

// 0xd1
void POP_D(cpuState *state) {
    state->DE = readShort(state->memory, state->SP);
    state->SP += 2;

    state->PC += 1;
}

// 0xd2
void JNC(cpuState *state) {
    if (!state->flags.carry) {
        state->PC = readShort(state->memory, state->PC + 1);
    } else {
        state->PC += 3;
    }
}

// 0xd4
void CNC(cpuState *state) {
    state->PC += 3;

    if (!state->flags.carry) {
        writeShort(state->memory, state->SP - 3, state->PC);
        state->PC = readShort(state->memory, state->PC - 2);
        state->SP -= 2;
    }
}

// 0xd5
void PUSH_D(cpuState *state) {
    state->SP -= 2;
    writeShort(state->memory, state->SP, state->DE);

    state->PC += 1;
}

// 0xd6
void SUI(cpuState *state) {
    u16 answer = state->A - state->memory[state->PC + 1];

    state->flags.carry = (0xff < answer);

    state->A = answer & 0xff;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 2;
}

// 0xd7
void RST_2(cpuState *state) {
    state->SP -= 2;
    writeShort(state->memory, state->SP - 1, state->PC);
    state->PC = 0x0010;
}

// 0xd8
void RC(cpuState *state) {
    if (state->flags.carry) {
        state->PC = readShort(state->memory, state->SP - 1);
        state->SP += 2;
    } else {
        state->PC += 1;
    }
}

// 0xda
void JC(cpuState *state) {
    if (state->flags.carry) {
        state->PC = readShort(state->memory, state->PC + 1);
    } else {
        state->PC += 3;
    }
}

// 0xdc
void CC(cpuState *state) {
    state->PC += 3;

    if (state->flags.carry) {
        state->SP -= 2;
        writeShort(state->memory, state->SP - 1, state->PC);
        state->PC = readShort(state->memory, state->PC - 2);
    }
}

// 0xde
void SBI(cpuState *state) {
    u16 answer = state->A - state->memory[state->PC + 1] - state->flags.carry;

    state->A = answer & 0xff;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = (0xff < answer);
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 2;
}

// 0xdf
void RST_3(cpuState *state) {
    state->SP -= 2;
    writeShort(state->memory, state->SP - 1, state->PC);
    state->PC = 0x0018;
}

// 0xe0
void RPO(cpuState *state) {
    if (!state->flags.parity) {
        state->PC = readShort(state->memory, state->SP - 1);
        state->SP += 2;
    } else {
        state->PC += 1;
    }
}

// 0xe1
void POP_H(cpuState *state) {
    state->HL = readShort(state->memory, state->SP);
    state->SP += 2;

    state->PC += 1;
}

// 0xe2
void JPO(cpuState *state) {
    if (!state->flags.parity) {
        state->PC = readShort(state->memory, state->PC + 1);
    } else {
        state->PC += 3;
    }
}

// 0xe3
void XTHL(cpuState *state) {
    u16 temp = readShort(state->memory, state->SP);
    writeShort(state->memory, state->SP, state->HL);
    state->HL = temp;

    state->PC += 1;
}

// 0xe4
void CPO(cpuState *state) {
    state->PC += 3;

    if (!state->flags.parity) {
        state->SP -= 2;
        writeShort(state->memory, state->SP - 1, state->PC);
        state->PC = readShort(state->memory, state->PC - 2);
    }
}

// 0xe5
void PUSH_H(cpuState *state) {
    state->SP -= 2;
    writeShort(state->memory, state->SP, state->HL);

    state->PC += 1;
}

// 0xe6
void ANI(cpuState *state) {
    u16 answer = state->A & state->memory[state->PC + 1];

    state->A = answer & 0xff;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = 0;
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 2;
}

// 0xe7
void RST_4(cpuState *state) {
    state->SP -= 2;
    writeShort(state->memory, state->SP - 1, state->PC);
    state->PC = 0x0020;
}

// 0xe8
void RPE(cpuState *state) {
    if (state->flags.parity) {
        state->PC = readShort(state->memory, state->SP - 1);
        state->SP += 2;
    } else {
        state->PC += 1;
    }
}

// 0xe9
void PCHL(cpuState *state) {
    state->PC = state->HL;

    //state->PC += 1;
}

// 0xea
void JPE(cpuState *state) {
    if (state->flags.parity) {
        state->PC = readShort(state->memory, state->PC + 1);
    } else {
        state->PC += 3;
    }
}

// 0xeb
void XCHG(cpuState *state) {
    u16 temp = state->HL;
    state->HL = state->DE;
    state->DE = temp;

    state->PC += 1;
}

// 0xec
void CPE(cpuState *state) {
    state->PC += 3;

    if (state->flags.parity) {
        state->SP -= 2;
        writeShort(state->memory, state->SP - 1, state->PC);
        state->PC = readShort(state->memory, state->PC - 2);
    }
}

// 0xee
void XRI(cpuState *state) {
    u16 answer = state->A ^ state->memory[state->PC + 1];

    state->A = answer & 0xff;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = 0;
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 2;
}

// 0xef
void RST_5(cpuState *state) {
    state->SP -= 2;
    writeShort(state->memory, state->SP - 1, state->PC);
    state->PC = 0x0028;
}

// 0xf0
void RP(cpuState *state) {
    if (state->flags.parity) {
        state->PC = readShort(state->memory, state->SP - 1);
        state->SP += 2;
    } else {
        state->PC += 1;
    }
}

// 0xf1
void POP_PSW(cpuState *state) {
    u16 answer = readShort(state->memory, state->SP);
    state->SP += 2;

    state->A = answer >> 8 & 0xff;

    state->flags.flagByte = answer & 0xff;

    state->PC += 1;
}

// 0xf2
void JP(cpuState *state) {
    if (state->flags.parity) {
        state->PC = readShort(state->memory, state->PC + 1);
    } else {
        state->PC += 3;
    }
}

// 0xf4
void CP(cpuState *state) {
    state->PC += 3;

    if (state->flags.parity) {
        state->SP -= 2;
        writeShort(state->memory, state->SP - 1, state->PC);
        state->PC = readShort(state->memory, state->PC - 2);
    }
}

// 0xf5
void PUSH_PSW(cpuState *state) {

    state->SP -= 2;
    writeShort(state->memory, state->SP, (state->A << 8) | state->flags.flagByte);

    state->PC += 1;
}

// 0xf6
void ORI(cpuState *state) {
    u16 answer = state->A | state->memory[state->PC + 1];

    state->A = answer & 0xff;

    state->flags.sign = state->A >> 7;
    state->flags.zero = (0 == state->A);
    state->flags.carry = 0;
    state->flags.parity = !(__builtin_popcount(state->A) & 1);

    state->PC += 2;
}

// 0xf7
void RST_6(cpuState *state) {
    state->SP -= 2;
    writeShort(state->memory, state->SP - 1, state->PC);
    state->PC = 0x0030;
}

// 0xf8
void RM(cpuState *state) {
    if (state->flags.sign) {
        state->PC = readShort(state->memory, state->SP - 1);
        state->SP += 2;
    } else {
        state->PC += 1;
    }
}

// 0xf9
void SPHL(cpuState *state) {
    state->SP = state->HL;

    state->PC += 1;
}

// 0xfa
void JM(cpuState *state) {
    if (state->flags.sign) {
        state->PC = readShort(state->memory, state->PC + 1);
    } else {
        state->PC += 3;
    }
}

// 0xfc
void CM(cpuState *state) {
    state->PC += 3;

    if (state->flags.sign) {
        state->SP -= 2;
        writeShort(state->memory, state->SP - 1, state->PC);
        state->PC = readShort(state->memory, state->PC - 2);
    }
}


// 0xfe
void CPI(cpuState *state) {
    u16 answer = state->A - state->memory[state->PC + 1];

    state->flags.carry = (0xff < answer);

    answer = answer & 0xff;

    state->flags.sign = answer >> 7;
    state->flags.zero = (0 == answer);
    state->flags.parity = !(__builtin_popcount(answer) & 1);

    state->PC += 2;
}

// 0xff
void RST_7(cpuState *state) {
    state->SP -= 2;
    writeShort(state->memory, state->SP - 1, state->PC);
    state->PC = 0x0038;
}