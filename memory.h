#ifndef MEMORY_H
#define MEMORY_H

#include <inttypes.h>

int32_t load_word(int address);
int16_t load_half(int address);
int8_t load_byte(int address);

void store_word(int address, int32_t value);
void store_half(int address, int16_t value);
void store_byte(int address, int8_t value);

void dump_memory(void);

#endif