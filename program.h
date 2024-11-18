#ifndef PROGRAM_H
#define PROGRAM_H

// opaque type
typedef void* Program;

Program load_program(char* path);
int next_instruction(Program program);
void unload_program(Program program);

#endif