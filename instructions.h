#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

typedef enum Opcode {
    Invalid,
    ADDI,
    SLTI,
    SLTIU,
    ANDI,
    ORI,
    XORI,
    SLLI,
    SRLI,
    SRAI,
    ADD,
    SLT,
    SLTU,
    AND,
    OR,
    XOR,
    SLL,
    SRL,
    SUB,
    SRA,
    NOP,
    JAL,
    JALR,
    BEQ,
    BNE,
    BLT,
    BLTU,
    BGE,
    BGEU,
    LW,
    LH,
    LHU,
    LB,
    LBU,
    SW,
    SH,
    SB,
    ECALL,
    EBREAK,
    LUI,
    AUIPC
} Opcode;

typedef enum InstructionType {
    Unknown,
    Environment,
    R,
    I,
    S,
    B,
    U,
    J,
} InstructionType;

typedef struct InstructionR {
    Opcode opcode;
    unsigned char rs1;
    unsigned char rs2;
    unsigned char rd;
    // funct7
    // funct3
} InstructionR;

typedef struct InstructionI {
    Opcode opcode;
    int immediate;
    unsigned char rs1;
    unsigned char rd;
    // funct3
} InstructionI;

typedef struct InstructionS {
    Opcode opcode;
    unsigned char rs1;
    unsigned char rs2;
    int immediate;
    // funct3
} InstructionS;

// B is just a variant of S, where the bits are located a little bit differently
// It has all the same data
typedef InstructionS InstructionB;

typedef struct InstructionU {
    Opcode opcode;
    int immediate;
    unsigned char rd;
    // funct3
} InstructionU;

// J is just a variant of U, where the bits are located a little bit differently
// It has all the same data
typedef InstructionU InstructionJ;

typedef struct InstructionEnvironment {
    Opcode opcode;
} InstructionEnvironment;

typedef struct InstructionUnknown {
    int instruction;
} InstructionUnknown;

typedef struct InstructionData {
    InstructionType type;
    void* data;
} InstructionData;

#endif