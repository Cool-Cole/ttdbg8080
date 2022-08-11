//
// Created by Cole on 7/15/2022.
//

#include "handleState.h"

#ifndef TTDBG8080_BREAKPOINT_H
#define TTDBG8080_BREAKPOINT_H

// 0 for false
// 1 for true
int isBreakpointHit(cpuState *state);

//
void addBreakpoint(cpuState *state, u16 address);
void deleteBreakpoint(cpuState *state, u16 address);

void printBreakpoints(cpuState *state);

#endif //TTDBG8080_BREAKPOINT_H