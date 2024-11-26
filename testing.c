#include "decoder.h"
#include "executor.h"
#include "memory.h"
#include "program.h"
#include "registers.h"
#include <string.h>

int compare_registers(int expected[], const char* testName);
void reset_registers(void);
void run_binary(char* path);
void task1(int* total, int* passed);
void task2(int* total, int* passed);
void task3(int* total, int* passed);
void task4(int* total, int* passed);
void custom(int* total, int* passed);

int compare_registers(int expected[], const char* testName) {
    int allGood = 1;

    for (int i = 0; i < 32; i++) {
        if (get_register(i) != expected[i]) {
            if (allGood == 1) {
                printf("[FAILED] '%s'\n", testName);
            }

            printf("Register x%d (expected 0x%08x, actual 0x%08x)\n", i, expected[i], get_register(i));
            allGood = 0;
        }
    }

    if (allGood) {
        printf("[SUCCESS] '%s'\n", testName);
    }
    reset_registers();

    return allGood;
}

void reset_registers(void) {
    for (int i = 0; i < 32; i++) {
        set_register(i, 0);
    }
}

void run_binary(char* path) {
    Program *p = load_program(path);

    int steps = 0;
    while (has_instruction(p) && steps < 100000) {
        InstructionData instructionData = decode(fetch_instruction(p));

        if (instructionData.type == Unknown) // ecall
            break;
      
        execute_instruction(&instructionData, p);
        steps++;
    }
    unload_program(p);
}

void task1(int* total, int* passed) {
    printf("=== Starting Task 1 Tests ===\n");
    int totalStart = *total;
    int passedStart = *passed;
    int expected[32];

    memset(expected, 0, 32 * sizeof(int));
    run_binary("tests/task1/addlarge.bin");
    expected[10] = 0x80000001;
    expected[11] = 0x7ffffffe;
    expected[12] = 0xffffffff;
    expected[17] = 0xa;
    (*total)++;
    (*passed) += compare_registers(expected, "addlarge");
    
    memset(expected, 0, 32 * sizeof(int));
    run_binary("tests/task1/addneg.bin");
    expected[10] = 0xffffffe0;
    expected[11] = 0xffffffc0;
    expected[12] = 0xffffffa0;
    expected[17] = 0xa;
    (*total)++;
    (*passed) += compare_registers(expected, "addneg");
    
    memset(expected, 0, 32 * sizeof(int));
    run_binary("tests/task1/addpos.bin");
    expected[10] = 0x5;
    expected[11] = 0x6;
    expected[12] = 0xb;
    expected[17] = 0xa;
    (*total)++;
    (*passed) += compare_registers(expected, "addpos");
    
    memset(expected, 0, 32 * sizeof(int));
    run_binary("tests/task1/bool.bin");
    expected[5] = 0x12345678;
    expected[6] = 0xabcdefab;
    expected[11] = 0xb9f9b9d3;
    expected[12] = 0xbbfdfffb;
    expected[13] = 0x02044628;
    expected[14] = 0x12345359;
    expected[15] = 0x12345779;
    expected[16] = 0x420;
    expected[17] = 0xa;
    (*total)++;
    (*passed) += compare_registers(expected, "bool");
    
    memset(expected, 0, 32 * sizeof(int));
    run_binary("tests/task1/set.bin");
    expected[5] = 0x123;
    expected[6] = 0x456;
    expected[11] = 0x579;
    expected[12] = 0xfffffccd;
    expected[13] = 0x1;
    expected[14] = 0x1;
    expected[16] = 0x1;
    expected[17] = 0xa;
    (*total)++;
    (*passed) += compare_registers(expected, "set");
    
    memset(expected, 0, 32 * sizeof(int));
    run_binary("tests/task1/shift.bin");
    expected[10] = 0xffff0000;
    expected[11] = 0x0000ffff;
    expected[12] = 0xffffffff;
    expected[17] = 0xa;
    (*total)++;
    (*passed) += compare_registers(expected, "shift");
    
    memset(expected, 0, 32 * sizeof(int));
    run_binary("tests/task1/shift2.bin");
    expected[5] = 0x1;
    expected[10] = 0xabcdefab;
    expected[11] = 0x579bdf56;
    expected[12] = 0x55e6f7d5;
    expected[13] = 0xd5e6f7d5;
    expected[14] = 0x5e6f7d58;
    expected[15] = 0x1579bdf5;
    expected[16] = 0xf579bdf5;
    expected[17] = 0xa;
    (*total)++;
    (*passed) += compare_registers(expected, "shift2");

    printf("[Passed %d/%d Tests]\n", (*passed - passedStart), (*total - totalStart));
}

