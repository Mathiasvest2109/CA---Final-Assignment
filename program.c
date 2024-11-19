#include "program.h"
#include <stdlib.h>
#include <stdio.h>

Program* load_program(char* path) {
    Program* program = malloc(sizeof(Program));
    program->stream = fopen(path, "rb");
    // TODO: error handling?
    program->pc = 0;

    // Find the end of the file, and save where that is, so we know the size of the program
    fseek(program->stream, 0, SEEK_END);
    program->size = ftell(program->stream);
    fseek(program->stream, 0, SEEK_SET);
    return program;
}

bool has_instruction(Program* program) {
    return program->pc < program->size;
}

int fetch_instruction(Program* program) {
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