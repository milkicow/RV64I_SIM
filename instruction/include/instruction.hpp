#pragma once

#include <stdint.h>

#include <sstream>
#include <string>
#include <string_view>

namespace sim {

using instr_t = uint32_t;

// enum class InstrType : uint8_t { R, I, S, B, U, J };

enum InstrId {
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

    ECALL,

    FAKE_BB_END,
    NO_ID,
};

constexpr std::array<std::string_view, 52> InstrName{{
    // R - rype
    "ADD",
    "SUB",
    "SLL",
    "SLT",
    "SLTU",
    "XOR",
    "SRL",
    "SRA",
    "OR",
    "AND",
    "ADDW",
    "SLLW",
    "SRLW",
    "SUBW",
    "SRAW",

    // I - type
    "JALR",
    "LB",
    "LH",
    "LW",
    "LBU",
    "LHU",
    "ADDI",
    "SLTI",
    "SLTIU",
    "XORI",
    "ORI",
    "ANDI",
    "LWU",
    "LD",
    "SLLI",
    "SRLI",
    "SRAI",
    "ADDIW",
    "SLLIW",
    "SRLIW",
    "SRAIW",

    // S - type
    "SB",
    "SH",
    "SW",
    "SD",

    // B - type
    "BEQ",
    "BNE",
    "BLT",
    "BGE",
    "BLTU",
    "BGEU",

    // U - type
    "LUI",
    "AUIPC",

    // J - type
    "JAL",

    "ECALL",

    "FAKE_BB_END",
    "NO_ID",
}};

struct EncInstr {
    InstrId id;

    uint8_t rd = 0;
    uint8_t rs1 = 0;
    uint8_t rs2 = 0;

    uint64_t imm = 0;

    std::string format() {
        std::ostringstream oss{};
        oss << "Instruction: rd: " << +rd << " rs1: " << +rs1 << " rs2: " << +rs2 << " imm: " << imm
            << " (" << static_cast<int64_t>(imm) << ")";
        return oss.str();
    }

    void flush() {
        id = NO_ID;

        rd = 0;
        rs1 = 0;
        rs2 = 0;

        imm = 0;
    }

    bool is_terminal() {
        switch (id) {
            case JAL:
            case JALR:
            case BEQ:
            case BNE:
            case BLT:
            case BGE:
            case BLTU:
            case BGEU:
                return true;
            default:
                return false;
        }
    }
};

}  // namespace sim
