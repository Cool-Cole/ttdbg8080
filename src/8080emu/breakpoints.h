//
// Created by Cole on 7/15/2022.
//

#include "handleState.h"

#ifndef TTDBG8080_BREAKPOINT_H
#define TTDBG8080_BREAKPOINT_H

// 0 for false
// 1 for true
int isBreakpointHit(cpuState *state);

void addBreakPoint(cpuState *state, u16 address);
int deleteBreakPoint(cpuState *state, u16 address);

#endif //TTDBG8080_BREAKPOINT_H