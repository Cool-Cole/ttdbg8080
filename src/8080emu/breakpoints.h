//
// Created by Cole on 7/15/2022.
//

#include <stdbool.h>

#include "handleState.h"

#ifndef TTDBG8080_BREAKPOINT_H
#define TTDBG8080_BREAKPOINT_H

bool isBreakpointHit(cpuState *state, u16 addr);

void addBreakpoint(cpuState *state, u16 address);
void deleteBreakpoint(cpuState *state, u16 address);

void printBreakpoints(cpuState *state);

#endif //TTDBG8080_BREAKPOINT_H