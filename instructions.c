#include <stdlib.h>
#include <stdio.h>
#include "instructions.h"

// Bit manipulations to extract relevant info from the instruction-number
#define GET_OPCODE(instruction) (instruction & 0b111111)
#define GET_RD(instruction)  ((instruction & (0b11111 << 7)) >> 7)
#define GET_RS1(instruction) ((instruction & (0b11111 << 15)) >> 15)
#define GET_RS2(instruction) ((instruction & (0b11111 << 20)) >> 20)
#define GET_FUNCT3(instruction) ((instruction & (0b111 << 12)) >> 12)
#define GET_FUNCT7(instruction) ((instruction & (0b1111111 << 25)) >> 25)

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

    // Just mask out all the common parts. Some may not be used, but it's better to keep the code DRY
    int rd = GET_RD(instructionValue);
    int rs1 = GET_RS1(instructionValue);
    int rs2 = GET_RS2(instructionValue);
    int funct3 = GET_FUNCT3(instructionValue);

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
                int funct7 = GET_FUNCT7(instructionValue);
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
            immediateInstruction->immediate = GET_I_IMMEDIATE(instructionValue);
            immediateInstruction->opcode = Unknown;

            if (opcode == 0b0010011) {
                switch (funct3) {
                    case 0b000:
                        immediateInstruction->opcode = ADDI;
                        break;
                    case 0b010:
                        immediateInstruction->opcode = SLTI;
                        break;
                    case 0b011:
                        immediateInstruction->opcode = SLTIU;
                        break;
                    case 0b111:
                        immediateInstruction->opcode = ANDI;
                        break;
                    case 0b110:
                        immediateInstruction->opcode = ORI;
                        break;
                    case 0b100:
                        immediateInstruction->opcode = XORI;
                        break;
                    case 0b001:
                        immediateInstruction->opcode = SLLI;
                        break;
                    case 0b101:
                        int funct7 = GET_FUNCT7(instructionValue);
                        immediateInstruction->opcode = (funct7 == 0b0000000) ? SRLI : SRAI;
                        break;
                    default:
                        // huh
                        break;
                }
            }
            else if (opcode == 0b1100111) {
                immediateInstruction->opcode = JALR;
            }
            else if (opcode == 0b0000011) {
                // TODO
            }
            if (opcode == 0b0100011) {
                switch (funct3) {
                    case 0b010:
                        immediateInstruction->opcode = LW;
                        break;
                    case 0b001:
                        immediateInstruction->opcode = LH;
                        break;
                    case 0b101:
                        immediateInstruction->opcode = LHU;
                        break;
                    case 0b000:
                        immediateInstruction->opcode = LB;
                        break;
                    case 0b100:
                        immediateInstruction->opcode = LBU;
                        break;
                }
            }

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

            saveInstruction->immediate = GET_S_IMMEDIATE(instructionValue);

            decoded.data = saveInstruction;
            break;
        case B:
            InstructionB* branchInstruction = malloc(sizeof(InstructionB));
            branchInstruction->rs1 = rs1;
            branchInstruction->rs2 = rs2;
            branchInstruction->opcode = Unknown;
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

            branchInstruction->immediate = GET_B_IMMEDIATE(instructionValue);
            
            decoded.data = branchInstruction;
            break;
        case U:
            InstructionU* upperInstruction = malloc(sizeof(InstructionU));
            upperInstruction->rd = rd;
            upperInstruction->immediate = GET_U_IMMEDIATE(instructionValue);
            // The only 2 U-type instructions (in this part of RISC-V at least)
            upperInstruction->opcode = (opcode == 0b0110111) ? LUI : AUIPC;
            
            decoded.data = upperInstruction;
            break;
        case J:
            InstructionJ* jumpInstruction = malloc(sizeof(InstructionJ));
            jumpInstruction->rd = rd;
            // It's the only J-type instruction (in this part of RISC-V at least)
            jumpInstruction->opcode = JAL;

            jumpInstruction->immediate = GET_J_IMMEDIATE(instructionValue);
            
            decoded.data = jumpInstruction;
            break;
    }

    return decoded;
}