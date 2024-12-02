#include "registers.h"
#include <stdio.h>

int registers[31];

int get_register(int index) {
    if (index == 0) return 0;

    return registers[index - 1];
}

void set_register(int index, int value) {
    if (index == 0) return; // Do nothing

    registers[index - 1] = value;
}

void dump_registers(void) {
    for (int i = 0; i < 32; i++) {
        printf("0x%08x (x%d)\n", get_register(i), i);
    }
}