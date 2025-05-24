#include "decoder.hpp"

#include <fmt/format.h>

#include <array>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "common.hpp"
#include "instruction.hpp"
#include "logger.hpp"

namespace sim {

const std::array<instr_t, kOpcodeNum> Decoder::m_mask{{
    [0b0000011] = 0x707f,  // LOAD
    // [0b0001111] = 0x707f,      // MISC-MEM
    [0b0010011] = 0x707f,      // OP-IMM
    [0b0011011] = 0x707f,      // OP-IMM-32
    [0b0010111] = 0x7f,        // AUIPC
    [0b0100011] = 0x707f,      // STORE
    [0b0110011] = 0xfe00707f,  // OP
    [0b0110111] = 0x7f,        // LUI
    [0b0111011] = 0xfe00707f,  // OP-32
    [0b1100011] = 0x707f,      // BRANCH
    [0b1100111] = 0x707f,      // JALR
    [0b1101111] = 0x7f,        // JAL
    [0b1110011] = 0x707f,      // SYSTEM
}};

enum class Decoder::Match : instr_t {  // MATCH
    // LUI
    LUI = 0x37,

    // AUIPC
    AUIPC = 0x17,

    // JAL
    JAL = 0x6f,

    // JALR
    JALR = 0x67,

    // BRANCH
    BEQ = 0x63,
    BNE = 0x1063,
    BLT = 0x4063,
    BGE = 0x5063,
    BLTU = 0x6063,
    BGEU = 0x7063,

    // LOAD
    LB = 0x3,
    LH = 0x1003,
    LW = 0x2003,
    LBU = 0x4003,
    LHU = 0x5003,
    LWU = 0x6003,
    LD = 0x3003,

    // STORE
    SB = 0x23,
    SH = 0x1023,
    SW = 0x2023,
    SD = 0x3023,

    // OP-IMM
    ADDI = 0x13,
    SLTI = 0x2013,
    SLTIU = 0x3013,
    ANDI = 0x7013,
    ORI = 0x6013,
    XORI = 0x4013,
    SLLI = 0x1013,
    SRI = 0x5013,  // combination of SRLI and SRAI
    // SRAI = 0x5013,

    // OP-IMM-32
    ADDIW = 0x1b,
    SLLIW = 0x101b,
    SRIW = 0x501b,  // combination of SRLIW and SRAIW
    // SRAIW = 0x4000501b,

    // OP
    ADD = 0x33,
    SLT = 0x2033,
    SLTU = 0x3033,
    AND = 0x7033,
    OR = 0x6033,
    XOR = 0x4033,
    SLL = 0x1033,
    SRL = 0x5033,
    SUB = 0x40000033,
    SRA = 0x40005033,

    // MISC-MEM
    // FENCE = 0xf,
    // FENCE_I = 0x100f,

    // SYSTEM
    ECALL = 0x73,
    // EBREAK = 0x100073,

