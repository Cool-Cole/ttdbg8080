#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "8080emu/handleState.h"
#include "8080emu/breakpoints.h"

#include "8080disassemble/disassemble8080.h"

void handlePrompt(char *command, int maxLen);
void handleWrite(cpuState *state);
void printHelp(void);
void printDbgHelp(void);
void printState(cpuState *state);

int main(int argc, char *argv[]) {

    cpuState state = initState();

    int maxLen = 128;
    char debuggerCommands[maxLen];
    debuggerCommands[0] = '\0';

    // This is needed for the strtol
    char *temp;

    char opt;

    setbuf(stdout, 0);

    // This loop parses the user provided command line arguments
    while((opt = getopt(argc, argv, ":l:h")) != -1)
    {
        switch(opt)
        {
            case 'l':
                printf("Loading ROM file %s\n", optarg);
                loadROM(&state, optarg, 0x0100);
                printf("ROM File loaded!\n\n");
                break;
            case 'h':
                printHelp();
                break;
            case ':':
                printf("option needs a value\n");
                break;
            case '?':
                printf("unknown option: %c\n", optopt);
                break;
        }
    }

    printf("In order to access the list of debugger commands, run the command \"h\".\n");

    // TESTING CODE FROM http://www.emulator101.com/full-8080-emulation.html
    // Fix the first instruction to be JMP 0x100
    state.memory[0] = 0xc3;
    state.memory[1] = 0;
    state.memory[2] = 0x01;

    // Fix the stack pointer from 0x6ad to 0x7ad
    // this 0x06 byte 112 in the code, which is
    // byte 112 + 0x100 = 368 in memory
    state.memory[368] = 0x7;

    //Skip DAA test
    state.memory[0x59c] = 0xc3;
    state.memory[0x59d] = 0xc2;
    state.memory[0x59e] = 0x05;

    // The main debugger loop
    do{

        handlePrompt(debuggerCommands, maxLen);

        // Warning: Parsing commands in this manner assumes that the user is inputting them correctly
        // Proper error handling should be implemented for serious situation.
        switch (debuggerCommands[0]) {
            case 'z':

                if(strlen(debuggerCommands) < 3){
                    printf("To few arguments for command z\n");
                    break;
                }

                if(emulateReplay(&state, atoi(debuggerCommands + 1)) == 1){
                    printf("Breakpoint hit at 0x%04x!\\n", state.PC);
                }
                else {
                    printf("Successfully stepped back %d instruction(s)\n\n", atoi(debuggerCommands + 1));
                }
                break;
            case 's':

                if(strlen(debuggerCommands) < 3){
                    printf("To few arguments for command s\n");
                    break;
                }

                if(emulateSteps(&state, atoi(debuggerCommands + 1)) == 1){
                    printf("Breakpoint hit at 0x%04x!\\n", state.PC);
                }
                else {
                    printf("Successfully stepped forward %d instruction(s)\n\n", atoi(debuggerCommands + 1));
                }
                break;
            case 'p':
                printState(&state);
                printBreakpoints(&state);
                break;
            case 'd':

                if(strlen(debuggerCommands) < 3){
                    printf("To few arguments for command d\n");
                    break;
                }

                disassembleMulti(&state, state.PC, atoi(debuggerCommands + 1));
                break;
            case 'b':

                if(strlen(debuggerCommands) < 3){
                    printf("To few arguments for command b\n");
                    break;
                }

                if(strtol(debuggerCommands + 2, &temp, 16) > UINT16_MAX){
                    printf("%The number you entered is to high!\n"
                            "Please enter a 16 bit address number.\n");
                } else {
                    // Convert the address the user provided to an unsigned short using strol.
                    addBreakpoint(&state, (u16)strtol(debuggerCommands + 2, &temp, 16));
                }

                break;
            case 'n':

                if(strlen(debuggerCommands) < 3){
                    printf("To few arguments for command n\n");
                    break;
                }

                if(strtol(debuggerCommands + 2, &temp, 16) > UINT16_MAX){
                    printf("%The number you entered is to high!\n"
                           "Please enter a 16 bit address number.\n");
                } else {
                    // Convert the address the user provided to an unsigned short using strol.
                    deleteBreakpoint(&state, (u16)strtol(debuggerCommands + 2, &temp, 16));
                }

                break;
            case 'h':
                printDbgHelp();
                break;
            case 'x':

                if(strlen(debuggerCommands) < 3){
                    printf("To few arguments for command x\n");
                    break;
                }

                if(dumpState(&state, debuggerCommands + 2) == 0){
                    printf("CPU memory successfully dumped to %s\n", debuggerCommands + 2);
                } else {
                    printf("Could not dump the CPU memory to %s\n", debuggerCommands + 2);
                }

                break;
            case 'w':
                handleWrite(&state);
                break;
            case 'q':
                exit(EXIT_SUCCESS);
                break;
            default:
                printf("%c is an unrecognized option! Use h to print the debugger help page.\n\n", debuggerCommands[0]);
        }

        // Don't loop around when done executing the rom
    } while(0x0700 != state.PC);

    return EXIT_SUCCESS;
}

