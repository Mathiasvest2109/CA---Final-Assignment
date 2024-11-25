#include "executor.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "memory.h"

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

            switch (instr->opcode) {
                case JALR:
                    set_register(instr->rd, program->pc);              // save return address in rd, SKAL DEN VOKSE MED 4 ELLER IKKE?
                    program->pc = (get_register(instr->rs1) + instr->immediate) & ~1; // go to target address, and ensure even number
                    break;
                case LW:
                    // Waiting for Task 3 to be implemented
                    int word = load_word(get_register(instr->rs1) + instr->immediate);
                    set_register(instr->rd, word);
                    break;
                case LH:

                    int half_word = load_half(get_register(instr->rs1) + instr->immediate);
                    set_register(instr->rd, half_word);
                    break;
                case LHU:

                    int unsigned_half_word = load_half(get_register(instr->rs1) + instr->immediate);//unsigned
                    set_register(instr->rd, unsigned_half_word & 0xffff); 
                    break;
                case LB:

                    int byte = load_byte(get_register(instr->rs1) + instr->immediate);
                    set_register(instr->rd, byte);
                    break;
                case LBU:

                    int unsigned_byte = load_byte(get_register(instr->rs1) + instr->immediate);
                    set_register(instr->rd, unsigned_byte & 0xff);
                    break;
                default:
                    int result = alu(get_register(instr->rs1), instr->immediate, instr->opcode);
                    set_register(instr->rd, result);
                    break;
            }
            break;
        }

        case S: {
            InstructionS* instr = (InstructionS*)instructionData->data;

            // S-type instructions 
            switch (instr->opcode) {
                case SW: {
                    store_word(get_register(instr->rs1) + instr->immediate, get_register(instr->rs2));
                    break;
                }
                case SH:
                    store_half(get_register(instr->rs1) + instr->immediate, get_register(instr->rs2) & 0xffff); //bit masking to get 16 bits
                    break;

                case SB:
                    store_byte(get_register(instr->rs1) + instr->immediate, get_register(instr->rs2) & 0xff); //bit masking to get 8 bits
                    break;

                default:
                    //idk girl
                    break;
            }
            break;
        }

        case B: {
            InstructionB* instr = (InstructionB*)instructionData->data;

            bool shouldBranch;
            // B-type instructions
            switch (instr->opcode) {
                case BEQ:
                    shouldBranch = (get_register(instr->rs1) == get_register(instr->rs2));
                    break;
                case BNE:
                    shouldBranch = (get_register(instr->rs1) != get_register(instr->rs2));
                    break;
                case BLT:
                    shouldBranch = (get_register(instr->rs1) < get_register(instr->rs2));
                    break;
                case BGE:
                    shouldBranch = (get_register(instr->rs1) >= get_register(instr->rs2));
                    break;
                case BLTU:
                    shouldBranch = ((unsigned int)get_register(instr->rs1) < (unsigned int)get_register(instr->rs2));
                    break;
                case BGEU:
                    shouldBranch = ((unsigned int)get_register(instr->rs1) >= (unsigned int)get_register(instr->rs2));
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

            // J-type instruction (e.g., JAL)
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