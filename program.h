#ifndef PROGRAM_H
#define PROGRAM_H

// opaque type
typedef void* Program;

Program load_program(char* path);
int next_instruction(Program program);
void set_program_counter(Program program, int programCounter);
int get_program_counter(Program program);
void unload_program(Program program);

#endif