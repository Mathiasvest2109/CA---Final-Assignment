#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "instructions.h"
#include "registers.h"
#include "program.h"

// function to execute a decoded instruction
void execute_instruction(InstructionData* instructionData, Program* program);

#endif
