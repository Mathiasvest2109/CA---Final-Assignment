#include "program.h"
#include <stdlib.h>
#include <stdio.h>

Program* load_program(char* path) {
    Program* p = malloc(sizeof(Program));
    p->stream = fopen(path, "rb");
    p->pc = 0;
    return p;
}

int next_instruction(Program* program) {
    int instruction;

    // Set the stream to the same position as the program counter (as it could've been updated)
    fseek(program->stream, program->pc, SEEK_SET);

    // Actually read the instruction
    fread(&instruction, 4, 1, program->stream);

    // Update the program counter accordingly
    program->pc += 4;
    return instruction;
}

void unload_program(Program* program) {
    fclose(program->stream);
    free(program);
}