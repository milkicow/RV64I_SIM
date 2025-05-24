#pragma once

#include <stdint.h>

#include <sstream>
#include <string>
#include <string_view>

namespace sim {

using instr_t = uint32_t;

enum class InstrType : uint8_t { R, I, S, B, U, J };

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

static std::string reg_name(uint8_t reg) { return 'x' + std::to_string(reg); }

struct EncInstr {
    InstrId id;

    uint8_t rd = 0;
    uint8_t rs1 = 0;
    uint8_t rs2 = 0;

    uint64_t imm = 0;

    InstrType instr_t;

    std::string format() const {
        switch (instr_t) {
            case InstrType::R:
                return format_R();
            case InstrType::I:
                return format_I();
            case InstrType::B:
                return format_B();
            case InstrType::S:
                return format_S();
            case InstrType::U:
                return format_U();
            case InstrType::J:
                return format_J();
            default:
                return "";
        }
    }

    std::string format_R() const {
        std::ostringstream oss{};
        oss << InstrName[id] << ' ' << reg_name(rd) << ", " << reg_name(rs1) << ", "
            << reg_name(rs2);
        return oss.str();
    }

    std::string format_I() const {
        std::ostringstream oss{};
        oss << InstrName[id] << ' ' << reg_name(rd) << ", " << reg_name(rs1) << ", " << std::hex
            << imm;
        return oss.str();
    }

    std::string format_B() const {
        std::ostringstream oss{};
        oss << InstrName[id] << ' ' << reg_name(rs1) << ", " << reg_name(rs2) << ", " << std::hex
            << imm;
        return oss.str();
    }

    std::string format_S() const {
        std::ostringstream oss{};
        oss << InstrName[id] << ' ' << reg_name(rs2) << ", " << imm << '(' << reg_name(rs1) << ')';
        return oss.str();
    }

    std::string format_U() const {
        std::ostringstream oss{};
        oss << InstrName[id] << ' ' << reg_name(rd) << ", " << std::hex << (imm >> 12);
        return oss.str();
    }

    std::string format_J() const {
        std::ostringstream oss{};
        oss << InstrName[id] << ' ' << reg_name(rd) << ", " << std::hex << imm;
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
