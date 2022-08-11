//
// Created by Cole on 11/22/2021.
//

#include <stdio.h>

#include "handleState.h"
#include "opcodes.h"

#include "breakpoints.h"

struct cpuState initState(void) {

    // Initialize all values in the structure to zero
    // This action nulls pointers as well
    cpuState state = {0};

    // Initialize the necessary flag bytes
    state.flags._alwaysOne = 1;

    // Change UINT16_MAX to UINT16_MAX + 1 due to 0 being an addressable index.
    state.memory = calloc(UINT16_MAX + 1, sizeof(u8));

    state.initialMemory = calloc(UINT16_MAX + 1, sizeof(u8));


    if (NULL == state.memory) {
        //TODO: Find a way to notify caller if this function fails instead of exiting (something like the equivalent of returning null)
        exit(EXIT_FAILURE);
    }

    return state;
}

void loadROM(cpuState *state, const char *filePath, const u16 memOffset) {

    FILE *rfp;

    rfp = fopen(filePath, "rb");

    if (NULL == rfp) {
        printf("Could not open %s\nQuitting...\n", filePath);
        exit(EXIT_FAILURE);
    }

    fseek(rfp, 0, SEEK_END);
    size_t fileSize = ftell(rfp);
    fseek(rfp, 0, SEEK_SET);

    if (UINT16_MAX < fileSize + memOffset) {
        printf("The ROM cannot fit in program memory.\n Quitting...");
        exit(EXIT_FAILURE);
    }

    fread(state->memory + memOffset, 1, fileSize, rfp);

    fclose(rfp);

    // In order to facilitate the time travel aspect of the debugger save a copy of the initial state for future reference
    for(int i = 0; i < UINT16_MAX + 1; i++){
        state->initialMemory[i] = state->memory[i];
    }
}

int dumpState(cpuState *state, const char *filename) {
    FILE *wfp;

    wfp = fopen(filename, "wb");

    if (NULL == wfp) {
        printf("ERROR: Could not open %s\n", filename);
        return -1;
    }

    fwrite(state->memory, 65536, 1, wfp);

    fclose(wfp);

    return 0;
}


// IDEA: Use different int codes for break points and watch points
int emulateSteps(cpuState *state, int steps){

    for(int i = 0; i < steps; i++){

        emulateState(state);

        if(isBreakpointHit(state, state->PC) == 1){
            return 1; // 1 can indicate a break point was hit?
        }

    }

    return 0;
}

int emulateReplay(cpuState *state, int steps){

    // Restore the state of the memory before proceeding with emulation
    for(int i = 0; i < UINT16_MAX + 1; i++){
        state->memory[i] = state->initialMemory[i];
    }

    state->PC = 0;

    state->HL = 0;
    state->BC = 0;
    state->DE = 0;

    state->flags.flagByte = 0;
    state->flags._alwaysOne = 1;

    u64 temp = state->totalInstructionCounter;

    state->totalInstructionCounter = 0;

    return emulateSteps(state, temp - steps);
}

