//
// Created by cole on 11/28/21.
//

#ifndef INC_8080EMU_OPCODES_H
#define INC_8080EMU_OPCODES_H

#include <stdlib.h>

#include "../lib/intDefs.h"
#include "handleState.h"

// 0x00
void NOP(cpuState *state);

// 0x01
void LXI_BC(cpuState *state);

// 0x02
void STAX_B(cpuState *state);

// 0x03
void INX_BC(cpuState *state);

// 0x04
void INR_B(cpuState *state);

// 0x05
void DCR_B(cpuState *state);

// 0x06
void MVI_B(cpuState *state);

// 0x07
void RLC(cpuState *state);

// 0x09
void DAD_BC(cpuState *state);

// 0x0a
void LDAX_BC(cpuState *state);

// 0x0b
void DCX_BC(cpuState *state);

// 0x0c
void INR_C(cpuState *state);

// 0x0d
void DCR_C(cpuState *state);

// 0x0e
void MVI_C(cpuState *state);

// 0x0f
void RRC(cpuState *state);

// 0x11
void LXI_DE(cpuState *state);

// 0x12
void STAX_D(cpuState *state);

// 0x13
void INX_DE(cpuState *state);

// 0x14
void INR_D(cpuState *state);

// 0x15
void DCR_D(cpuState *state);

// 0x16
void MVI_D(cpuState *state);

// 0x17
void RAL(cpuState *state);

// 0x19
void DAD_DE(cpuState *state);

// 0x1a
void LDAX_DE(cpuState *state);

// 0x1b
void DCX_DE(cpuState *state);

// 0x1c
void INR_E(cpuState *state);

// 0x1d
void DCR_E(cpuState *state);

// 0x1e
void MVI_E(cpuState *state);

// 0x1f
void RAR(cpuState *state);

// 0x21
void LXI_HL(cpuState *state);

// 0x22
void SHLD(cpuState *state);

// 0x23
void INX_HL(cpuState *state);

// 0x24
void INR_H(cpuState *state);

// 0x25
void DCR_H(cpuState *state);

// 0x26
void MVI_H(cpuState *state);

// 0x29
void DAD_HL(cpuState *state);

// 0x2a
void LHLD(cpuState *state);

// 0x2b
void DCX_HL(cpuState *state);

// 0x2c
void INR_L(cpuState *state);

// 0x2d
void DCR_L(cpuState *state);

// 0x2e
void MVI_L(cpuState *state);

// 0x2f
void CMA(cpuState *state);

// 0x31
void LXI_SP(cpuState *state);

// 0x32
void STA(cpuState *state);

// 0x33
void INX_SP(cpuState *state);

// 0x34
void INR_M(cpuState *state);

// 0x35
void DCR_M(cpuState *state);

// 0x36
void MVI_M(cpuState *state);

// 0x37
void STC(cpuState *state);

// 0x39
void DAD_SP(cpuState *state);

// 0x3a
void LDA(cpuState *state);

// 0x3b
void DCX_SP(cpuState *state);

// 0x3c
void INR_A(cpuState *state);

// 0x3d
void DCR_A(cpuState *state);

// 0x3e
void MVI_A(cpuState *state);

// 0x3f
void CMC(cpuState *state);

// 0x40
void MOV_B_B(cpuState *state);

// 0x41
void MOV_B_C(cpuState *state);

// 0x42
void MOV_B_D(cpuState *state);

// 0x43
void MOV_B_E(cpuState *state);

// 0x44
void MOV_B_H(cpuState *state);

// 0x45
void MOV_B_L(cpuState *state);

// 0x46
void MOV_B_M(cpuState *state);

// 0x47
void MOV_B_A(cpuState *state);

// 0x48
void MOV_C_B(cpuState *state);

// 0x49
void MOV_C_C(cpuState *state);

// 0x4a
void MOV_C_D(cpuState *state);

// 0x4b
void MOV_C_E(cpuState *state);

// 0x4c
void MOV_C_H(cpuState *state);

// 0x4d
void MOV_C_L(cpuState *state);

// 0x4e
void MOV_C_M(cpuState *state);

// 0x4f
void MOV_C_A(cpuState *state);

// 0x50
void MOV_D_B(cpuState *state);

// 0x51
void MOV_D_C(cpuState *state);

// 0x52
void MOV_D_D(cpuState *state);

// 0x53
void MOV_D_E(cpuState *state);

