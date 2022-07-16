//
// Created by cole on 11/16/21.
//

#include <stdio.h>


#include "disassemble8080.h"

/* Register view for reference
 * |bit|-----------8 bits-----------|------------16 bits-------------|
 * | S | Z | - | AC | - | P | - | C |   <-- flags                    |
 * |--------------------------------|--------------------------------|
 * |            A (Accumulator)     |                                |
 * |--------------------------------|--------------------------------|
 * |            B Register          |            C Register          |
 * |--------------------------------|--------------------------------|
 * |            D Register          |            E Register          |
 * |--------------------------------|--------------------------------|
 * |            H Register          |            L Register          | (HL register pair is sometimes referred to as M)
 * |-----------------------------------------------------------------|
 * |                        Stack Pointer                            |
 * |-----------------------------------------------------------------|
 * |                        Program Counter                          |
 * |-----------------------------------------------------------------|
 *
 * Instruction ref:
 * http://www.emulator101.com/reference/8080-by-opcode.html
 *
 */

// Disassemble
void disassembleMulti(u8 *memory, u16 currentInstruction, int steps){
    int opLen = 0;

    for(int i = 0; i < steps; i++){
        opLen += disassemble(memory, currentInstruction + opLen);
    }

    puts("");
}

int disassemble(u8 *memory, u16 currentInstruction){

    unsigned char *code = &memory[currentInstruction];
    int opbytes = 1;
    printf("0x%04x ", currentInstruction);

       // 8080 opcode ref
       // http://textfiles.com/programming/8080.op
       // http://www.emulator101.com/reference/8080-by-opcode.html
    switch (*code) {
               // The classic NOP
               // Does nothing
        case 0x00: printf("NOP"); break;
               // Loads code[2] into the B register and code[1] into the C register
        case 0x01: printf("LXI    BC,  B <- 0x%02x C <- 0x%02x", code[2], code[1]); opbytes=3; break;
               // Stores the contents of the accumulator register (A) to the address pointed at by
               // the extended register (BC)
               // Ref: https://www.tutorialspoint.com/instruction-type-stax-rp-in-8085-microprocessor
        case 0x02: printf("STAX   B"); break;
               // Increments the extended register BC by 1
        case 0x03: printf("INX    B"); break;
               // Increment the B register by 1
        case 0x04: printf("INR    B"); break;
               // De-increment the B register by 1
        case 0x05: printf("DCR    B"); break;

        case 0x06: printf("MVI    B <- 0x%02x", code[1]); opbytes = 2; break;
               // Rotate Accumulator Left thru Carry:  Bit0=Bit7  C=Bit7.
        case 0x07: printf("RLC"); break;

        case 0x08: printf("NOP"); break;
               // HL = HL + BL
        case 0x09: printf("DAD    B"); break;
               // Load the memory address pointed to by (BL) into the Accumulator
        case 0x0a: printf("LDAX   B"); break;
               // BC = BC-1
        case 0x0b: printf("DCX    B"); break;
               // C <- C+1
        case 0x0c: printf("INR    C"); break;
               // 	C <-C-1
        case 0x0d: printf("DCR    C"); break;
               // 	C <- byte 2
        case 0x0e: printf("MVI    C <- 0x%02x", code[1]); opbytes = 2; break;
               // A = A >> 1; bit 7 = prev bit 0; CY = prev bit 0
        case 0x0f: printf("RRC"); break;

        case 0x10: printf("NOP"); break;
               // D <- byte 3, E <- byte 2
        case 0x11: printf("LXI    DE, D <- 0x%02x E <- 0x%02x", code[2], code[1]); opbytes = 3; break;
               // (DE) <- A
        case 0x12: printf("STAX   D"); break;
               // DE <- DE + 1
        case 0x13: printf("INX    D"); break;
               // D <- D+1
        case 0x14: printf("INR    D"); break;
               // D <- D-1
        case 0x15: printf("DCR    D"); break;
               // D <- byte 2
        case 0x16: printf("MVI    D <- 0x%02x", code[1]); opbytes = 2; break;
               // A = A << 1; bit 0 = prev CY; CY = prev bit 7
        case 0x17: printf("RAL"); break;

        case 0x18: printf("NOP"); break;
               // HL = HL + DE
        case 0x19: printf("DAD    D"); break;
               // A <- (DE)
        case 0x1a: printf("LDAX   D"); break;
               // DE = DE-1
        case 0x1b: printf("DCX    D"); break;
               // E <-E+1
        case 0x1c: printf("INR    E"); break;
               // E <- E-1
        case 0x1d: printf("DCR    E"); break;
               // 	E <- byte 2
        case 0x1e: printf("MVI    E <- 0x%02x", code[1]); opbytes = 2; break;
               // A = A >> 1; bit 7 = prev bit 7; CY = prev bit 0
        case 0x1f: printf("RAR"); break;

        case 0x20: printf("NOP"); break;
               // 	H <- byte 3, L <- byte 2
        case 0x21: printf("LXI    HL, H <- 0x%02x L <- 0x%02x", code[2], code[1]); opbytes = 3; break;
               // Stores the data in the HL register pair into the specified address
               // (adr) <-L; (adr+1)<-H
        case 0x22: printf("SHLD   $0x%02x%02x", code[2], code[1]); opbytes = 3; break;    // TODO: Check Endian Byte Order
               // HL <- HL + 1
        case 0x23: printf("INX    HL"); break;
               // H <- H + 1
        case 0x24: printf("INR    H"); break;
               // H <- H - 1
        case 0x25: printf("DCR    H"); break;
               // H <- Byte 2
        case 0x26: printf("MVI    H <- 0x%02x", code[1]); opbytes = 2; break;
               // "special" ???
               // https://www.tutorialspoint.com/daa-instruction-in-8085-microprocessor
        case 0x27: printf("DAA"); break;

        case 0x28: printf("NOP"); break;
               // HL = HL + HL
        case 0x29: printf("DAD    HL"); break;
               // L <- (adr); H<-(adr+1)
        case 0x2a: printf("LHLD   $0x%02x%02x", code[2], code[1]); opbytes = 3; break;
               // HL = HL - 1
        case 0x2b: printf("DCX    H"); break;
               // L <- L + 1
        case 0x2c: printf("INR    L"); break;
               // L <- L - 1
        case 0x2d: printf("DCR    L"); break;
               // L <- byte 2
        case 0x2e: printf("MVI    L <- 0x%02x", code[1]); opbytes = 2; break;
               // A <- !A
               // Flips all the bits in the CMA
        case 0x2f: printf("CMA"); break;

        case 0x30: printf("NOP"); break;
               // Load the instruction arguments into the stack pointer
        case 0x31: printf("LXI    SP <- 0x%02x%02x", code[2], code[1]); opbytes = 3; break;
               // Store the contents of the accumulator at the address passed as an instruction argument
        case 0x32: printf("STA    $0x%02x%02x", code[2], code[1]); opbytes = 3; break;
               // Increment the Stack Pointer
        case 0x33: printf("INX    SP"); break;
               // Increment the HL register
        case 0x34: printf("INR    M"); break;
               // Decrement the HL register
        case 0x35: printf("DCR    M"); break;
               // 	(HL) <- byte 2
        case 0x36: printf("MVI    M <- 0x%02x", code[1]); opbytes = 2; break;
               // Sets the carry flag to 1 regardless of previous value
        case 0x37: printf("STC"); break;

        case 0x38: printf("NOP"); break;
               // HL = HL + SP
        case 0x39: printf("DAD    SP"); break;
               // Load the byte pointed to the by the address into the accumulator
        case 0x3a: printf("LDA    $0x%02x%02x", code[2], code[1]); opbytes = 3; break;
               // Decrement Stack Pointer
        case 0x3b: printf("DCX    SP"); break;
               // Increment the Accumulator
        case 0x3c: printf("INR    A"); break;
               // Decrement the Accumulator
        case 0x3d: printf("DCR    A"); break;
        case 0x3e: printf("MVI    A <- 0x%02x", code[1]); opbytes = 2; break;
               // Flip the carry flag bit
        case 0x3f: printf("CMC"); break;

           /* ---------------- Move bytes into the B register ----------- */
        
        case 0x40: printf("MOV    B <- B"); break;
        case 0x41: printf("MOV    B <- C"); break;
        case 0x42: printf("MOV    B <- D"); break;
        case 0x43: printf("MOV    B <- E"); break;
        case 0x44: printf("MOV    B <- H"); break;
        case 0x45: printf("MOV    B <- L"); break;
        case 0x46: printf("MOV    B <- (HL)"); break;
        case 0x47: printf("MOV    B <- A"); break;

           /* ---------------- Move bytes into the C register ----------- */

        case 0x48: printf("MOV    C <- B"); break;
        case 0x49: printf("MOV    C <- C"); break;
        case 0x4a: printf("MOV    C <- D"); break;
        case 0x4b: printf("MOV    C <- E"); break;
        case 0x4c: printf("MOV    C <- H"); break;
        case 0x4d: printf("MOV    C <- L"); break;
        case 0x4e: printf("MOV    C <- (HL)"); break;
        case 0x4f: printf("MOV    C <- A"); break;

           /* ---------------- Move bytes into the D register ----------- */

        case 0x50: printf("MOV    D <- B"); break;
        case 0x51: printf("MOV    D <- C"); break;
        case 0x52: printf("MOV    D <- D"); break;
        case 0x53: printf("MOV    D <- E"); break;
        case 0x54: printf("MOV    D <- H"); break;
        case 0x55: printf("MOV    D <- L"); break;
        case 0x56: printf("MOV    D <- (HL)"); break;
        case 0x57: printf("MOV    D <- A"); break;

           /* ---------------- Move bytes into the E register ----------- */

        case 0x58: printf("MOV    E <- B"); break;
        case 0x59: printf("MOV    E <- C"); break;
        case 0x5a: printf("MOV    E <- D"); break;
        case 0x5b: printf("MOV    E <- E"); break;
        case 0x5c: printf("MOV    E <- H"); break;
        case 0x5d: printf("MOV    E <- L"); break;
        case 0x5e: printf("MOV    E <- (HL)"); break;
        case 0x5f: printf("MOV    E <- A"); break;

           /* ---------------- Move bytes into the H register ----------- */

        case 0x60: printf("MOV    H <- B"); break;
        case 0x61: printf("MOV    H <- C"); break;
        case 0x62: printf("MOV    H <- D"); break;
        case 0x63: printf("MOV    H <- E"); break;
        case 0x64: printf("MOV    H <- H"); break;
        case 0x65: printf("MOV    H <- L"); break;
        case 0x66: printf("MOV    H <- (HL)"); break;
        case 0x67: printf("MOV    H <- A"); break;

           /* ---------------- Move bytes into the L register ----------- */

        case 0x68: printf("MOV    L <- B"); break;
        case 0x69: printf("MOV    L <- C"); break;
        case 0x6a: printf("MOV    L <- D"); break;
        case 0x6b: printf("MOV    L <- E"); break;
        case 0x6c: printf("MOV    L <- H"); break;
        case 0x6d: printf("MOV    L <- L"); break;
        case 0x6e: printf("MOV    L <- (HL)"); break;
        case 0x6f: printf("MOV    L <- A"); break;

           /* ---------------- Move the contents of the registers into the HL address ----------- */

        case 0x70: printf("MOV    (HL) <- B"); break;
        case 0x71: printf("MOV    (HL) <- C"); break;
        case 0x72: printf("MOV    (HL) <- D"); break;
        case 0x73: printf("MOV    (HL) <- E"); break;
        case 0x74: printf("MOV    (HL) <- H"); break;
        case 0x75: printf("MOV    (HL) <- L"); break;
               // Special instruction
        case 0x76: printf("HLT"); break;
        case 0x77: printf("MOV    (HL) <- A"); break;

           /* ---------------- Move bytes into the A register ----------- */

               // Move the contents of the B register into the A register
        case 0x78: printf("MOV    A <- B"); break;
        case 0x79: printf("MOV    A <- C"); break;
        case 0x7a: printf("MOV    A <- D"); break;
        case 0x7b: printf("MOV    A <- E"); break;
        case 0x7c: printf("MOV    A <- H"); break;
        case 0x7d: printf("MOV    A <- L"); break;
        case 0x7e: printf("MOV    A <- (HL)"); break;
        case 0x7f: printf("MOV    A <- A"); break;

           /* ---------------- Addition instructions ----------- */

        case 0x80: printf("ADD    B    /* A <- A + B */"); break;
        case 0x81: printf("ADD    C    /* A <- A + C */"); break;
        case 0x82: printf("ADD    D    /* A <- A + D */"); break;
        case 0x83: printf("ADD    E    /* A <- A + E */"); break;
        case 0x84: printf("ADD    H    /* A <- A + H */"); break;
        case 0x85: printf("ADD    L    /* A <- A + L */"); break;
        case 0x86: printf("ADD    M    /* A <- A + (HL) */"); break;
        case 0x87: printf("ADD    A    /* A <- A + A */"); break;
        case 0x88: printf("ADC    B    /* A <- A + B + CY */"); break;
        case 0x89: printf("ADC    C    /* A <- A + C + CY */"); break;
        case 0x8a: printf("ADC    D    /* A <- A + D + CY */"); break;
        case 0x8b: printf("ADC    E    /* A <- A + E + CY */"); break;
        case 0x8c: printf("ADC    H    /* A <- A + H + CY */"); break;
        case 0x8d: printf("ADC    L    /* A <- A + L + CY */"); break;
        case 0x8e: printf("ADC    (HL)    /* A <- A + (HL) + CY */"); break;
        case 0x8f: printf("ADC    A    /* A <- A + A + CY */"); break;

           /* ---------------- Subtraction instructions ----------- */

        case 0x90: printf("SUB    B    /* A <- A - B */"); break;
        case 0x91: printf("SUB    C    /* A <- A - C */"); break;
        case 0x92: printf("SUB    D    /* A <- A + D */"); break;
        case 0x93: printf("SUB    E    /* A <- A - E */"); break;
        case 0x94: printf("SUB    H    /* A <- A + H */"); break;
        case 0x95: printf("SUB    L    /* A <- A - L */"); break;
        case 0x96: printf("SUB    (HL)    /* A <- A + (HL) */"); break;
        case 0x97: printf("SUB    A    /* A <- A - A */"); break;
        case 0x98: printf("SBB    B    /* A <- A - B - CY */"); break;
        case 0x99: printf("SBB    C    /* A <- A - C - CY */"); break;
        case 0x9a: printf("SBB    D    /* A <- A - D - CY */"); break;
        case 0x9b: printf("SBB    E    /* A <- A - E - CY */"); break;
        case 0x9c: printf("SBB    H    /* A <- A - H - CY */"); break;
        case 0x9d: printf("SBB    L    /* A <- A - L - CY */"); break;
        case 0x9e: printf("SBB    (HL)    /* A <- A - (HL) - CY */"); break;
        case 0x9f: printf("SBB    A    /* A <- A - A - CY */"); break;

           /* ---------------- Bitwise AND instructions ----------- */

        case 0xa0: printf("ANA    B    /* A <- A & B */"); break;
        case 0xa1: printf("ANA    C    /* A <- A & C */"); break;
        case 0xa2: printf("ANA    D    /* A <- A & D */"); break;
        case 0xa3: printf("ANA    E    /* A <- A & E */"); break;
        case 0xa4: printf("ANA    H    /* A <- A & H */"); break;
        case 0xa5: printf("ANA    L    /* A <- A & L */"); break;
        case 0xa6: printf("ANA    (HL) /* A <- A & (HL) */"); break;
        case 0xa7: printf("ANA    A    /* A <- A & A */"); break;

           /* ---------------- Bitwise XOR instructions ----------- */

        case 0xa8: printf("XRA    B    /* A <- A ^ B */"); break;
        case 0xa9: printf("XRA    C    /* A <- A ^ C */"); break;
        case 0xaa: printf("XRA    D    /* A <- A ^ D */"); break;
        case 0xab: printf("XRA    E    /* A <- A ^ E */"); break;
        case 0xac: printf("XRA    H    /* A <- A ^ H */"); break;
        case 0xad: printf("XRA    L    /* A <- A ^ L */"); break;
        case 0xae: printf("XRA    (HL) /* A <- A ^ (HL) */"); break;
        case 0xaf: printf("XRA    A    /* A <- A ^ A */"); break;

            /* ---------------- Bitwise OR instructions ----------- */

        case 0xb0: printf("ORA    B    /* A <- A | B */"); break;
        case 0xb1: printf("ORA    C    /* A <- A | C */"); break;
        case 0xb2: printf("ORA    D    /* A <- A | D */"); break;
        case 0xb3: printf("ORA    E    /* A <- A | E */"); break;
        case 0xb4: printf("ORA    H    /* A <- A | H */"); break;
        case 0xb5: printf("ORA    L    /* A <- A | L */"); break;
        case 0xb6: printf("ORA    (HL) /* A <- A | (HL) */"); break;
        case 0xb7: printf("ORA    A    /* A <- A | A */"); break;

           /* ---------------- CMP instructions ----------- */

        case 0xb8: printf("CMP    B"); break;
        case 0xb9: printf("CMP    C"); break;
        case 0xba: printf("CMP    D"); break;
        case 0xbb: printf("CMP    E"); break;
        case 0xbc: printf("CMP    H"); break;
        case 0xbd: printf("CMP    L"); break;
        case 0xbe: printf("CMP    (HL)"); break;
        case 0xbf: printf("CMP    A"); break;

           /* ---------------- Control Flow instructions ----------- */

               // if NZ, RET
        case 0xc0: printf("RNZ"); break;
               // C <- (sp); B <- (sp+1); sp <- sp+2
        case 0xc1: printf("POP    B"); break;
               // if NZ, PC <- adr
        case 0xc2: printf("JNZ    $0x%02x%02x", code[2], code[1]); opbytes = 3; break;
               // PC <= adr
        case 0xc3: printf("JMP    $0x%02x%02x", code[2], code[1]); opbytes = 3; break;
               // if NZ, CALL adr
        case 0xc4: printf("CNZ    $0x%02x%02x", code[2], code[1]); opbytes = 3; break;
               // (sp-2)<-C; (sp-1)<-B; sp <- sp - 2
        case 0xc5: printf("PUSH   B"); break;
               // 	A <- A + byte
        case 0xc6: printf("ADI    0x%02x", code[1]); opbytes = 2; break;
               // CALL $0
        case 0xc7: printf("RST    0"); break;
               // if Z, RET
        case 0xc8: printf("RZ"); break;
               // PC.lo <- (sp); PC.hi<-(sp+1); SP <- SP+2
        case 0xc9: printf("RET"); break;
               // if Z, PC <- adr
        case 0xca: printf("JZ     $0x%02x%02x", code[2], code[1]); opbytes = 3; break;

        case 0xcb: printf("NOP"); break;
               // 	if Z, CALL adr
        case 0xcc: printf("CZ     $0x%02x%02x", code[2], code[1]); opbytes = 3; break;
               // (SP-1)<-PC.hi;(SP-2)<-PC.lo;SP<-SP-2;PC=adr
        case 0xcd: printf("CALL   $0x%02x%02x", code[2], code[1]); opbytes = 3; break;
               // A <- A + data + CY
        case 0xce: printf("ACI    0x%02x", code[1]); opbytes = 2; break;
               // 	CALL $8
        case 0xcf: printf("RST    1"); break;
               // if NCY, RET
        case 0xd0: printf("RNC"); break;
               // 	E <- (sp); D <- (sp+1); sp <- sp+2
        case 0xd1: printf("POP    D"); break;
               // if NCY, PC<-adr
        case 0xd2: printf("JNC    $0x%02x%02x", code[2], code[1]); opbytes = 3; break;
               // special
        case 0xd3: printf("OUT    0x%02x", code[1]); opbytes = 2; break;
               // if NCY, CALL adr
        case 0xd4: printf("CNC    $0x%02x%02x", code[2], code[1]); opbytes = 3; break;
               // (sp-2)<-E; (sp-1)<-D; sp <- sp - 2
        case 0xd5: printf("PUSH   D"); break;
               // A <- A - data
        case 0xd6: printf("SUI    0x%02x", code[1]); opbytes = 2; break;
               // CALL $10
        case 0xd7: printf("RST    2"); break;
               // if CY, RET
        case 0xd8: printf("RC"); break;
        case 0xd9: printf("NOP"); break;
               // if CY, PC<-adr
        case 0xda: printf("JC     $0x%02x%02x", code[2], code[1]); opbytes = 3; break;
               // special
        case 0xdb: printf("IN     0x%02x", code[1]); opbytes = 2; break;
               // if CY, CALL adr
        case 0xdc: printf("CC     $0x%02x%02x", code[2], code[1]); opbytes = 3; break;

        case 0xdd: printf("NOP"); break;
               // A <- A - data - CY
        case 0xde: printf("SBI    0x%02x", code[1]); opbytes = 2; break;
               // CALL $18
        case 0xdf: printf("RST    3"); break;
               // if PO, RET
        case 0xe0: printf("RPO"); break;
               // L <- (sp); H <- (sp+1); sp <- sp+2
        case 0xe1: printf("POP    H"); break;
               // if PO, PC <- adr
        case 0xe2: printf("JPO    $0x%02x%02x", code[2], code[1]); opbytes = 3; break;
               // L <-> (SP); H <-> (SP+1)
        case 0xe3: printf("XTHL"); break;
               // if PO, CALL adr
        case 0xe4: printf("CPO    $0x%02x%02x", code[2], code[1]); opbytes = 3; break;
               // (sp-2)<-L; (sp-1)<-H; sp <- sp - 2
        case 0xe5: printf("PUSH   HL"); break;
               // A <- A & data
        case 0xe6: printf("ANI    0x%02x", code[1]); opbytes = 2; break;
               // CALL $20
        case 0xe7: printf("RST    4"); break;
               // if PE, RET
        case 0xe8: printf("RPE"); break;
               // PC.hi <- H; PC.lo <- L
        case 0xe9: printf("PCHL"); break;
               // if PE, PC <- adr
        case 0xea: printf("JPE    $0x%02x%02x", code[2], code[1]); opbytes = 3; break;
               // H <-> D; L <-> E
        case 0xeb: printf("XCHG"); break;
               // if PE, CALL adr
        case 0xec: printf("CPE    $0x%02x%02x", code[2], code[1]); opbytes = 3; break;

        case 0xed: printf("NOP"); break;
               // A <- A ^ data
        case 0xee: printf("XRI    0x%02x", code[1]); opbytes = 2; break;
               // CALL $28
        case 0xef: printf("RST    5"); break;
               // if P, RET
        case 0xf0: printf("RP"); break;
               // flags <- (sp); A <- (sp+1); sp <- sp+2
        case 0xf1: printf("POP   PSW"); break;
               // 	if P=1 PC <- adr
        case 0xf2: printf("JP     $0x%02x%02x", code[2], code[1]); opbytes = 3; break;
               // special
        case 0xf3: printf("DI"); break;
               // if P, PC <- adr
        case 0xf4: printf("CP     $0x%02x%02x", code[2], code[1]); opbytes = 3; break;
               // (sp-2)<-flags; (sp-1)<-A; sp <- sp - 2
        case 0xf5: printf("PUSH   PSW"); break;
               // A <- A | data
        case 0xf6: printf("ORI    0x%02x", code[1]); opbytes = 2; break;
               // CALL $30
        case 0xf7: printf("RST 6"); break;
               // if M, RET
        case 0xf8: printf("RM"); break;
               // SP=HL
        case 0xf9: printf("SPHL"); break;
               // if M, PC <- adr
        case 0xfa: printf("JM     $0x%02x%02x", code[2], code[1]); opbytes = 3; break;
               // special
        case 0xfb: printf("EI"); break;
               // if M, CALL adr
        case 0xfc: printf("CM     $0x%02x%02x", code[2], code[1]); opbytes = 3; break;

        case 0xfd: printf("NOP"); break;
               // A - data
        case 0xfe: printf("CPI    0x%02x", code[1]); opbytes = 2; break;
               // CALL $38
        case 0xff: printf("RST   7"); break;
    }

    // print a new line character
    puts("");

    return opbytes;
}