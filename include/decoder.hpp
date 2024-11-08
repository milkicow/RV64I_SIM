#pragma once

#include <array>

#include "instruction.hpp"

namespace decoder {

constexpr size_t kOpcodeNum = 1 << 7;

template <size_t b>
uint64_t bit(uint64_t value) {
    static_assert(b < 64);
    return (value >> b) & 0b1;
}

template <size_t high_bit, size_t low_bit>
uint64_t bits(uint64_t value) {
    static_assert(high_bit < 64);
    static_assert(low_bit <= high_bit);

    return ((uint64_t(1) << (high_bit - low_bit + 1)) - 1) & (value >> low_bit);
}

template <size_t high_bit, size_t low_bit>
uint64_t sbits(uint64_t value) {
    static_assert(high_bit < 64);
    static_assert(low_bit <= high_bit);

    return static_cast<uint64_t>(static_cast<int64_t>(value) << (63 - high_bit) >>
                                 (63 - high_bit + low_bit));
}

template <size_t high_bit>
uint64_t sext(uint64_t value) {  // == sbits<high_bit , 0>
    static_assert(high_bit < 64);

    return static_cast<int64_t>(value) << (63 - high_bit) >> (63 - high_bit);
}

class Decoder final {
   private:
    static const std::array<instruction::instr_t, kOpcodeNum> m_mask;
    enum class Match : instruction::instr_t;

    static void decode_r_type(instruction::instr_t raw_instr, instruction::EncInstr &enc_instr);
    static void decode_i_type(instruction::instr_t raw_instr, instruction::EncInstr &enc_instr);
    static void decode_s_type(instruction::instr_t raw_instr, instruction::EncInstr &enc_instr);
    static void decode_b_type(instruction::instr_t raw_instr, instruction::EncInstr &enc_instr);
    static void decode_u_type(instruction::instr_t raw_instr, instruction::EncInstr &enc_instr);
    static void decode_j_type(instruction::instr_t raw_instr, instruction::EncInstr &enc_instr);

   public:
    static void decode_instruction(instruction::instr_t raw_instr,
                                   instruction::EncInstr &enc_instr);
};

}  // namespace decoder