// 0x54
void MOV_D_H(cpuState *state);

// 0x55
void MOV_D_L(cpuState *state);

// 0x56
void MOV_D_M(cpuState *state);

// 0x57
void MOV_D_A(cpuState *state);

// 0x58
void MOV_E_B(cpuState *state);

// 0x59
void MOV_E_C(cpuState *state);

// 0x5a
void MOV_E_D(cpuState *state);

// 0x5b
void MOV_E_E(cpuState *state);

// 0x5c
void MOV_E_H(cpuState *state);

// 0x5d
void MOV_E_L(cpuState *state);

// 0x5e
void MOV_E_M(cpuState *state);

// 0x5f
void MOV_E_A(cpuState *state);

// 0x60
void MOV_H_B(cpuState *state);

// 0x61
void MOV_H_C(cpuState *state);

// 0x62
void MOV_H_D(cpuState *state);

// 0x63
void MOV_H_E(cpuState *state);

// 0x64
void MOV_H_H(cpuState *state);

// 0x65
void MOV_H_L(cpuState *state);

// 0x66
void MOV_H_M(cpuState *state);

// 0x67
void MOV_H_A(cpuState *state);

// 0x68
void MOV_L_B(cpuState *state);

// 0x69
void MOV_L_C(cpuState *state);

// 0x6a
void MOV_L_D(cpuState *state);

// 0x6b
void MOV_L_E(cpuState *state);

// 0x6c
void MOV_L_H(cpuState *state);

// 0x6d
void MOV_L_L(cpuState *state);

// 0x6e
void MOV_L_M(cpuState *state);

// 0x6f
void MOV_L_A(cpuState *state);

// 0x70
void MOV_M_B(cpuState *state);

// 0x71
void MOV_M_C(cpuState *state);

// 0x72
void MOV_M_D(cpuState *state);

// 0x73
void MOV_M_E(cpuState *state);

// 0x74
void MOV_M_H(cpuState *state);

// 0x75
void MOV_M_L(cpuState *state);

// 0x77
void MOV_M_A(cpuState *state);

// 0x78
void MOV_A_B(cpuState *state);

// 0x79
void MOV_A_C(cpuState *state);

// 0x7a
void MOV_A_D(cpuState *state);

// 0x7b
void MOV_A_E(cpuState *state);

// 0x7c
void MOV_A_H(cpuState *state);

// 0x7d
void MOV_A_L(cpuState *state);

// 0x7e
void MOV_A_M(cpuState *state);

// 0x7f
void MOV_A_A(cpuState *state);

// 0x80
void ADD_B(cpuState *state);

// 0x81
void ADD_C(cpuState *state);

// 0x82
void ADD_D(cpuState *state);

// 0x83
void ADD_E(cpuState *state);

// 0x84
void ADD_H(cpuState *state);

// 0x85
void ADD_L(cpuState *state);

// 0x86
void ADD_M(cpuState *state);

// 0x87
void ADD_A(cpuState *state);

// 0x88
void ADC_B(cpuState *state);

// 0x89
void ADC_C(cpuState *state);

// 0x8a
void ADC_D(cpuState *state);

// 0x8b
void ADC_E(cpuState *state);

// 0x8c
void ADC_H(cpuState *state);

// 0x8d
void ADC_L(cpuState *state);

// 0x8e
void ADC_M(cpuState *state);

// 0x8f
void ADC_A(cpuState *state);

// 0x90
void SUB_B(cpuState *state);

// 0x91
void SUB_C(cpuState *state);

// 0x92
void SUB_D(cpuState *state);

// 0x93
void SUB_E(cpuState *state);

// 0x94
void SUB_H(cpuState *state);

// 0x95
void SUB_L(cpuState *state);

// 0x96
void SUB_M(cpuState *state);

// 0x97
void SUB_A(cpuState *state);

// 0x99
void SBB_B(cpuState *state);

// 0x99
void SBB_C(cpuState *state);

// 0x9a
void SBB_D(cpuState *state);

// 0x9b
void SBB_E(cpuState *state);

// 0x9c
void SBB_H(cpuState *state);

//0x9d
void SBB_L(cpuState *state);

// 0x9e
void SBB_M(cpuState *state);

// 0x9f
void SBB_A(cpuState *state);

// 0xa0
void ANA_B(cpuState *state);

// 0xa1
void ANA_C(cpuState *state);

// 0xa2
void ANA_D(cpuState *state);

// 0xa3
void ANA_E(cpuState *state);

