#include <stdio.h>
#include "decoder.h"
#include "program.h"

#define GET_OPCODE(instruction) (instruction & 0b111111)

int main(int argc, char** args)
{
   // printf("Hello world");
   Program p = load_program("addlarge.bin");

   print_lui(decode(next_instruction(p)).data);
   print_addi(decode(next_instruction(p)).data);
   print_lui(decode(next_instruction(p)).data);
   print_addi(decode(next_instruction(p)).data);
   print_add(decode(next_instruction(p)).data);
   print_addi(decode(next_instruction(p)).data);

   InstructionData i = decode(next_instruction(p));
   if (i.type == Unknown && ((InstructionUnknown*)i.data)->opcode == ECALL) {
      printf("ecall\n");
      printf("[SUCCESS]\n");
   }
   else {
      printf("not ecall\n");
      printf("[FAILURE]");
   }

   unload_program(p);
   printf("---------------------\n");

   p = load_program("addlarge.bin");
   printf("[pc = %d] ", get_program_counter(p));
   print_lui(decode(next_instruction(p)).data);
   printf("[pc = %d] ", get_program_counter(p));
   print_addi(decode(next_instruction(p)).data);
   printf("[pc = %d] ", get_program_counter(p));
   print_lui(decode(next_instruction(p)).data);
   
   printf("--- RESET ---\n");
   set_program_counter(p, 0);

   printf("[pc = %d] ", get_program_counter(p));
   print_lui(decode(next_instruction(p)).data);
   printf("[pc = %d] ", get_program_counter(p));
   print_addi(decode(next_instruction(p)).data);
   printf("[pc = %d] ", get_program_counter(p));
   print_lui(decode(next_instruction(p)).data);
   printf("[pc = %d] ", get_program_counter(p));
   print_addi(decode(next_instruction(p)).data);
   
   unload_program(p);
   return 0;
}

void print_lui(InstructionU* instruction) {
   printf("lui x%d, 0x%x\n", instruction->rd, instruction->immediate);
}

void print_addi(InstructionI* instruction) {
   printf("addi x%d, x%d, %d\n", instruction->rd, instruction->rs1, instruction->immediate);
}

void print_add(InstructionR* instruction) {
   printf("add x%d, x%d, x%d\n", instruction->rd, instruction->rs1, instruction->rs2);
}