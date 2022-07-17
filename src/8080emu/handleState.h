#ifndef INC_8080EMU_HANDLESTATE_H
#define INC_8080EMU_HANDLESTATE_H

#include "../lib/intDefs.h"

typedef struct cpuFlags {

    // TODO: look into using bit fields and a union to u8
    // Pros: will make Pop/Push_PSW look nicer, will save space
    // Cons: possibly slower

    // https://retrocomputing.stackexchange.com/questions/12300/bit-one-of-the-intel-8080s-flags-register

    union {
        u8 flagByte;
        struct { // TODO - The oder of the flags in the flag byte may be backwards
            u8 sign : 1;
            u8 zero : 1;
            u8 _alwaysZero1 : 1;
            u8 auxCarry : 1;
            u8 _alwaysZero2 : 1;
            u8 parity : 1;
            u8 _alwaysOne : 1;
            u8 carry : 1;
        };
    };

} cpuFlags;

typedef struct cpuState {

    /*
     WARNING: Little-endian dependant use of unions below
     WARNING: This code is not portable to big-endian platforms.
     If you need to port this code to a big-endian architecture you will need to switch the order of the 8-bit registers.
    */

    // HL register
    union {
        u16 HL;
        struct {
            u8 L, H;
        };
    };

    // DE register
    union {
        u16 DE;
        struct {
            u8 E, D;
        };
    };

    // BC register
    union {
        u16 BC;
        struct {
            u8 C, B;
        };
    };

    u8 A;

    u16 SP;
    u16 PC;

    // TODO: change name to stateFlags
    struct cpuFlags flags;

    u8 *memory;

    // This is necessary to facilitate time travel debugging
    u64 totalInstructionCounter;
    u8 *initialMemory;

    // The necessary data to facilitate breakpoints
    u16 *breakpoints;
    u16 numBreakPoints;

} cpuState;

struct cpuState initState(void);
int emulateState(cpuState *state);
int emulateSteps(cpuState *state, int steps);
int dumpState(cpuState *state, const char *filename);
void loadROM(cpuState *state, const char *filePath, const u16 memOffset);
int emulateReplay(cpuState *state, int steps);


#endif//INC_8080EMU_HANDLESTATE_H
