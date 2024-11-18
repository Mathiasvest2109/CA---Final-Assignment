#include "program.h"
#include <stdio.h>

Program load_program(char* path) {
    FILE* program = fopen(path, "rb");
    return program;
}

int next_instruction(Program program) {
    int instruction;
    fread(&instruction, 4, 1, program);
    return instruction;
}

void unload_program(Program program) {
    fclose(program);
}