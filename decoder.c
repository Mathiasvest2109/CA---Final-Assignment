#include "decoder.h"
#include <stdlib.h>
#include <stdio.h>

// Bit manipulations to extract relevant info from the instruction-number
#define GET_OPCODE(instruction) (instruction & 0b111111)
#define GET_RD(instruction)  ((instruction & (0b11111 << 7)) >> 7)
#define GET_RS1(instruction) ((instruction & (0b11111 << 15)) >> 15)
#define GET_RS2(instruction) ((instruction & (0b11111 << 20)) >> 20)
#define GET_FUNCT3(instruction) ((instruction & (0b111 << 12)) >> 12)
#define GET_FUNCT7(instruction) ((instruction & (0b1111111 << 25)) >> 25)

// Get the immediate values for different instruction types
#define GET_I_IMMEDIATE(instruction) ((instruction & (0b111111111111 << 20)) >> 20)
#define GET_S_IMMEDIATE(instruction) (((instruction & (0b11111 << 7)) >> 7) | ((instruction & (0b1111111 << 25)) >> 20))
#define GET_B_IMMEDIATE(instruction) (((instruction & (0b1111 << 8)) >> 7) | ((instruction & (0b111111 << 25)) >> 20) | ((instruction & (0b1 << 7)) << 4) | ((instruction & (0b1 << 31)) >> 19))
#define GET_U_IMMEDIATE(instruction) (instruction & 0xFFFFF000)
#define GET_J_IMMEDIATE(instruction) ((instruction & (0b1111111111 << 21)) >> 20) | ((instruction & (0b1 << 20)) >> 9) | (instruction & (0b11111111 << 12)) | ((instruction & (0b1 << 31)) >> 11)


