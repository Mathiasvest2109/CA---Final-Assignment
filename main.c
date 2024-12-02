#include <stdio.h>
#include "decoder.h"
#include "memory.h"
#include "program.h"
#include "executor.h"

int main(int argc, char** args)
{
   if (argc == 1) {
      printf("No program given\n");
      return 1;
   }
   else if (argc > 2) {
      printf("Too many programs given\n");
      return 2;
   }

   Program* p = load_program(args[1]);

   while (!p->isHalting) {
      InstructionData instructionData = decode(fetch_instruction(p));

      if (instructionData.type == Unknown) {
         // Unknown instruction, stopping execution
         break;
      }
      execute_instruction(&instructionData, p);
   }

   dump_registers();
   int statusCode = p->statusCode;
   unload_program(p);

   return statusCode;
}