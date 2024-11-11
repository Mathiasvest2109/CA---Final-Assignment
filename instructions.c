#include <stdlib.h>
#include <stdio.h>
#include "instructions.h"

Instruction decode(int instructionValue) {
    int opcode = instructionValue & OPCODE_MASK;
    Instruction decoded;

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

    // Just mask out all the common parts. Some may not be used, but it's better to keep the code DRY
    int rd = instructionValue & RD_MASK;
    int rs1 = instructionValue & RS1_MASK;
    int rs2 = instructionValue & RS2_MASK;
    int funct3 = instructionValue & FUNCT3_MASK;

    int immediate;
    switch (decoded.type)
    {
        case R:
            InstructionR* registerInstruction = malloc(sizeof(InstructionR));
            registerInstruction->rd = rd;
            registerInstruction->rs1 = rs1;
            registerInstruction->rs2 = rs2;
            registerInstruction->opcode = Unknown;

            // Set the "opcode", based on funct3, and possibly funct7
            if (opcode == 0b0110011) {
                int funct7 = (instructionValue & (0b1111111 << 25)) >> 25;
                switch (funct3) {
                    case 0b000:
                        switch (funct7) {
                            case 0b0000000:
                                registerInstruction->opcode = ADD;
                                break;
                            case 0b0100000:
                                registerInstruction->opcode = SUB;
                                break;
                        }
                        break;
                    case 0b010:
                        registerInstruction->opcode = SLT;
                        break;
                    case 0b011:
                        registerInstruction->opcode = SLTU;
                        break;
                    case 0b111:
                        registerInstruction->opcode = AND;
                        break;
                    case 0b110:
                        registerInstruction->opcode = OR;
                        break;
                    case 0b100:
                        registerInstruction->opcode = XOR;
                        break;
                    case 0b001:
                        registerInstruction->opcode = SLL;
                        break;
                    case 0b101:
                        switch (funct7) {
                            case 0b0000000:
                                registerInstruction->opcode = SRL;
                                break;
                            case 0b0100000:
                                registerInstruction->opcode = SRA;
                                break;
                        }
                        break;
                    default:
                        // huh
                        break;
                }
            }

            decoded.data = registerInstruction;
            break;
        case I:
            InstructionI* immediateInstruction = malloc(sizeof(InstructionI));
            immediateInstruction->rd = rd;
            immediateInstruction->rs1 = rs1;
            immediateInstruction->immediate = instructionValue & (0xFFF << 20);
            immediateInstruction->opcode = Unknown;

            if (opcode == 0b0010011) {
                switch (funct3) {
                    case 0b000:
                        registerInstruction->opcode = ADDI;
                        break;
                    case 0b010:
                        registerInstruction->opcode = SLTI;
                        break;
                    case 0b011:
                        registerInstruction->opcode = SLTIU;
                        break;
                    case 0b111:
                        registerInstruction->opcode = ANDI;
                        break;
                    case 0b110:
                        registerInstruction->opcode = ORI;
                        break;
                    case 0b100:
                        registerInstruction->opcode = XORI;
                        break;
                    case 0b001:
                        registerInstruction->opcode = SLLI;
                        break;
                    case 0b101:
                        int funct7 = (instructionValue & (0b1111111 << 25)) >> 25;
                        registerInstruction->opcode = (funct7 == 0b0000000) ? SRLI : SRAI;
                        break;
                    default:
                        // huh
                        break;
                }
            }
            else if (opcode == 0b1100111) {
                registerInstruction->opcode = JALR;
            }
            else if (opcode == 0b0000011) {
                // TODO
            }
            if (opcode == 0b0100011) {
                switch (funct3) {
                    case 0b010:
                        registerInstruction->opcode = LW;
                        break;
                    case 0b001:
                        registerInstruction->opcode = LH;
                        break;
                    case 0b101:
                        registerInstruction->opcode = LHU;
                        break;
                    case 0b000:
                        registerInstruction->opcode = LB;
                        break;
                    case 0b100:
                        registerInstruction->opcode = LBU;
                        break;
                }
            }
            // Set the "opcode" as well

            decoded.data = immediateInstruction;
            break;
        case S:
            InstructionS* saveInstruction = malloc(sizeof(InstructionS));
            saveInstruction->rs1 = rs1;
            saveInstruction->rs2 = rs2;
            saveInstruction->opcode = Unknown;

            if (opcode == 0b0100011) {
                switch (funct3) {
                    case 0b010:
                        saveInstruction->opcode = SW;
                        break;
                    case 0b001:
                        saveInstruction->opcode = SH;
                        break;
                    case 0b000:
                        saveInstruction->opcode = SB;
                        break;
                }
            }
            // Set the "opcode" as well

            immediate  = (instructionValue & (0xF << 7)) >> 7;
            immediate |= (instructionValue & (0b111111 << 25)) >> (25 - 7);
            saveInstruction->immediate = immediate;
            break;
        case B:
            InstructionB* branchInstruction = malloc(sizeof(InstructionB));
            saveInstruction->rs1 = rs1;
            saveInstruction->rs2 = rs2;
            saveInstruction->opcode = Unknown;
            switch (funct3) {
                case 0b000:
                    branchInstruction->opcode = BEQ;
                    break;
                case 0b001:
                    branchInstruction->opcode = BNE;
                    break;
                case 0b100:
                    branchInstruction->opcode = BLT;
                    break;
                case 0b101:
                    branchInstruction->opcode = BGE;
                    break;
                case 0b110:
                    branchInstruction->opcode = BLTU;
                    break;
                case 0b111:
                    branchInstruction->opcode = BGEU;
            }
            // Set the "opcode" as well

            immediate  = (instructionValue & (0b111 << 7)) >> (7 - 1);
            immediate |= (instructionValue & (0b11111 << 24)) >> (24 - 5);
            immediate |= (instructionValue & (0b1 << 6)) << (11 - 6);
            immediate |= (instructionValue & (0b1 << 31)) >> (31 - 12);
            saveInstruction->immediate = immediate;
            break;
        case U:
            InstructionU* upperInstruction = malloc(sizeof(InstructionU));
            upperInstruction->rd = rd;
            upperInstruction->immediate = instructionValue & (~0xFFF);
            // The only 2 U-type instructions (in this part of RISC-V at least)
            upperInstruction->opcode = (opcode == 0b0110111) ? LUI : AUIPC;
            break;
        case J:
            InstructionJ* upperInstruction = malloc(sizeof(InstructionJ));
            upperInstruction->rd = rd;
            // It's the only J-type instruction (in this part of RISC-V at least)
            upperInstruction->opcode = JAL;

            immediate  = (instructionValue & (0b111111111 << 21)) >> (21 - 1);
            immediate |= (instructionValue & (0b1 << 20)) >> (20 - 11);
            immediate |= (instructionValue & (0b1111111 << 12)); // >> (12 - 12);
            immediate |= (instructionValue & (0b1 << 31)) >> (31 - 20);
            upperInstruction->immediate = immediate;
            break;
    }

    return decoded;
}