#ifndef PROGRAM_H
#define PROGRAM_H

#include <stdio.h>

typedef struct Program {
    FILE* stream;
    int pc;
} Program;

Program* load_program(char* path);
int next_instruction(Program* program);
void unload_program(Program* program);

#endif