    // OP-32
    ADDW = 0x3b,
    SUBW = 0x4000003b,
    SLLW = 0x103b,
    SRLW = 0x503b,
    SRAW = 0x4000503b,
};

void Decoder::decode_r_type(instr_t raw_instr, EncInstr &enc_instr) {
    enc_instr.rs2 = bits<24, 20>(raw_instr);
    enc_instr.rs1 = bits<19, 15>(raw_instr);
    enc_instr.rd = bits<11, 7>(raw_instr);
}

void Decoder::decode_i_type(instr_t raw_instr, EncInstr &enc_instr) {
    enc_instr.imm = sbits<31, 20>(raw_instr);
    enc_instr.rs1 = bits<19, 15>(raw_instr);
    enc_instr.rd = bits<11, 7>(raw_instr);
}

void Decoder::decode_s_type(instr_t raw_instr, EncInstr &enc_instr) {
    enc_instr.imm = (sbits<31, 25>(raw_instr) << 5) | (bits<11, 7>(raw_instr));
    enc_instr.rs2 = bits<24, 20>(raw_instr);
    enc_instr.rs1 = bits<19, 15>(raw_instr);
}

void Decoder::decode_b_type(instr_t raw_instr, EncInstr &enc_instr) {
    enc_instr.imm = sbits<12, 0>((bit<31>(raw_instr) << 12) | (bits<30, 25>(raw_instr) << 5) |
                                 (bits<11, 8>(raw_instr) << 1) | (bit<7>(raw_instr) << 11));
    enc_instr.rs2 = bits<24, 20>(raw_instr);
    enc_instr.rs1 = bits<19, 15>(raw_instr);
}

void Decoder::decode_u_type(instr_t raw_instr, EncInstr &enc_instr) {
    enc_instr.imm = sbits<31, 12>(raw_instr) << 12;
    enc_instr.rd = bits<11, 7>(raw_instr);
}

void Decoder::decode_j_type(instr_t raw_instr, EncInstr &enc_instr) {
    enc_instr.imm = sbits<20, 0>((bit<31>(raw_instr) << 20) | (bits<30, 21>(raw_instr) << 1) |
                                 (bit<20>(raw_instr) << 11) | (bits<19, 12>(raw_instr) << 12));
    enc_instr.rd = bits<11, 7>(raw_instr);
}

void Decoder::decode_instruction(instr_t raw_instr, EncInstr &enc_instr) try {
    auto opcode = bits<6, 0>(raw_instr);

    instr_t match = raw_instr & m_mask[opcode];
    auto match_id = static_cast<Match>(match);

    switch (match_id) {
        // R - type
        case Match::ADD: {
            enc_instr.id = InstrId::ADD;
            enc_instr.instr_t = InstrType::R;

            decode_r_type(raw_instr, enc_instr);
            break;
        }
        case Match::SUB: {
            enc_instr.id = InstrId::SUB;
            enc_instr.instr_t = InstrType::R;

            decode_r_type(raw_instr, enc_instr);
            break;
        }
        case Match::SLL: {
            enc_instr.id = InstrId::SLL;
            enc_instr.instr_t = InstrType::R;

            decode_r_type(raw_instr, enc_instr);
            break;
        }
        case Match::SLT: {
            enc_instr.id = InstrId::SLT;
            enc_instr.instr_t = InstrType::R;

            decode_r_type(raw_instr, enc_instr);
            break;
        }
        case Match::SLTU: {
            enc_instr.id = InstrId::SLTU;
            enc_instr.instr_t = InstrType::R;

            decode_r_type(raw_instr, enc_instr);
            break;
        }
        case Match::XOR: {
            enc_instr.id = InstrId::XOR;
            enc_instr.instr_t = InstrType::R;

            decode_r_type(raw_instr, enc_instr);
            break;
        }
        case Match::SRL: {
            enc_instr.id = InstrId::SRL;
            enc_instr.instr_t = InstrType::R;

            decode_r_type(raw_instr, enc_instr);
            break;
        }
        case Match::SRA: {
            enc_instr.id = InstrId::SRA;
            decode_r_type(raw_instr, enc_instr);
            break;
        }
        case Match::OR: {
            enc_instr.id = InstrId::OR;
            enc_instr.instr_t = InstrType::R;

            decode_r_type(raw_instr, enc_instr);
            break;
        }
        case Match::AND: {
            enc_instr.id = InstrId::AND;
            enc_instr.instr_t = InstrType::R;

            decode_r_type(raw_instr, enc_instr);
            break;
        }
        case Match::ADDW: {
            enc_instr.id = InstrId::ADDW;
            enc_instr.instr_t = InstrType::R;

            decode_r_type(raw_instr, enc_instr);
            break;
        }
        case Match::SLLW: {
            enc_instr.id = InstrId::SLLW;
            enc_instr.instr_t = InstrType::R;

            decode_r_type(raw_instr, enc_instr);
            break;
        }
        case Match::SRLW: {
            enc_instr.id = InstrId::SRLW;
            enc_instr.instr_t = InstrType::R;

            decode_r_type(raw_instr, enc_instr);
            break;
        }
        case Match::SUBW: {
            enc_instr.id = InstrId::SUBW;
            enc_instr.instr_t = InstrType::R;

            decode_r_type(raw_instr, enc_instr);
            break;
        }
        case Match::SRAW: {
            enc_instr.id = InstrId::SRAW;
            enc_instr.instr_t = InstrType::R;

            decode_r_type(raw_instr, enc_instr);
            break;
        }

        // I - type
        case Match::JALR: {
            enc_instr.id = InstrId::JALR;
            enc_instr.instr_t = InstrType::I;

            decode_i_type(raw_instr, enc_instr);
            break;
        }
        case Match::LB: {
            enc_instr.id = InstrId::LB;
            enc_instr.instr_t = InstrType::I;

            decode_i_type(raw_instr, enc_instr);
            break;
        }
        case Match::LH: {
            enc_instr.id = InstrId::LH;
            enc_instr.instr_t = InstrType::I;

            decode_i_type(raw_instr, enc_instr);
            break;
        }
        case Match::LW: {
            enc_instr.id = InstrId::LW;
            enc_instr.instr_t = InstrType::I;

            decode_i_type(raw_instr, enc_instr);
            break;
        }
        case Match::LBU: {
            enc_instr.id = InstrId::LBU;
            enc_instr.instr_t = InstrType::I;

            decode_i_type(raw_instr, enc_instr);
            break;
        }
        case Match::LHU: {
            enc_instr.id = InstrId::LHU;
            enc_instr.instr_t = InstrType::I;

            decode_i_type(raw_instr, enc_instr);
            break;
        }
        case Match::ADDI: {
            enc_instr.id = InstrId::ADDI;
            enc_instr.instr_t = InstrType::I;

            decode_i_type(raw_instr, enc_instr);
            break;
        }
        case Match::SLTI: {
            enc_instr.id = InstrId::SLTI;
            enc_instr.instr_t = InstrType::I;

            decode_i_type(raw_instr, enc_instr);
            break;
        }
        case Match::SLTIU: {
            enc_instr.id = InstrId::SLTIU;
            enc_instr.instr_t = InstrType::I;

            decode_i_type(raw_instr, enc_instr);
            break;
        }
        case Match::XORI: {
            enc_instr.id = InstrId::XORI;
            enc_instr.instr_t = InstrType::I;

            decode_i_type(raw_instr, enc_instr);
            break;
        }
        case Match::ORI: {
            enc_instr.id = InstrId::ORI;
            enc_instr.instr_t = InstrType::I;

            decode_i_type(raw_instr, enc_instr);
            break;
        }
        case Match::ANDI: {
            enc_instr.id = InstrId::ANDI;
            enc_instr.instr_t = InstrType::I;

            decode_i_type(raw_instr, enc_instr);
            break;
        }
        case Match::LWU: {
            enc_instr.id = InstrId::LWU;
            enc_instr.instr_t = InstrType::I;

            decode_i_type(raw_instr, enc_instr);
            break;
        }
        case Match::LD: {
            enc_instr.id = InstrId::LD;
            enc_instr.instr_t = InstrType::I;

            decode_i_type(raw_instr, enc_instr);
            break;
        }
        case Match::SLLI: {
            enc_instr.id = InstrId::SLLI;
            enc_instr.instr_t = InstrType::I;

            decode_i_type(raw_instr, enc_instr);
            break;
        }
        case Match::SRI: {
            enc_instr.instr_t = InstrType::I;

            decode_i_type(raw_instr, enc_instr);
            if (bit<10>(enc_instr.imm)) {
                enc_instr.id = InstrId::SRAI;
            } else {
                enc_instr.id = InstrId::SRLI;
            }
            break;
        }
        case Match::ADDIW: {
            enc_instr.id = InstrId::ADDIW;
            enc_instr.instr_t = InstrType::I;

            decode_i_type(raw_instr, enc_instr);
            break;
        }
        case Match::SLLIW: {
            enc_instr.id = InstrId::SLLIW;
            enc_instr.instr_t = InstrType::I;

            decode_i_type(raw_instr, enc_instr);
            break;
        }
        case Match::SRIW: {
            enc_instr.instr_t = InstrType::I;

            decode_i_type(raw_instr, enc_instr);
            if (bit<10>(enc_instr.imm)) {
                enc_instr.id = InstrId::SRAIW;
            } else {
                enc_instr.id = InstrId::SRLIW;
            }
            break;
        }

        // S - type
        case Match::SB: {
            enc_instr.id = InstrId::SB;
            enc_instr.instr_t = InstrType::S;

            decode_s_type(raw_instr, enc_instr);
            break;
        }
        case Match::SH: {
            enc_instr.id = InstrId::SH;
            enc_instr.instr_t = InstrType::S;

            decode_s_type(raw_instr, enc_instr);
            break;
        }
        case Match::SW: {
            enc_instr.id = InstrId::SW;
            enc_instr.instr_t = InstrType::S;

            decode_s_type(raw_instr, enc_instr);
            break;
        }
        case Match::SD: {
            enc_instr.id = InstrId::SD;
            enc_instr.instr_t = InstrType::S;

            decode_s_type(raw_instr, enc_instr);
            break;
        }

        // B - type
        case Match::BEQ: {
            enc_instr.id = InstrId::BEQ;
            enc_instr.instr_t = InstrType::B;

            decode_b_type(raw_instr, enc_instr);
            break;
        }
        case Match::BNE: {
            enc_instr.id = InstrId::BNE;
            enc_instr.instr_t = InstrType::B;

            decode_b_type(raw_instr, enc_instr);
            break;
        }
        case Match::BLT: {
            enc_instr.id = InstrId::BLT;
            enc_instr.instr_t = InstrType::B;

            decode_b_type(raw_instr, enc_instr);
            break;
        }
        case Match::BGE: {
            enc_instr.id = InstrId::BGE;
            enc_instr.instr_t = InstrType::B;

            decode_b_type(raw_instr, enc_instr);
            break;
        }
        case Match::BLTU: {
            enc_instr.id = InstrId::BLTU;
            enc_instr.instr_t = InstrType::B;

            decode_b_type(raw_instr, enc_instr);
            break;
        }
        case Match::BGEU: {
            enc_instr.id = InstrId::BGEU;
            enc_instr.instr_t = InstrType::B;

            decode_b_type(raw_instr, enc_instr);
            break;
        }

        // U - type
        case Match::LUI: {
            enc_instr.id = InstrId::LUI;
            enc_instr.instr_t = InstrType::U;

            decode_u_type(raw_instr, enc_instr);
            break;
        }
        case Match::AUIPC: {
            enc_instr.id = InstrId::AUIPC;
            enc_instr.instr_t = InstrType::U;

            decode_u_type(raw_instr, enc_instr);
            break;
        }

        // J - type
        case Match::JAL: {
            enc_instr.id = InstrId::JAL;
            enc_instr.instr_t = InstrType::J;

            decode_j_type(raw_instr, enc_instr);
            break;
        }
        case Match::ECALL: {
            enc_instr.id = InstrId::ECALL;
            break;
        }
            // case Match::EBREAK:
            // case Match::FENCE:
            // case Match::FENCE_I:
        default: {
            std::ostringstream oss{};
            oss << std::hex << std::to_string(match);
            throw std::runtime_error("Match with unknown instruction : " + oss.str());
        }
    }

    // Logger &myLogger = Logger::getInstance();
    // myLogger.message(Logger::severity_level::standard, "Decoder",
    //                  fmt::format("Match {} {:#08x}\n", InstrName[enc_instr.id], raw_instr));

} catch (const std::invalid_argument &e) {
    std::cerr << e.what() << std::endl;
}

}  // namespace sim
