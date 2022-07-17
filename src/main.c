#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "8080emu/handleState.h"
#include "8080emu/breakpoints.h"

#include "8080disassemble/disassemble8080.h"

void handlePrompt(char *command, int maxLen);
void printHelp(void);
void printDbgHelp(void);
void printState(cpuState *state);

int main(int argc, char *argv[]) {

    cpuState state = initState();

    int maxLen = 128;
    char debuggerCommands[maxLen];
    debuggerCommands[0] = '\0';

    // This is needed for the strtol
    char temp;

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
    //state.memory[0] = 0xc3;
    //state.memory[1] = 0;
    //state.memory[2] = 0x01;

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
            case 's':
                if(emulateSteps(&state, atoi(debuggerCommands + 1)) == 1){
                    printf("Breakpoint hit at 0x%04x!\\n", state.PC);
                }
                else {
                    printf("Successfully emulated %d instruction(s)\n\n", atoi(debuggerCommands + 1));
                }
                break;
            case 'p':
                printState(&state);
                printBreakPoints(&state);
                break;
            case 'd':
                disassembleMulti(state.memory, state.PC, atoi(debuggerCommands + 1));
                break;
            case 'b':

                if(strtol(debuggerCommands + 2, &temp, 16) > UINT16_MAX){
                    printf("%The number you entered is to high!\n"
                            "Please enter a lower address number.\n");
                } else {
                    addBreakpoint(&state, (u16)strtol(debuggerCommands + 2, &temp, 16));
                }

                break;
            case 'h':
                printDbgHelp();
                break;
            case 'x':

                if(dumpState(&state, debuggerCommands + 2) == 0){
                    printf("CPU memory successfully dumped to %s\n", debuggerCommands + 2);
                } else {
                    printf("Could not dump the CPU memory to %s\n", debuggerCommands + 2);
                }

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
    //"w 0xffff  set a watchpoint at an address\n"
    "c         continue program execution until a breakpoint or watchpoint is hit\n"
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