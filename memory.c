#include "memory.h"
#include <stdio.h>
#include <string.h>

// dubious macros
#define KiB * (1024)
#define MiB * (1024 KiB)
#define GiB * (1024 MiB)

// allocate the memory (1 "MB")
unsigned char memory[1 MiB];

int32_t load_word(int address) {
    uint32_t loaded;
    memcpy(&loaded, memory + address, sizeof(loaded));
    return loaded;
}

int16_t load_half(int address) {
    uint16_t loaded;
    memcpy(&loaded, memory + address, sizeof(loaded));
    return loaded;
}

int8_t load_byte(int address) {
    uint8_t loaded;
    memcpy(&loaded, memory + address, sizeof(loaded));
    return loaded;
}

void store_word(int address, int32_t value) {
    memcpy(memory + address, &value, sizeof(value));
}

void store_half(int address, int16_t value) {
    memcpy(memory + address, &value, sizeof(value));
}

void store_byte(int address, int8_t value) {
    memcpy(memory + address, &value, sizeof(value));
}

void dump_memory(void) {
    for (int i = 0; i < 30; i++) {
        printf("%02X", memory[i]);
    }
    printf("\n");
}