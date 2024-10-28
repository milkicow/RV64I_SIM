#include "decoder.hpp"

#include <array>
#include <iostream>
#include <stdexcept>

#include "instruction.hpp"

namespace decoder {

const std::array<Decoder::mask_t, g_opcode_num> Decoder::m_masks{{
    [0b0000011] = 0x707f,      // LOAD
    [0b0001111] = 0x707f,      // MISC-MEM
    [0b0010011] = 0xfc00707f,  // OP-IMM
    [0b0011011] = 0xfe00707f,  // OP-IMM-32
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

void Decoder::decode_r_type(instruction::instr_t raw_instr, instruction::EncInstr &enc_instr) {
    enc_instr.rs2 = bits<24, 20>(raw_instr);
    enc_instr.rs1 = bits<19, 15>(raw_instr);
    enc_instr.rd = bits<11, 7>(raw_instr);
}

void Decoder::decode_i_type(instruction::instr_t raw_instr, instruction::EncInstr &enc_instr) {
    enc_instr.imm = sbits<31, 20>(raw_instr);
    enc_instr.rs1 = bits<19, 15>(raw_instr);
    enc_instr.rd = bits<11, 7>(raw_instr);
}

void Decoder::decode_s_type(instruction::instr_t raw_instr, instruction::EncInstr &enc_instr) {
    enc_instr.imm = (sbits<31, 25>(raw_instr) << 5) | (bits<11, 7>(raw_instr));
    enc_instr.rs2 = bits<24, 20>(raw_instr);
    enc_instr.rs1 = bits<19, 15>(raw_instr);
}

void Decoder::decode_b_type(instruction::instr_t raw_instr, instruction::EncInstr &enc_instr) {
    enc_instr.imm = sbits<12, 0>((bit<31>(raw_instr) << 12) | (bit<31>(raw_instr) << 5) |
                                 (bits<11, 8>(raw_instr) << 1) | (bit<7>(raw_instr) << 11));
    enc_instr.rs2 = bits<24, 20>(raw_instr);
    enc_instr.rs1 = bits<19, 15>(raw_instr);
}

void Decoder::decode_u_type(instruction::instr_t raw_instr, instruction::EncInstr &enc_instr) {
    enc_instr.imm = sbits<31, 12>(raw_instr) << 12;
    enc_instr.rd = bits<11, 7>(raw_instr);
}

void Decoder::decode_j_type(instruction::instr_t raw_instr, instruction::EncInstr &enc_instr) {
    enc_instr.imm = sbits<20, 0>((bit<31>(raw_instr) << 20) | (bits<30, 21>(raw_instr) << 1) |
                                 (bit<20>(raw_instr) << 11) | (bits<19, 12>(raw_instr) << 12));
    enc_instr.rd = bits<11, 7>(raw_instr);
}

void Decoder::decode_instruction(instruction::instr_t raw_instr,
                                 instruction::EncInstr &enc_instr) try {
    auto opcode = bits<6, 0>(raw_instr);
    uint32_t match = raw_instr & m_masks[opcode];

    enc_instr.id = static_cast<instruction::InstrId>(match);
    auto instr_type = enc_instr.get_instruction_type();
    switch (instr_type) {
        case instruction::InstrType::R: {
            decode_r_type(raw_instr, enc_instr);
            break;
        }
        case instruction::InstrType::I: {
            decode_i_type(raw_instr, enc_instr);
            break;
        }
        case instruction::InstrType::S: {
            decode_s_type(raw_instr, enc_instr);
            break;
        }
        case instruction::InstrType::B: {
            decode_b_type(raw_instr, enc_instr);
            break;
        }
        case instruction::InstrType::U: {
            decode_u_type(raw_instr, enc_instr);
            break;
        }
        case instruction::InstrType::J: {
            decode_j_type(raw_instr, enc_instr);
            break;
        }
        default: {
            throw std::invalid_argument("Recieved unknown instruction type: " +
                                        std::to_string(static_cast<uint8_t>(instr_type)));
        }
    }
} catch (const std::invalid_argument &e) {
    std::cerr << e.what() << std::endl;
}

}  // namespace decoder
