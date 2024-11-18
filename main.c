#include <stdio.h>
#include "decoder.h"
#include "memory.h"

int main(int argc, char** args)
{
   store_byte(0, 0xF0);
   store_half(1, 0xF0);
   store_word(3, 0xF0);

   printf("Word: %u\n", load_word(3));
   printf("Half: %u\n", load_half(1));
   printf("Byte: %u\n", (uint8_t)load_byte(0));

   printf("Mangled: %u (0x%08X)\n", load_word(0), load_word(0));
   dump_memory();

   return 0;
}