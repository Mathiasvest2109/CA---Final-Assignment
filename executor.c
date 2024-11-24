#include "executor.h"
#include <stdio.h>

void execute_instruction(InstructionData* instructionData) {
    // if no instructionData
    if (instructionData == NULL || instructionData->data == NULL) {
        return;
    }

    switch (instructionData->type) {
        case R: {
            // allows access to the fields specific to InstructionR after decoding the instruction
            InstructionR* instr = (InstructionR*)instructionData->data;

            // R-type instructions
            switch (instr->opcode) {
                case ADD: {
                    int result = get_register(instr->rs1) + get_register(instr->rs2);
                    set_register(instr->rd, result);
                    break;
                }
                case SUB: {
                    int result = get_register(instr->rs1) - get_register(instr->rs2);
                    set_register(instr->rd, result);
                    break;
                }
                case SLT: {
                    int result = get_register(instr->rs1) < get_register(instr->rs2) ? 1 : 0; // condition ? value_if_true : value_if_false
                    set_register(instr->rd, result);
                    break;
                }
                case SLTU: {
                    unsigned int result = (unsigned int)get_register(instr->rs1) < (unsigned int)get_register(instr->rs2) ? 1 : 0; 
                    set_register(instr->rd, result);
                    break;
                }
                case AND: {
                    int result = get_register(instr->rs1) & get_register(instr->rs2);
                    set_register(instr->rd, result);
                    break;
                }
                case OR: {
                    int result = get_register(instr->rs1) | get_register(instr->rs2);
                    set_register(instr->rd, result);
                    break;
                }
                case XOR: {
                    int result = get_register(instr->rs1) ^ get_register(instr->rs2);
                    set_register(instr->rd, result);
                    break;
                }
                case SLL: { 
                    int result = get_register(instr->rs1) << get_register(instr->rs2);
                    set_register(instr->rd, result);
                    break;
                }
                case SRL: {

                    unsigned int shift_amoumt = get_register(instr->rs2) & 0x1F;  // get lower 5 bits of rs2
                    unsigned int result = (unsigned int)get_register(instr->rs1) >> shift_amoumt; //logical right shifts on the value in register rs1
                    set_register(instr->rd, result);
                    break;
                    
                }
                case SRA: {
                    int result = get_register(instr->rs1) >> get_register(instr->rs2);
                    set_register(instr->rd, result);
                    break;
                }
                default:
                    // idk girl
                    break;
            }
            break;
        }
        
        case I: {
            InstructionI* instr = (InstructionI*)instructionData->data;

            // I-type instructions (e.g., ADDI, SLTI, etc.)
            switch (instr->opcode) {
                case ADDI: {
                    int result = get_register(instr->rs1) + instr->immediate;
                    set_register(instr->rd, result);
                    break;
                }
                case SLTI: {
                    int result = get_register(instr->rs1) < instr->immediate ? 1 : 0;
                    set_register(instr->rd, result);
                    break;
                }
                case SLTIU: {
                    unsigned int result = (unsigned int)get_register(instr->rs1) < (unsigned int)instr->immediate ? 1 : 0;
                    set_register(instr->rd, result);
                    break;
                }
                case ANDI: {
                    int result = get_register(instr->rs1) & instr->immediate;
                    set_register(instr->rd, result);
                    break;
                }
                case ORI: {
                    int result = get_register(instr->rs1) | instr->immediate;
                    set_register(instr->rd, result);
                    break;
                }
                case XORI: {
                    int result = get_register(instr->rs1) ^ instr->immediate;
                    set_register(instr->rd, result);
                    break;
                }
                case SLLI: {
                    int result = get_register(instr->rs1) << instr->immediate;
                    set_register(instr->rd, result);
                    break;
                }
                case SRLI: {
                    unsigned int shift_amount = instr->immediate & 0x1F; // immediate shift amount
                    unsigned int result = (unsigned int)get_register(instr->rs1) >> shift_amount; // logical right shift
                    set_register(instr->rd, result);
                    break;
                }
                case SRAI: {
                    int result = get_register(instr->rs1) >> instr->immediate;
                    set_register(instr->rd, result);
                    break;
                }
                case JALR: {
                    // int target = PC + 4;
                    // goto rs1 + imm12
                    // set_register(instr->rd) = target; 
                    // will continue when Task 2 gets Implemented
                    break;
                }
                case LW: {
                    // Waiting for Task 3 to be implemented
                    // Also need, LH, LHU, LB, LBU
                    break;
                }
                default:
                    //idk girl
                    break;
            }
            break;
        }

        case S: {
            InstructionI* instr = (InstructionI*)instructionData->data;

            // S-type instructions (e.g., ADDI, SLTI, etc.)
            switch (instr->opcode) {
                case SW: {
                    // Awaiting Task 3 implementation
                    // Also need SH, SB
                    break;
                }
                default:
                    //idk girl
                    break;
            }
            break;
        }

        case B: {
            InstructionB* instr = (InstructionB*)instructionData->data;

            // B-type instructions (e.g., BEQ, BNE, etc.)
            switch (instr->opcode) {
                case BEQ: {
                    if (get_register(instr->rs1) == get_register(instr->rs2)) {
                        // PC += instr->immediate << 1;
                    } else {
                        // PC += 4;
                    }
                    break;
                }
                case BNE: {
                    if (get_register(instr->rs1) != get_register(instr->rs2)) {
                        // PC += instr->immediate << 1;
                    } else {
                        // PC += 4;
                    }
                    break;
                }
                case BLT: {
                    if (get_register(instr->rs1) < get_register(instr->rs2)) {
                        // PC += instr->immediate << 1;
                    } else {
                        // PC += 4;
                    }
                    break;
                }
                case BGE: {
                    if (get_register(instr->rs1) >= get_register(instr->rs2)) {
                        // PC += instr->immediate << 1;
                    } else {
                        // PC += 4;
                    }
                    break;
                }
                case BLTU: {
                    if ((unsigned int)get_register(instr->rs1) < (unsigned int)get_register(instr->rs2)) {
                        // PC += instr->immediate << 1;
                    } else {
                        // PC += 4;
                    }
                    break;
                }
                case BGEU: {
                    if ((unsigned int)get_register(instr->rs1) >= (unsigned int)get_register(instr->rs2)) {
                        // PC += instr->immediate << 1;
                    } else {
                        // PC += 4;
                    }
                    break;
                }
                default:
                    // idk girl
                    break;
            }
            break;
        }

        case U: {
            InstructionU* instr = (InstructionU*)instructionData->data;

            // U-type instructions (e.g., LUI, AUIPC)
            switch (instr->opcode) {
                case LUI: {
                    set_register(instr->rd, instr->immediate << 12); // load upper immediate into the destination register
                    break;
                }
                case AUIPC: {
                    // set_register(instr->rd) = PC + (instr->immediate << 12); // add upper immediate to PC and store the result in rd
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