void handlePrompt(char *command, const int maxLen){

    printf("ttdbg: ");
    fgets(command, maxLen, stdin);

    // print a new line character
    puts("");

    int i = 2;

    // replace the new line character with a null terminator
    // add a null terminator to the end of the sequence if no new line character is found
    do {

        i++;

        if(command[i] == '\n'){
            command[i] = '\0';
        }

        if(i == maxLen){
            command[i - 1] = '\0';
            break;
        }

    } while(command[i] != '\0');

}

void handleWrite(cpuState *state){
    short maxLen = 128;
    char command[maxLen];

    bool validInput = false;

    enum registers{SP, PC, HL, DE, BC, H, L, D, E, B, C, A, addr};

    enum registers selectedRegister;

    u16 address;

    char *temp;

    printf("Please enter an address in the form 0xffff or a register.\n"\
    "The available registers are: SP, PC, HL, DE, BC, H, L, D, E, B, C, A\n");

    do {
        printf("Address or register: ");
        fgets(command, maxLen, stdin);
        puts("");

        if(strncmp(command, "0x", 2) == 0){
            address = (u16)strtol(command, &temp, 16);
            selectedRegister = addr;
            validInput = true;
        } else if(strncmp(command, "SP", 2) == 0) {
            selectedRegister = SP;
            validInput = true;
        } else if(strncmp(command, "PC", 2) == 0) {
            selectedRegister = PC;
            validInput = true;
        } else if(strncmp(command, "HL", 2) == 0) {
            selectedRegister = HL;
            validInput = true;
        } else if(strncmp(command, "DE", 2) == 0) {
            selectedRegister = DE;
            validInput = true;
        } else if(strncmp(command, "BC", 2) == 0) {
            selectedRegister = BC;
            validInput = true;
        } else if(strncmp(command, "H", 1) == 0) {
            selectedRegister = H;
            validInput = true;
        } else if(strncmp(command, "L", 1) == 0) {
            selectedRegister = L;
            validInput = true;
        } else if(strncmp(command, "D", 1) == 0) {
            selectedRegister = D;
            validInput = true;
        } else if(strncmp(command, "E", 1) == 0) {
            selectedRegister = E;
            validInput = true;
        } else if(strncmp(command, "B", 1) == 0) {
            selectedRegister = B;
            validInput = true;
        } else if(strncmp(command, "C", 1) == 0) {
            selectedRegister = C;
            validInput = true;
        } else if(strncmp(command, "A", 1) == 0) {
            selectedRegister = A;
            validInput = true;
        }

        if(strlen(command) > 7){
            validInput = false;
        }

        if(validInput == false){
            printf("Could not parse your input. Please try again.\n");
        }

    } while(validInput == false);

    validInput = false;

    u16 shortVal;
    u8 byteVal;

    do {

        // Enums start and 0 and count up, so if it is less than BC then it is a 2 byte register
        // else, the value can only accept 1 byte
        if(selectedRegister <= BC){
            printf("Please enter a 2 byte value in the form 0xffff: ");

            fgets(command, maxLen, stdin);

            if(strlen(command) == 7){
                validInput = true;
                shortVal = (u16)strtol(command, &temp, 16);
            }
        } else {
            printf("Please enter a 1 byte value in the form 0xff: ");

            fgets(command, maxLen, stdin);

            if(strlen(command) == 5){
                validInput = true;
                byteVal = (u8)strtol(command, &temp, 16);
            }
        }

        // Print a new line character
        puts("");

        if(validInput == false){
            printf("Could not parse your input. Please try again.\n");
        }

    } while(validInput == false);

    switch(selectedRegister){
        case SP:
            state->SP = shortVal;
            break;
        case PC:
            state->SP = shortVal;
            break;
        case HL:
            state->HL = shortVal;
            break;
        case DE:
            state->DE = shortVal;
            break;
        case BC:
            state->BC = shortVal;
            break;
        case H:
            state->H = byteVal;
            break;
        case L:
            state->L = byteVal;
            break;
        case D:
            state->D = byteVal;
            break;
        case E:
            state->E = byteVal;
            break;
        case B:
            state->B = byteVal;
            break;
        case C:
            state->C = byteVal;
            break;
        case A:
            state->A = byteVal;
            break;
        case addr:
            state->memory[address] = byteVal;
            break;
    }


}