// 0xa4
void ANA_H(cpuState *state);

// 0xa5
void ANA_L(cpuState *state);

// 0xa6
void ANA_M(cpuState *state);

// 0xa7
void ANA_A(cpuState *state);

// 0xa8
void XRA_B(cpuState *state);

// 0xa9
void XRA_C(cpuState *state);

// 0xaa
void XRA_D(cpuState *state);

// 0xab
void XRA_E(cpuState *state);

// 0xac
void XRA_H(cpuState *state);

// 0xad
void XRA_L(cpuState *state);

// 0xae
void XRA_M(cpuState *state);

// 0xaf
void XRA_A(cpuState *state);

// 0xb0
void ORA_B(cpuState *state);

// 0xb1
void ORA_C(cpuState *state);

// 0xb2
void ORA_D(cpuState *state);

// 0xb3
void ORA_E(cpuState *state);

// 0xb4
void ORA_H(cpuState *state);

// 0xb5
void ORA_L(cpuState *state);

// 0xb6
void ORA_M(cpuState *state);

// 0xb7
void ORA_A(cpuState *state);

// 0xb8
void CMP_B(cpuState *state);

// 0xb9
void CMP_C(cpuState *state);

// 0xba
void CMP_D(cpuState *state);

// 0xbb
void CMP_E(cpuState *state);

// 0xbc
void CMP_H(cpuState *state);

// 0xbd
void CMP_L(cpuState *state);

// 0xbe
void CMP_M(cpuState *state);

// 0xbf
void CMP_A(cpuState *state);

// 0xc0
void RNZ(cpuState *state);

// 0xc1
void POP_B(cpuState *state);

// 0xc2
void JNZ(cpuState *state);

// 0xc3
void JMP(cpuState *state);

// 0xc4
void CNZ(cpuState *state);

// 0xc5
void PUSH_B(cpuState *state);

// 0xc6
void ADI(cpuState *state);

// 0xc7
void RST_0(cpuState *state);

// 0xc8
void RZ(cpuState *state);

// 0xc9
void RET(cpuState *state);

// 0xca
void JZ(cpuState *state);

// 0xcc
void CZ(cpuState *state);

// 0xcd
void CALL(cpuState *state);

// 0xce
void ACI(cpuState *state);

// 0xcf
void RST_1(cpuState *state);

// 0xd0
void RNC(cpuState *state);

// 0xd1
void POP_D(cpuState *state);

// 0xd2
void JNC(cpuState *state);

// 0xd4
void CNC(cpuState *state);

// 0xd5
void PUSH_D(cpuState *state);

// 0xd6
void SUI(cpuState *state);

// 0xd7
void RST_2(cpuState *state);

// 0xd8
void RC(cpuState *state);

// 0xda
void JC(cpuState *state);

// 0xdc
void CC(cpuState *state);

// 0xde
void SBI(cpuState *state);

// 0xdf
void RST_3(cpuState *state);

// 0xe0
void RPO(cpuState *state);

// 0xe1
void POP_H(cpuState *state);

// 0xe2
void JPO(cpuState *state);

// 0xe3
void XTHL(cpuState *state);

// 0xe4
void CPO(cpuState *state);

// 0xe5
void PUSH_H(cpuState *state);

// 0xe6
void ANI(cpuState *state);

// 0xe7
void RST_4(cpuState *state);

// 0xe8
void RPE(cpuState *state);

// 0xe9
void PCHL(cpuState *state);

// 0xea
void JPE(cpuState *state);

// 0xeb
void XCHG(cpuState *state);

// 0xec
void CPE(cpuState *state);

// 0xee
void XRI(cpuState *state);

// 0xef
void RST_5(cpuState *state);

// 0xf0
void RP(cpuState *state);

// 0xf1
void POP_PSW(cpuState *state);

// 0xf2
void JP(cpuState *state);

// 0xf4
void CP(cpuState *state);

// 0xf5
void PUSH_PSW(cpuState *state);

// 0xf6
void ORI(cpuState *state);

// 0xf7
void RST_6(cpuState *state);

// 0xf8
void RM(cpuState *state);

// 0xf9
void SPHL(cpuState *state);

// 0xfa
void JM(cpuState *state);

// 0xfc
void CM(cpuState *state);

// 0xfe
void CPI(cpuState *state);

// 0xff
void RST_7(cpuState *state);


#endif//INC_8080EMU_OPCODES_H
