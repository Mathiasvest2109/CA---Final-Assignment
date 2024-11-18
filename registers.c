#include "registers.h"

int registers[31];

int get_register(int index) {
    if (index == 0) return 0;

    return registers[index - 1];
}

void set_register(int index, int value) {
    if (index == 0) return;

    registers[index - 1] = value;
}