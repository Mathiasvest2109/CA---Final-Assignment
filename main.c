#include <stdio.h>
#include "instructions.h"

int main(int argc, char** args)
{
   // printf("Hello world");

   // addi x3, x0, 69
   InstructionData instructionData = decode(0x4500193);

   printf("Type: %d\n", instructionData.type);
   InstructionI* instruction = instructionData.data;

   printf("Opcode: %d\nx%d <- x%d + %d\n", instruction->opcode, instruction->rd, instruction->rs1, instruction->immediate);

   return 0;
}