# Time Travel Debugger for the Intel 8080

Or ttdbg for short.

This project is a debugger and emulator for the intel 8080.

![An image of the intel 8080A](https://upload.wikimedia.org/wikipedia/commons/3/3a/KL_Intel_i8080_Black_Background.jpg "An intel 8080A")
(Image courtesy of wikipedia)

## Features
- [X] Emulation of the 8080 architecture (missing Auxiliary carry CPU flag)
- [ ] Supports break points
- [ ] Supports watch points
- [X] Builtin disassembler
- [X] Step forwards in program execution
- [ ] Step backwards in program execution
- [X] Dump the emulator memory to a file
- [ ] Facilitates user modification of register values

## How it works



### The disassembler

The disassembler is a typical linear sweep disassembler, 

### The emulator

The emulator 

### Time travel debugging

The emulator/debugger facilitates the ability to step backwards in program execution the emulator simply reruns the program back to the desired point in execution. All user modifications to the emulator state are recorded.