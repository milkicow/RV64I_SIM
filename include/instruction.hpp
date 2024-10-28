#pragma once

#include <stdint.h>

namespace instruction {

using instr_t = uint32_t;

enum class InstrType : uint8_t { R, I, S, B, U, J };

enum class InstrId : instr_t {  // MATCH
    LUI = 0x37,
    AUIPC = 0x17,
    JAL = 0x6f,
    JALR = 0x67,
    BEQ = 0x63,
    BNE = 0x1063,
    BLT = 0x4063,
    BGE = 0x5063,
    BLTU = 0x6063,
    BGEU = 0x7063,
    LB = 0x3,
    LH = 0x1003,
    LW = 0x2003,
    LBU = 0x4003,
    LHU = 0x5003,
    SB = 0x23,
    SH = 0x1023,
    SW = 0x2023,
    ADDI = 0x13,
    SLTI = 0x2013,
    SLTIU = 0x3013,
    XORI = 0x4013,
    ORI = 0x6013,
    ANDI = 0x7013,
    ADD = 0x33,
    SUB = 0x40000033,
    SLL = 0x1033,
    SLT = 0x2033,
    SLTU = 0x3033,
    XOR = 0x4033,
    SRL = 0x5033,
    SRA = 0x40005033,
    OR = 0x6033,
    AND = 0x7033,
    FENCE = 0xf,
    FENCE_I = 0x100f,
    PAUSE = 0x100000f,  // hint instruction
    ECALL = 0x73,
    EBREAK = 0x100073,

    LWU = 0x6003,
    LD = 0x3003,
    SD = 0x3023,

    SLLI = 0x1013,
    SRLI = 0x5013,
    SRAI = 0x40005013,

    ADDIW = 0x1b,
    SLLIW = 0x101b,
    SRLIW = 0x501b,
    SRAIW = 0x4000501b,
    ADDW = 0x3b,
    SUBW = 0x4000003b,
    SLLW = 0x103b,
    SRLW = 0x503b,
    SRAW = 0x4000503b,

    UNKNOWN = 0xFFFFFFFF,
};

struct EncInstr final {
    InstrId id = InstrId::UNKNOWN;

    uint8_t rd = 0;
    uint8_t rs1 = 0;
    uint8_t rs2 = 0;

    uint32_t imm = 0;

    InstrType get_instruction_type();
};

}  // namespace instruction
