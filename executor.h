#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "instructions.h"
#include "registers.h"

// Function to execute a decoded instruction
void execute_instruction(InstructionData* instructionData);

#endif