void printHelp(void){
    printf("The 8080 time travel debugger.\n"
           "Usage:\n"
           "-h print this message\n"
           "-l <name of rom file to load>\n"
           "\n");

    printDbgHelp();

    exit(EXIT_SUCCESS);
}

void printDbgHelp(void){
    printf("Using the debugger:\n"
    "p         print the CPU flags and registers\n"
    "s N       step froward N instructions\n"
    "z N       step back N instructions\n"
    "b 0xffff  set a breakpoint at an address\n"
    "n 0xffff  remove the breakpoint at that address\n"
    "w         write to a register or address, a prompt will guide you\n"
    "c         continue program execution until a breakpoint is hit\n"
    "d N       disassemble the next N instructions at the offset of the instruction pointer\n"
    "x file    dump the current emulator memory to a file\n"
    "h         print the debugger help page\n"
    "q         Quit the debugger\n\n"
    );
}

void printState(cpuState *state){
    printf("CPU Registers - \n");
    printf("SP: 0x%04x\n", state->SP);
    printf("PC: 0x%04x\n", state->PC);
    printf("HL: 0x%04x\n", state->HL);
    printf("H: 0x%02x\n", state->H);
    printf("L: 0x%02x\n", state->L);
    printf("DE: 0x%04x\n", state->DE);
    printf("D: 0x%02x\n", state->D);
    printf("E: 0x%02x\n", state->E);
    printf("BC: 0x%04x\n", state->BC);
    printf("B: 0x%02x\n", state->B);
    printf("C: 0x%02x\n", state->C);
    printf("A: 0x%02x\n\n", state->A);

    printf("CPU Flags - \n");
    printf("Flag Byte: 0x%02x\n", state->flags.flagByte);
    printf("Sign Bit: %d\n", state->flags.sign);
    printf("Zero Bit: %d\n", state->flags.zero);
    printf("Always Zero Bit: %d\n", state->flags._alwaysZero1);
    printf("Auxiliary Carry Bit: %d\n", state->flags.auxCarry);
    printf("Always Zero Bit: %d\n", state->flags._alwaysZero2);
    printf("Parity Bit: %d\n", state->flags.parity);
    printf("Always One Bit: %d\n", state->flags._alwaysOne);
    printf("Carry Bit: %d\n\n", state->flags.carry);
}