void task2(int* total, int* passed) {
    printf("=== Starting Task 2 Tests ===\n");
    int totalStart = *total;
    int passedStart = *passed;
    int expected[32];

    memset(expected, 0, 32 * sizeof(int));
    run_binary("tests/task2/branchcnt.bin");
    expected[10] = 0xa;
    expected[11] = 0xa;
    expected[12] = 0xa;
    expected[17] = 0xa;
    (*total)++;
    (*passed) += compare_registers(expected, "branchcnt");

    memset(expected, 0, 32 * sizeof(int));
    run_binary("tests/task2/branchmany.bin");
    expected[10] = 0x3;
    expected[11] = 0x4;
    expected[12] = 0x3;
    expected[17] = 0xa;
    (*total)++;
    (*passed) += compare_registers(expected, "branchmany");

    memset(expected, 0, 32 * sizeof(int));
    run_binary("tests/task2/branchtrap.bin");
    expected[5] = 0xffffff85;
    expected[6] = 0xffffff85;
    expected[7] = 0x7a;
    expected[11] = 0x1;
    expected[12] = 0x2;
    expected[13] = 0x3;
    expected[14] = 0x4;
    expected[15] = 0x5;
    expected[17] = 0xa;
    (*total)++;
    (*passed) += compare_registers(expected, "branchtrap");

    printf("[Passed %d/%d Tests]\n", (*passed - passedStart), (*total - totalStart));
}

void task3(int* total, int* passed) {
    printf("=== Starting Task 3 Tests ===\n");
    int totalStart = *total;
    int passedStart = *passed;
    int expected[32];

    memset(expected, 0, 32 * sizeof(int));
    run_binary("tests/task3/loop.bin");
    expected[1] = 0x8;
    expected[2] = 0x00100000;
    expected[6] = 0x64;
    expected[10] = 0x1356;
    expected[11] = 0x64;
    expected[12] = 0xc80;
    expected[14] = 0x64;
    expected[15] = 0x1356;
    expected[16] = 0x64;
    expected[17] = 0xa;
    (*total)++;
    (*passed) += compare_registers(expected, "loop");

    memset(expected, 0, 32 * sizeof(int));
    run_binary("tests/task3/recursive.bin");
    expected[1] = 0x8;
    expected[2] = 0x100000;
    expected[10] = 0x65;
    expected[15] = 0x65;
    expected[17] = 0xa;
    (*total)++;
    (*passed) += compare_registers(expected, "recursive");

    memset(expected, 0, 32 * sizeof(int));
    run_binary("tests/task3/string.bin");
    expected[1] = 0x49;
    expected[2] = 0x73;
    expected[3] = 0x20;
    expected[4] = 0x69;
    expected[5] = 0x74;
    expected[6] = 0x20;
    expected[7] = 0x6d;
    expected[8] = 0x65;
    expected[9] = 0x20;
    expected[10] = 0x10c;
    expected[11] = 0x79;
    expected[12] = 0x6f;
    expected[13] = 0x75;
    expected[14] = 0x27;
    expected[15] = 0x72;
    expected[16] = 0x65;
    expected[17] = 0xa;
    expected[18] = 0x20;
    expected[19] = 0x6c;
    expected[20] = 0x6f;
    expected[21] = 0x6f;
    expected[22] = 0x6b;
    expected[23] = 0x69;
    expected[24] = 0x6e;
    expected[25] = 0x67;
    expected[26] = 0x20;
    expected[27] = 0x66;
    expected[28] = 0x6f;
    expected[29] = 0x72;
    expected[30] = 0x3f;
    (*total)++;
    (*passed) += compare_registers(expected, "string");

    memset(expected, 0, 32 * sizeof(int));
    run_binary("tests/task3/width.bin");
    expected[5] = 0xabcdefab;
    expected[6] = 0x7f;
    expected[11] = 0xffffffab;
    expected[12] = 0xffffefab;
    expected[13] = 0xabcdefab;
    expected[14] = 0xab;
    expected[15] = 0xefab;
    expected[16] = 0xabefabab;
    expected[17] = 0xa;
    (*total)++;
    (*passed) += compare_registers(expected, "width");

    printf("[Passed %d/%d Tests]\n", (*passed - passedStart), (*total - totalStart));
}

void custom(int* total, int* passed) {
    printf("=== Starting Custom Tests ===\n");
    int totalStart = *total;
    int passedStart = *passed;
    int expected[32];

    memset(expected, 0, 32 * sizeof(int));
    run_binary("tests/custom/endianness1.bin");
    expected[2] = 0x3;
    expected[5] = 0x01020304;
    expected[6] = 0x1;
    expected[7] = 0x2;
    expected[17] = 0xa;
    expected[28] = 0x3;
    expected[29] = 0x4;
    (*total)++;
    (*passed) += compare_registers(expected, "endianness1");

    memset(expected, 0, 32 * sizeof(int));
    run_binary("tests/custom/endianness2.bin");
    expected[2] = 0x3;
    expected[5] = 0x01020304;
    expected[6] = 0x1;
    expected[7] = 0x2;
    expected[17] = 0xa;
    expected[28] = 0x3;
    expected[29] = 0x4;
    (*total)++;
    (*passed) += compare_registers(expected, "endianness2");


    printf("[Passed %d/%d Tests]\n", (*passed - passedStart), (*total - totalStart));
}

int main(void) {
    int totalTests = 0;
    int passedTests = 0;
    task1(&totalTests, &passedTests);
    task2(&totalTests, &passedTests);
    task3(&totalTests, &passedTests);
    custom(&totalTests, &passedTests);
    printf("=== Done with all tests ===\n");
    printf("Ran    %d tests in total\n", totalTests);
    printf("Passed %d tests in total\n", passedTests);
    printf("Passed %.02f%% of all tests\n", ((float)passedTests / totalTests) * 100);
}