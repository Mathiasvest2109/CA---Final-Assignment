#include "program.h"
#include "memory.h"
#include <stdlib.h>
#include <stdio.h>

Program* load_program(char* path) {
    Program* program = malloc(sizeof(Program));

    // TODO error handling?
    FILE* file = fopen(path, "rb");

    // Read the program into memory
    int address = 0;
    while (1) {
        int n;
        int bytesRead = fread(&n, 4, 1, file);

        if (bytesRead == 0) {
            break;
        }
        store_word(address, n);
        address += 4;
    }

    program->pc = 0;
    program->size = ftell(file);
    program->isHalting = false;
    program->statusCode = 0;

    return program;
}

bool has_instruction(Program* program) {
    return program->pc < program->size;
}

int fetch_instruction(Program* program) {
    int instruction = load_word(program->pc);

    // Update the program counter accordingly
    program->pc += 4;
    return instruction;
}

void unload_program(Program* program) {
    free(program);
}