int emulateState(cpuState *state) {

    // Read the current instruction from memory
    u8 *currentOpcode = &state->memory[state->PC];

    state->totalInstructionCounter += 1;

    switch (*currentOpcode) {
        case 0x00:// NOP
        case 0x08:
        case 0x10:
        case 0x18:
        case 0x20:// RIM
        case 0x27:// DAA
        case 0x28:
        case 0x30:// SIM
        case 0x38:
        case 0x76:// HLT
        case 0xcb:
        case 0xd3:// OUT D8
        case 0xd9:
        case 0xdb:// IN D8
        case 0xdd:
        case 0xed:
        case 0xf3:// DI
        case 0xfb:// EI
        case 0xfd:
            NOP(state);
            break;
        case 0x01:
            LXI_BC(state);
            break;
        case 0x02:
            STAX_B(state);
            break;
        case 0x03:
            INX_BC(state);
            break;
        case 0x04:
            INR_B(state);
            break;
        case 0x05:
            DCR_B(state);
            break;
        case 0x06:
            MVI_B(state);
            break;
        case 0x07:
            RLC(state);
            break;
        case 0x09:
            DAD_BC(state);
            break;
        case 0x0a:
            LDAX_BC(state);
            break;
        case 0x0b:
            DCX_BC(state);
            break;
        case 0x0c:
            INR_C(state);
            break;
        case 0x0d:
            DCR_C(state);
            break;
        case 0x0e:
            MVI_C(state);
            break;
        case 0x0f:
            RRC(state);
            break;
        case 0x11:
            LXI_DE(state);
            break;
        case 0x12:
            STAX_D(state);
            break;
        case 0x13:
            INX_DE(state);
            break;
        case 0x14:
            INR_D(state);
            break;
        case 0x15:
            DCR_D(state);
            break;
        case 0x16:
            MVI_D(state);
            break;
        case 0x17:
            RAL(state);
            break;
        case 0x19:
            DAD_DE(state);
            break;
        case 0x1a:
            LDAX_DE(state);
            break;
        case 0x1b:
            DCX_DE(state);
            break;
        case 0x1c:
            INR_E(state);
            break;
        case 0x1d:
            DCR_E(state);
            break;
        case 0x1e:
            MVI_E(state);
            break;
        case 0x1f:
            RAR(state);
            break;
        case 0x21:
            LXI_HL(state);
            break;
        case 0x22:
            SHLD(state);
            break;
        case 0x23:
            INX_HL(state);
            break;
        case 0x24:
            INR_H(state);
            break;
        case 0x25:
            DCR_H(state);
            break;
        case 0x26:
            MVI_H(state);
            break;
        case 0x29:
            DAD_HL(state);
            break;
        case 0x2a:
            LHLD(state);
            break;
        case 0x2b:
            DCX_HL(state);
            break;
        case 0x2c:
            INR_L(state);
            break;
        case 0x2d:
            DCR_L(state);
            break;
        case 0x2e:
            MVI_L(state);
            break;
        case 0x2f:
            CMA(state);
            break;
        case 0x31:
            LXI_SP(state);
            break;
        case 0x32:
            STA(state);
            break;
        case 0x33:
            INX_SP(state);
            break;
        case 0x34:
            INR_M(state);
            break;
        case 0x35:
            DCR_M(state);
            break;
        case 0x36:
            MVI_M(state);
            break;
        case 0x37:
            STC(state);
            break;
        case 0x39:
            DAD_SP(state);
            break;
        case 0x3a:
            LDA(state);
            break;
        case 0x3b:
            DCX_SP(state);
            break;
        case 0x3c:
            INR_A(state);
            break;
        case 0x3d:
            DCR_A(state);
            break;
        case 0x3e:
            MVI_A(state);
            break;
        case 0x3f:
            CMC(state);
            break;
        case 0x40:
            MOV_B_B(state);
            break;
        case 0x41:
            MOV_B_C(state);
            break;
        case 0x42:
            MOV_B_D(state);
            break;
        case 0x43:
            MOV_B_E(state);
            break;
        case 0x44:
            MOV_B_H(state);
            break;
        case 0x45:
            MOV_B_L(state);
            break;
        case 0x46:
            MOV_B_M(state);
            break;
        case 0x47:
            MOV_B_A(state);
            break;
        case 0x48:
            MOV_C_B(state);
            break;
        case 0x49:
            MOV_C_C(state);
            break;
        case 0x4a:
            MOV_C_D(state);
            break;
        case 0x4b:
            MOV_C_E(state);
            break;
        case 0x4c:
            MOV_C_H(state);
            break;
        case 0x4d:
            MOV_C_L(state);
            break;
        case 0x4e:
            MOV_C_M(state);
            break;
        case 0x4f:
            MOV_C_A(state);
            break;
        case 0x50:
            MOV_D_B(state);
            break;
        case 0x51:
            MOV_D_C(state);
            break;
        case 0x52:
            MOV_D_D(state);
            break;
        case 0x53:
            MOV_D_E(state);
            break;
        case 0x54:
            MOV_D_H(state);
            break;
        case 0x55:
            MOV_D_L(state);
            break;
        case 0x56:
            MOV_D_M(state);
            break;
        case 0x57:
            MOV_D_A(state);
            break;
        case 0x58:
            MOV_E_B(state);
            break;
        case 0x59:
            MOV_E_C(state);
            break;
        case 0x5a:
            MOV_E_D(state);
            break;
        case 0x5b:
            MOV_E_E(state);
            break;
        case 0x5c:
            MOV_E_H(state);
            break;
        case 0x5d:
            MOV_E_L(state);
            break;
        case 0x5e:
            MOV_E_M(state);
            break;
        case 0x5f:
            MOV_E_A(state);
            break;
        case 0x60:
            MOV_H_B(state);
            break;
        case 0x61:
            MOV_H_C(state);
            break;
        case 0x62:
            MOV_H_D(state);
            break;
        case 0x63:
            MOV_H_E(state);
            break;
        case 0x64:
            MOV_H_H(state);
            break;
        case 0x65:
            MOV_H_L(state);
            break;
        case 0x66:
            MOV_H_M(state);
            break;
        case 0x67:
            MOV_H_A(state);
            break;
        case 0x68:
            MOV_L_B(state);
            break;
        case 0x69:// nice
            MOV_L_C(state);
            break;
        case 0x6a:
            MOV_L_D(state);
            break;
        case 0x6b:
            MOV_L_E(state);
            break;
        case 0x6c:
            MOV_L_H(state);
            break;
        case 0x6d:
            MOV_L_L(state);
            break;
        case 0x6e:
            MOV_L_M(state);
            break;
        case 0x6f:
            MOV_L_A(state);
            break;
        case 0x70:
            MOV_M_B(state);
            break;
        case 0x71:
            MOV_M_C(state);
            break;
        case 0x72:
            MOV_M_D(state);
            break;
        case 0x73:
            MOV_M_E(state);
            break;
        case 0x74:
            MOV_M_H(state);
            break;
        case 0x75:
            MOV_M_L(state);
            break;
        case 0x77:
            MOV_M_A(state);
            break;
        case 0x78:
            MOV_A_B(state);
            break;
        case 0x79:
            MOV_A_C(state);
            break;
        case 0x7a:
            MOV_A_D(state);
            break;
        case 0x7b:
            MOV_A_E(state);
            break;
        case 0x7c:
            MOV_A_H(state);
            break;
        case 0x7d:
            MOV_A_L(state);
            break;
        case 0x7e:
            MOV_A_M(state);
            break;
        case 0x7f:
            MOV_A_A(state);
            break;
        case 0x80:
            ADD_B(state);
            break;
        case 0x81:
            ADD_C(state);
            break;
        case 0x82:
            ADD_D(state);
            break;
        case 0x83:
            ADD_E(state);
            break;
        case 0x84:
            ADD_H(state);
            break;
        case 0x85:
            ADD_L(state);
            break;
        case 0x86:
            ADD_M(state);
            break;
        case 0x87:
            ADD_A(state);
            break;
        case 0x88:
            ADC_B(state);
            break;
        case 0x89:
            ADC_C(state);
            break;
        case 0x8a:
            ADC_D(state);
            break;
        case 0x8b:
            ADC_E(state);
            break;
        case 0x8c:
            ADC_H(state);
            break;
        case 0x8d:
            ADC_L(state);
            break;
        case 0x8e:
            ADC_M(state);
            break;
        case 0x8f:
            ADC_A(state);
            break;
        case 0x90:
            SUB_B(state);
            break;
        case 0x91:
            SUB_C(state);
            break;
        case 0x92:
            SUB_D(state);
            break;
        case 0x93:
            SUB_E(state);
            break;
        case 0x94:
            SUB_H(state);
            break;
        case 0x95:
            SUB_L(state);
            break;
        case 0x96:
            SUB_M(state);
            break;
        case 0x97:
            SUB_A(state);
            break;
        case 0x98:
            SBB_B(state);
            break;
        case 0x99:
            SBB_C(state);
            break;
        case 0x9a:
            SBB_D(state);
            break;
        case 0x9b:
            SBB_E(state);
            break;
        case 0x9c:
            SBB_H(state);
            break;
        case 0x9d:
            SBB_L(state);
            break;
        case 0x9e:
            SBB_M(state);
            break;
        case 0x9f:
            SBB_A(state);
            break;
        case 0xa0:
            ANA_B(state);
            break;
        case 0xa1:
            ANA_C(state);
            break;
        case 0xa2:
            ANA_D(state);
            break;
        case 0xa3:
            ANA_E(state);
            break;
        case 0xa4:
            ANA_H(state);
            break;
        case 0xa5:
            ANA_L(state);
            break;
        case 0xa6:
            ANA_M(state);
            break;
        case 0xa7:
            ANA_A(state);
            break;
        case 0xa8:
            XRA_B(state);
            break;
        case 0xa9:
            XRA_C(state);
            break;
        case 0xaa:
            XRA_D(state);
            break;
        case 0xab:
            XRA_E(state);
            break;
        case 0xac:
            XRA_H(state);
            break;
        case 0xad:
            XRA_L(state);
            break;
        case 0xae:
            XRA_M(state);
            break;
        case 0xaf:
            XRA_A(state);
            break;
        case 0xb0:
            ORA_B(state);
            break;
        case 0xb1:
            ORA_C(state);
            break;
        case 0xb2:
            ORA_D(state);
            break;
        case 0xb3:
            ORA_E(state);
            break;
        case 0xb4:
            ORA_H(state);
            break;
        case 0xb5:
            ORA_L(state);
            break;
        case 0xb6:
            ORA_M(state);
            break;
        case 0xb7:
            ORA_A(state);
            break;
        case 0xb8:
            CMP_B(state);
            break;
        case 0xb9:
            CMP_C(state);
            break;
        case 0xba:
            CMP_D(state);
            break;
        case 0xbb:
            CMP_E(state);
            break;
        case 0xbc:
            CMP_H(state);
            break;
        case 0xbd:
            CMP_L(state);
            break;
        case 0xbe:
            CMP_M(state);
            break;
        case 0xbf:
            CMP_A(state);
            break;
        case 0xc0:
            RNZ(state);
            break;
        case 0xc1:
            POP_B(state);
            break;
        case 0xc2:
            JNZ(state);
            break;
        case 0xc3:
            JMP(state);
            break;
        case 0xc4:
            CNZ(state);
            break;
        case 0xc5:
            PUSH_B(state);
            break;
        case 0xc6:
            ADI(state);
            break;
        case 0xc7:
            RST_0(state);
            break;
        case 0xc8:
            RZ(state);
            break;
        case 0xc9:
            RET(state);
            break;
        case 0xca:
            JZ(state);
            break;
        case 0xcc:
            CZ(state);
            break;
        case 0xcd:
            CALL(state);
            break;
        case 0xce:
            ACI(state);
            break;
        case 0xcf:
            RST_1(state);
            break;
        case 0xd0:
            RNC(state);
            break;
        case 0xd1:
            POP_D(state);
            break;
        case 0xd2:
            JNC(state);
            break;
        case 0xd4:
            CNC(state);
            break;
        case 0xd5:
            PUSH_D(state);
            break;
        case 0xd6:
            SUI(state);
            break;
        case 0xd7:
            RST_2(state);
            break;
        case 0xd8:
            RC(state);
            break;
        case 0xda:
            JC(state);
            break;
        case 0xdc:
            CC(state);
            break;
        case 0xde:
            SBI(state);
            break;
        case 0xdf:
            RST_3(state);
            break;
        case 0xe0:
            RPO(state);
            break;
        case 0xe1:
            POP_H(state);
            break;
        case 0xe2:
            JPO(state);
            break;
        case 0xe3:
            XTHL(state);
            break;
        case 0xe4:
            CPO(state);
            break;
        case 0xe5:
            PUSH_H(state);
            break;
        case 0xe6:
            ANI(state);
            break;
        case 0xe7:
            RST_4(state);
            break;
        case 0xe8:
            RPE(state);
            break;
        case 0xe9:
            PCHL(state);
            break;
        case 0xea:
            JPE(state);
            break;
        case 0xeb:
            XCHG(state);
            break;
        case 0xec:
            CPE(state);
            break;
        case 0xee:
            XRI(state);
            break;
        case 0xef:
            RST_5(state);
            break;
        case 0xf0:
            RP(state);
            break;
        case 0xf1:
            POP_PSW(state);
            break;
        case 0xf2:
            JP(state);
            break;
        case 0xf4:
            CP(state);
            break;
        case 0xf5:
            PUSH_PSW(state);
            break;
        case 0xf6:
            ORI(state);
            break;
        case 0xf7:
            RST_6(state);
            break;
        case 0xf8:
            RM(state);
            break;
        case 0xf9:
            SPHL(state);
            break;
        case 0xfa:
            JM(state);
            break;
        case 0xfc:
            CM(state);
            break;
        case 0xfe:
            CPI(state);
            break;
        case 0xff:
            RST_7(state);
            break;
    }

    return 0;
}
