# Time Travel Debugger and Emulator for the Intel 8080

Or ttdbg for short.

![An image of the intel 8080A](https://upload.wikimedia.org/wikipedia/commons/3/3a/KL_Intel_i8080_Black_Background.jpg "An intel 8080A")
(Image courtesy of wikipedia)

## Features
- [X] Emulation of the 8080 architecture (missing Auxiliary carry CPU flag)
- [X] Supports break points
- [ ] Supports watch points
- [X] Builtin disassembler
- [X] Step forwards in program execution
- [X] Step backwards in program execution
- [X] Dump the emulator memory to a file
- [X] Facilitates user modification of memory and register values
- [ ] User modification of memory and registers persists across time travel replays

## How it works

### The Disassembler

The disassembler is a typical linear sweep disassembler, as such it doesn't follow the programs control flow. This may result in incorrectly disassembled bytes so make sure to keep an eye out for that if you are using ttdbg8080.

### The Emulator

The emulator works by combing through instructions one by one and executing their related function found in handlestate.c. These functions manipulate a global state as defined by the cpuState structure found in handlestate.h.

### Time Travel Debugging

The emulator/debugger facilitates the ability to step backwards in program execution. To accomplish this, the emulator simply reruns the program back to the desired point in execution. Breakpoints are kept and still checked for while the emulator reruns the program. At this point any modifications to the program by the user are not preserved when this action occurs.

## Disclaimer

This is just a toy project that I occasionally work on. Feedback on this project's code is highly encouraged and appreciated. You can find my contact information here https://cole.pizza/contact/.