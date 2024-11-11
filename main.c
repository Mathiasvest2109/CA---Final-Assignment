#include <stdio.h>
#include "instructions.h"

int main(int argc, char** args)
{
   // printf("Hello world");

   // addi x3, x0, 69
   Instruction instr = decode(0x4500193);

   printf("Type: %d\n", instr.type);
   InstructionI* add = instr.data;

   printf("Opcode: %d\nx%d <- x%d + %d\n", add->opcode, add->rd, add->rs1, add->immediate);

   return 0;
}