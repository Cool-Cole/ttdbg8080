//
// Created by Cole on 7/15/2022.
//

#include "breakpoints.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


bool isBreakpointHit(cpuState *state, u16 addr){
    for(int i = 0; i < state->numBreakPoints; i++){
        if(addr == state->breakpoints[i]){
            return true;
        }
    }

    return false;
}

void addBreakpoint(cpuState *state, u16 address){

    if(state->numBreakPoints == 0){
        state->breakpoints = malloc(sizeof(u16));

        if(state->breakpoints == NULL){
            // If the allocation fails, exit the program.
            exit(EXIT_FAILURE);
        }

        state->breakpoints[0] = address;

        state->numBreakPoints = 1;

    } else {
        u16 *temp = realloc(state->breakpoints, state->numBreakPoints + 1);

        if(temp == NULL){
            // If the allocation fails, exit the program.
            exit(EXIT_FAILURE);
        }

        //free(state->breakpoints);

        state->breakpoints = temp;

        state->breakpoints[state->numBreakPoints] = address;

        state->numBreakPoints++;
    }

}

void deleteBreakpoint(cpuState *state, u16 address){
    int deletedBreakpointCount = 0;

    if(state->numBreakPoints != 0){

        if(state->numBreakPoints == 1){
            free(state->breakpoints);
            state->numBreakPoints = 0;
            state->breakpoints = NULL;

        } else {

            for(int i = 0; i < state->numBreakPoints; i++){

                if(state->breakpoints[i] == address){

                    deletedBreakpointCount += 1;

                    for(int j = i; j < state->numBreakPoints; j++){
                        state->breakpoints[j] = state->breakpoints[j + 1];
                    }

                }

            }

            state->numBreakPoints -= deletedBreakpointCount;

        }
    }
}

void printBreakpoints(cpuState *state){

    printf("Breakpoints:\n");

    if(state->breakpoints == 0){
        printf("No breakpoints are currently set.\n");
    } else {
        for(int i = 0; i < state->numBreakPoints; i++){
            printf("0x%04x\n", state->breakpoints[i]);
        }
    }

    printf("\n");
}