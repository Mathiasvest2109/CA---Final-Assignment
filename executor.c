#include "executor.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

void execute_instruction(InstructionData* instructionData, Program* program) {
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

            // I-type instructions
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
                    set_register(instr->rd, program->pc);              // save return address in rd, SKAL DEN VOKSE MED 4 ELLER IKKE?
                    program->pc = (get_register(instr->rs1) + instr->immediate) & ~1; // go to target address, and ensure even number
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

            // S-type instructions 
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

            bool shouldBranch;
            // B-type instructions
            switch (instr->opcode) {
                case BEQ: {
                    shouldBranch = (get_register(instr->rs1) == get_register(instr->rs2));
                    break;
                }
                case BNE: {
                    shouldBranch = (get_register(instr->rs1) != get_register(instr->rs2));
                    break;
                }
                case BLT: {
                    shouldBranch = (get_register(instr->rs1) < get_register(instr->rs2));
                    break;
                }
                case BGE: {
                    shouldBranch = (get_register(instr->rs1) >= get_register(instr->rs2));
                    break;
                }
                case BLTU: {
                    shouldBranch = ((unsigned int)get_register(instr->rs1) < (unsigned int)get_register(instr->rs2));
                    break;
                }
                case BGEU: {
                    shouldBranch = ((unsigned int)get_register(instr->rs1) >= (unsigned int)get_register(instr->rs2));
                    break;
                }
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
