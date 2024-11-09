#pragma once

#include <stdint.h>

namespace instruction {

using instr_t = uint32_t;

// enum class InstrType : uint8_t { R, I, S, B, U, J };

enum class InstrId {
    // R - rype
    ADD,
    SUB,
    SLL,
    SLT,
    SLTU,
    XOR,
    SRL,
    SRA,
    OR,
    AND,
    ADDW,
    SLLW,
    SRLW,
    SUBW,
    SRAW,

    // I - type
    JALR,
    LB,
    LH,
    LW,
    LBU,
    LHU,
    ADDI,
    SLTI,
    SLTIU,
    XORI,
    ORI,
    ANDI,
    LWU,
    LD,
    SLLI,
    SRLI,
    SRAI,
    ADDIW,
    SLLIW,
    SRLIW,
    SRAIW,

    // S - type
    SB,
    SH,
    SW,
    SD,

    // B - type
    BEQ,
    BNE,
    BLT,
    BGE,
    BLTU,
    BGEU,

    // U - type
    LUI,
    AUIPC,

    // J - type
    JAL,
};

struct EncInstr {
    InstrId id;

    uint8_t rd = 0;
    uint8_t rs1 = 0;
    uint8_t rs2 = 0;

    uint32_t imm = 0;
};

}  // namespace instruction
