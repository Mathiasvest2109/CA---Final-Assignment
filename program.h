#ifndef PROGRAM_H
#define PROGRAM_H

#include <stdio.h>
#include <stdbool.h>

typedef struct Program {
    int pc;
    int size;
} Program;

Program* load_program(char* path);
bool has_instruction(Program* program);
int fetch_instruction(Program* program);
void unload_program(Program* program);

#endif