InstructionData decode(int instructionValue) {
    int opcode = GET_OPCODE(instructionValue);
    InstructionData decoded;

    switch (opcode) {
        case 0b0110011: // add, or, and, etc.
            decoded.type = R;
            break;
        case 0b0010011: // addi, slti, andi, etc.
        case 0b1100111: // jalr
        case 0b0000011: // lw, lr, lhu, etc.
            decoded.type = I;
            break;
        case 0b0100011: // sw, sr, sb, etc.
            decoded.type = S;
            break;
        case 0b1100011: // beq, bne, blt, etc.
            decoded.type = B;
            break;
        case 0b0110111: // lui
        case 0b0010111: // auipc
            decoded.type = U;
            break;
        case 0b1101111: // jal
            decoded.type = J;
            break;
        case 0b1110011: // ecall and ebreak
            decoded.type = Unknown;

            int funct12 = (0xFFF << 20) & instructionValue;
            InstructionUnknown* environmentInstruction = malloc(sizeof(InstructionUnknown));
            environmentInstruction->opcode = (funct12 == 0) ? ECALL : EBREAK;
            environmentInstruction->instruction = instructionValue;

            decoded.data = environmentInstruction;
            return decoded;
        default:
            decoded.type = Unknown;
            decoded.data = NULL;
            return decoded;
    }

    switch (decoded.type)
    {
        case R: {
            InstructionR* instruction = malloc(sizeof(InstructionR));
            instruction->rd = GET_RD(instructionValue);
            instruction->rs1 = GET_RS1(instructionValue);
            instruction->rs2 = GET_RS2(instructionValue);
            instruction->opcode = Invalid;

            // Set the "opcode", based on GET_FUNCT3(instructionValue), and possibly GET_FUNCT7(instructionValue)
            if (opcode == 0b0110011) {
                switch (GET_FUNCT3(instructionValue)) {
                    case 0b000:
                        switch (GET_FUNCT7(instructionValue)) {
                            case 0b0000000:
                                instruction->opcode = ADD;
                                break;
                            case 0b0100000:
                                instruction->opcode = SUB;
                                break;
                        }
                        break;
                    case 0b010:
                        instruction->opcode = SLT;
                        break;
                    case 0b011:
                        instruction->opcode = SLTU;
                        break;
                    case 0b111:
                        instruction->opcode = AND;
                        break;
                    case 0b110:
                        instruction->opcode = OR;
                        break;
                    case 0b100:
                        instruction->opcode = XOR;
                        break;
                    case 0b001:
                        instruction->opcode = SLL;
                        break;
                    case 0b101:
                        switch (GET_FUNCT7(instructionValue)) {
                            case 0b0000000:
                                instruction->opcode = SRL;
                                break;
                            case 0b0100000:
                                instruction->opcode = SRA;
                                break;
                        }
                        break;
                    default:
                        // huh
                        break;
                }
            }

            decoded.data = instruction;
            break;
        }
        case I: {
            InstructionI* instruction = malloc(sizeof(InstructionI));
            instruction->rd = GET_RD(instructionValue);
            instruction->rs1 = GET_RS1(instructionValue);
            instruction->immediate = GET_I_IMMEDIATE(instructionValue);
            instruction->opcode = Invalid;

            if (opcode == 0b0010011) {
                switch (GET_FUNCT3(instructionValue)) {
                    case 0b000:
                        instruction->opcode = ADDI;
                        break;
                    case 0b010:
                        instruction->opcode = SLTI;
                        break;
                    case 0b011:
                        instruction->opcode = SLTIU;
                        break;
                    case 0b111:
                        instruction->opcode = ANDI;
                        break;
                    case 0b110:
                        instruction->opcode = ORI;
                        break;
                    case 0b100:
                        instruction->opcode = XORI;
                        break;
                    case 0b001:
                        instruction->opcode = SLLI;
                        break;
                    case 0b101:
                        instruction->opcode = (GET_FUNCT7(instructionValue) == 0b0000000) ? SRLI : SRAI;
                        break;
                    default:
                        // huh
                        break;
                }
            }
            else if (opcode == 0b1100111) {
                instruction->opcode = JALR;
            }
            else if (opcode == 0b0000011) {
                // TODO
            }
            if (opcode == 0b0100011) {
                switch (GET_FUNCT3(instructionValue)) {
                    case 0b010:
                        instruction->opcode = LW;
                        break;
                    case 0b001:
                        instruction->opcode = LH;
                        break;
                    case 0b101:
                        instruction->opcode = LHU;
                        break;
                    case 0b000:
                        instruction->opcode = LB;
                        break;
                    case 0b100:
                        instruction->opcode = LBU;
                        break;
                }
            }

            decoded.data = instruction;
            break;
        }
        case S: {
            InstructionS* instruction = malloc(sizeof(InstructionS));
            instruction->rs1 = GET_RS1(instructionValue);
            instruction->rs2 = GET_RS2(instructionValue);
            instruction->opcode = Invalid;

            if (opcode == 0b0100011) {
                switch (GET_FUNCT3(instructionValue)) {
                    case 0b010:
                        instruction->opcode = SW;
                        break;
                    case 0b001:
                        instruction->opcode = SH;
                        break;
                    case 0b000:
                        instruction->opcode = SB;
                        break;
                }
            }
            // Set the "opcode" as well

            instruction->immediate = GET_S_IMMEDIATE(instructionValue);

            decoded.data = instruction;
            break;
        }
        case B: {
            InstructionB* instruction = malloc(sizeof(InstructionB));
            instruction->rs1 = GET_RS1(instructionValue);
            instruction->rs2 = GET_RS2(instructionValue);
            instruction->opcode = Invalid;
            switch (GET_FUNCT3(instructionValue)) {
                case 0b000:
                    instruction->opcode = BEQ;
                    break;
                case 0b001:
                    instruction->opcode = BNE;
                    break;
                case 0b100:
                    instruction->opcode = BLT;
                    break;
                case 0b101:
                    instruction->opcode = BGE;
                    break;
                case 0b110:
                    instruction->opcode = BLTU;
                    break;
                case 0b111:
                    instruction->opcode = BGEU;
            }

            instruction->immediate = GET_B_IMMEDIATE(instructionValue);
            
            decoded.data = instruction;
            break;
        }
        case U: {
            InstructionU* instruction = malloc(sizeof(InstructionU));
            instruction->rd = GET_RD(instructionValue);
            instruction->immediate = GET_U_IMMEDIATE(instructionValue);
            // The only 2 U-type instructions (in this part of RISC-V at least)
            instruction->opcode = (opcode == 0b0110111) ? LUI : AUIPC;
            
            decoded.data = instruction;
            break;
        }
        case J: {
            InstructionJ* instruction = malloc(sizeof(InstructionJ));
            instruction->rd = GET_RD(instructionValue);
            // It's the only J-type instruction (in this part of RISC-V at least)
            instruction->opcode = JAL;

            instruction->immediate = GET_J_IMMEDIATE(instructionValue);
            
            decoded.data = instruction;
            break;
        }
        default: {
            // TODO
            exit(1);
        }
    }

    return decoded;
}