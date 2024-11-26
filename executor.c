#include "executor.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "memory.h"
#include <limits.h>

int alu(int arg1, int arg2, Opcode instruction);

void execute_instruction(InstructionData* instructionData, Program* program) {
    // if no instructionData
    if (instructionData == NULL || instructionData->data == NULL) {
        return;
    }

    switch (instructionData->type) {
        case R: {
            // allows access to the fields specific to InstructionR after decoding the instruction
            InstructionR* instr = (InstructionR*)instructionData->data;

            int result = alu(get_register(instr->rs1), get_register(instr->rs2), instr->opcode);
            set_register(instr->rd, result);
            break;
        }
        
        case I: {
            InstructionI* instr = (InstructionI*)instructionData->data;

            int result;
            switch (instr->opcode) {
                case JALR:
                    // save return address in rd, SKAL DEN VOKSE MED 4 ELLER IKKE?
                    result = program->pc;
                    program->pc = (get_register(instr->rs1) + instr->immediate) & ~1; // go to target address, and ensure even number
                    break;
                case LW:
                    result = load_word(get_register(instr->rs1) + instr->immediate);
                    break;
                case LH:
                    result = load_half(get_register(instr->rs1) + instr->immediate);
                    break;
                case LHU:
                    result = load_half(get_register(instr->rs1) + instr->immediate);
                    result &= 0xffff; // cut off the sign-extended part
                    break;
                case LB:
                    result = load_byte(get_register(instr->rs1) + instr->immediate);
                    break;
                case LBU:
                    result = load_byte(get_register(instr->rs1) + instr->immediate);
                    result &= 0xff; // cut off the sign-extended part
                    break;
                default: // it's an ALU instruction, with an immediate argument instead of a register
                    result = alu(get_register(instr->rs1), instr->immediate, instr->opcode);
                    break;
            }
            set_register(instr->rd, result);
            break;
        }

        case S: {
            InstructionS* instr = (InstructionS*)instructionData->data;
            int address = get_register(instr->rs1) + instr->immediate;
            int toSave = get_register(instr->rs2);

            // S-type instructions 
            switch (instr->opcode) {
                case SW:
                    // all 32 bits
                    store_word(address, toSave);
                    break;
                case SH:
                    // only the lower 16 bits
                    store_half(address, toSave & 0xffff);
                    break;
                case SB:
                    // only the lower 8 bits
                    store_byte(address, toSave & 0xff);
                    break;
                default:
                    //idk girl
                    break;
            }
            break;
        }

        case B: {
            InstructionB* instr = (InstructionB*)instructionData->data;
            int arg1 = get_register(instr->rs1);
            int arg2 = get_register(instr->rs2);

            bool shouldBranch;
            // B-type instructions
            switch (instr->opcode) {
                case BEQ:
                    shouldBranch = (arg1 == arg2);
                    break;
                case BNE:
                    shouldBranch = (arg1 != arg2);
                    break;
                case BLT:
                    shouldBranch = (arg1 < arg2);
                    break;
                case BGE:
                    shouldBranch = (arg1 >= arg2);
                    break;
                case BLTU:
                    shouldBranch = ((unsigned int)arg1 < (unsigned int)arg2);
                    break;
                case BGEU:
                    shouldBranch = ((unsigned int)arg1 >= (unsigned int)arg2);
                    break;
                default:
                    shouldBranch = false;
                    // idk girl
                    break;
            }
            
            // pc has already been incremented, so the branch target is off by 4, and the noBranch is just the pc itself
            int branchTarget = program->pc - 4 + (instr->immediate << 1);
            int noBranchTarget = program->pc;

            program->pc = (shouldBranch) ? (branchTarget) : (noBranchTarget);
            break;
        }

        case U: {
            InstructionU* instr = (InstructionU*)instructionData->data;

            // U-type instructions 
            switch (instr->opcode) {
                case LUI: {
                    set_register(instr->rd, instr->immediate << 12); // load upper immediate into the destination register
                    break;
                }
                case AUIPC: {
                    int target = program->pc - 4 + (instr->immediate << 12); // add upper immediate to PC and store the result in rd
                    set_register(instr->rd, target); // Compute address relative to PC
                    break;
                }
                default:
                    // idk girl
                    break;
            }
            break;
        }

        case J: {
            InstructionJ* instr = (InstructionJ*)instructionData->data;

            // J-type instruction (e.g. JAL)
            switch (instr->opcode) {
                case JAL: {
                    // rd = pc + 4
                    // goto pc + (imm20 << 1)
                    set_register(instr->rd, program->pc);                        // Save return address
                    program->pc = program->pc - 4 + (instr->immediate << 1);    // Jump to target
                    break;
                }
                default:
                    // idk girl
                    break;
            }
            break;
        }

        case Unknown:
            int arg = get_register(10); // a0
            switch (get_register(17))   // a7
            {
                case 1: // print int
                    printf("%d\n", arg);
                    break;
                case 2: // print float
                    printf("%f\n", (float)arg);
                    break;
                case 4: // print string
                    printf("%s\n", (char*)get_c_address(arg));
                    break;
                case 93: // exit with status code
                    program->statusCode = get_register(10);
                    // fallthrough
                case 10: // exit
                    program->isHalting = true;
                    break;
                case 11: // print char
                    printf("%c\n", (char)(arg & 0xff));
                    break;
                case 34: // print hex
                    printf("%x\n", arg);
                    break;
                case 35: // print binary
                    unsigned int mask = 1 << (sizeof(int) * CHAR_BIT - 1); // Start with the leftmost bit

                    while (mask) {
                        printf("%d", (arg & mask) ? 1 : 0); // Check if the corresponding bit is set
                        mask >>= 1; // Shift mask to the right
                    }

                    printf("\n");
                    break;
                case 36: // print unsigned
                    printf("%u\n",arg);
                    break;
                default:
                    break;
            }
        default:
            break;
    }
}

int alu(int arg1, int arg2, Opcode instruction) {
    switch (instruction) {
        // == ARITHMETIC ==
        case ADD:
        case ADDI:
            return arg1 + arg2;
        case SUB:
            return arg1 - arg2;
        // == LOGIC ==
        case SLT:
        case SLTI:
            return (arg1 < arg2) ? 1 : 0;
        case SLTU:
        case SLTIU:
            return ((unsigned int)arg1 < (unsigned int)arg2) ? 1 : 0;
        case AND:
        case ANDI:
            return arg1 & arg2;
        case OR:
        case ORI:
            return arg1 | arg2;
        case XOR:
        case XORI:
            return arg1 ^ arg2;
        // == SHIFTING ==
        case SLL:
        case SLLI:
            return arg1 << (arg2 & 0x1F);
        case SRL:
        case SRLI:
            return (unsigned int)arg1 >> (arg2 & 0x1F);
        case SRA:
        case SRAI:
            return arg1 >> (arg2 & 0x1F);
        default:
            return 0;
    }
}