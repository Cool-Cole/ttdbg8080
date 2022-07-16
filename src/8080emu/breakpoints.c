//
// Created by Cole on 7/15/2022.
//

#include "breakpoints.h"

#include "stdlib.h"

int isBreakpointHit(cpuState *state){
    for(int i = 0; i < state->numBreakPoints; i++){
        if(state->PC == state->breakpoints[i]){
            return 1;
        }
    }

    return 0;
}

void addBreakPoint(cpuState *state, u16 address){

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

        free(state->breakpoints);

        state->breakpoints = temp;

        state->breakpoints[state->numBreakPoints] = address;

        state->numBreakPoints++;
    }

}