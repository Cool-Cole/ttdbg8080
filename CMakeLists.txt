cmake_minimum_required(VERSION 3.22)
project(ttdbg8080 C)

set(CMAKE_C_STANDARD 23)

option(BUILD_ASAN "Build with -fsanitize=address" OFF)
option(BUILD_DEBUG "Build with debug symbols" OFF)
option(BUILD_FAST "Build with -O3" OFF)

if(BUILD_ASAN)
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
endif(BUILD_ASAN)

if(BUILD_DEBUG)
    add_compile_options(-g)
endif(BUILD_DEBUG)

if(BUILD_FAST)
    add_compile_options(-O3)
endif(BUILD_FAST)

add_compile_definitions(PATH="${CMAKE_CURRENT_LIST_DIR}")

include_directories(./src/8080emu)

add_executable(ttdbg8080
        src/main.c

        src/8080emu/handleState.h
        src/8080emu/handleState.c
        src/8080emu/opcodes.h
        src/8080emu/opcodes.c
        src/8080emu/breakpoints.h
        src/8080emu/breakpoints.c

        src/8080disassemble/disassemble8080.h
        src/8080disassemble/disassemble8080.c

        src/lib/intDefs.h
        )
