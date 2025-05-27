#include "executor.hpp"

#include <assert.h>
#include <fmt/format.h>

#include <iostream>
#include <sstream>

#include "basic_block.hpp"
#include "basic_block_cache.hpp"
#include "common.hpp"
#include "decoder.hpp"
#include "hart.hpp"
#include "logger.hpp"

namespace sim {

const std::array<Executor::executor_func_t, 51> Executor::functions{{
    [InstrId::ADD] = execute_add,
    [InstrId::SUB] = execute_sub,
    [InstrId::SLL] = execute_sll,
    [InstrId::SLT] = execute_slt,
    [InstrId::SLTU] = execute_sltu,
    [InstrId::XOR] = execute_xor,
    [InstrId::SRL] = execute_srl,
    [InstrId::SRA] = execute_sra,
    [InstrId::OR] = execute_or,
    [InstrId::AND] = execute_and,
    [InstrId::ADDW] = execute_addw,
    [InstrId::SLLW] = execute_sllw,
    [InstrId::SRLW] = execute_srlw,
    [InstrId::SUBW] = execute_subw,
    [InstrId::SRAW] = execute_sraw,

    // I - type
    [InstrId::JALR] = execute_jalr,
    [InstrId::LB] = execute_lb,
    [InstrId::LH] = execute_lh,
    [InstrId::LW] = execute_lw,
    [InstrId::LBU] = execute_lbu,
    [InstrId::LHU] = execute_lhu,
    [InstrId::ADDI] = execute_addi,
    [InstrId::SLTI] = execute_slti,
    [InstrId::SLTIU] = execute_sltiu,
    [InstrId::XORI] = execute_xori,
    [InstrId::ORI] = execute_ori,
    [InstrId::ANDI] = execute_andi,
    [InstrId::LWU] = execute_lwu,
    [InstrId::LD] = execute_ld,
    [InstrId::SLLI] = execute_slli,
    [InstrId::SRLI] = execute_srli,
    [InstrId::SRAI] = execute_srai,
    [InstrId::ADDIW] = execute_addiw,
    [InstrId::SLLIW] = execute_slliw,
    [InstrId::SRLIW] = execute_srliw,
    [InstrId::SRAIW] = execute_sraiw,

    // S - type
    [InstrId::SB] = execute_sb,
    [InstrId::SH] = execute_sh,
    [InstrId::SW] = execute_sw,
    [InstrId::SD] = execute_sd,

    // B - type
    [InstrId::BEQ] = execute_beq,
    [InstrId::BNE] = execute_bne,
    [InstrId::BLT] = execute_blt,
    [InstrId::BGE] = execute_bge,
    [InstrId::BLTU] = execute_bltu,
    [InstrId::BGEU] = execute_bgeu,

    // U - type
    [InstrId::LUI] = execute_lui,
    [InstrId::AUIPC] = execute_auipc,

    // J - type
    [InstrId::JAL] = execute_jal,

    [InstrId::FAKE_BB_END] = execute_fake_bb_end,

    [InstrId::ECALL] = execute_ecall,
}};

static void format_execute_instr(Hart &hart, const EncInstr *instruction) {
    Logger &myLogger = Logger::getInstance();

    auto str = fmt::format("[{:04x}]: {}", hart.get_pc(), instruction->format());
    myLogger.message(Logger::severity_level::trace, "", str);
}

// R - type
void Executor::execute_add(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];

    format_execute_instr(hart, &instr);

    hart.set_reg(instr.rd, hart.get_reg(instr.rs1) + hart.get_reg(instr.rs2));

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);

    ++instr_index;
    auto next_instr = instructions[instr_index];

    functions[next_instr.id](hart, instructions, instr_index);
}

void Executor::execute_sub(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];
    format_execute_instr(hart, &instr);

    hart.set_reg(instr.rd, hart.get_reg(instr.rs1) - hart.get_reg(instr.rs2));

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);

    ++instr_index;
    auto next_instr = instructions[instr_index];

    functions[next_instr.id](hart, instructions, instr_index);
}

void Executor::execute_slt(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];
    format_execute_instr(hart, &instr);

    hart.set_reg(instr.rd, (static_cast<signed_reg_t>(hart.get_reg(instr.rs1)) <
                            static_cast<signed_reg_t>(hart.get_reg(instr.rs2))));

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);

    ++instr_index;
    auto next_instr = instructions[instr_index];

    functions[next_instr.id](hart, instructions, instr_index);
}

void Executor::execute_sltu(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];
    format_execute_instr(hart, &instr);

    hart.set_reg(instr.rd, hart.get_reg(instr.rs1) < hart.get_reg(instr.rs2));

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);

    ++instr_index;
    auto next_instr = instructions[instr_index];

    functions[next_instr.id](hart, instructions, instr_index);
}

void Executor::execute_xor(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];
    format_execute_instr(hart, &instr);

    hart.set_reg(instr.rd, hart.get_reg(instr.rs1) ^ hart.get_reg(instr.rs2));

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);

    ++instr_index;
    auto next_instr = instructions[instr_index];

    functions[next_instr.id](hart, instructions, instr_index);
}

void Executor::execute_sll(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];
    format_execute_instr(hart, &instr);

    hart.set_reg(instr.rd, hart.get_reg(instr.rs1) << bits<5, 0>(hart.get_reg(instr.rs2)));

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);

    ++instr_index;
    auto next_instr = instructions[instr_index];

    functions[next_instr.id](hart, instructions, instr_index);
}

void Executor::execute_srl(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];
    format_execute_instr(hart, &instr);

    hart.set_reg(instr.rd, hart.get_reg(instr.rs1) >> bits<5, 0>(hart.get_reg(instr.rs2)));

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);

    ++instr_index;
    auto next_instr = instructions[instr_index];

    functions[next_instr.id](hart, instructions, instr_index);
}

void Executor::execute_sra(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];
    format_execute_instr(hart, &instr);

    hart.set_reg(instr.rd, static_cast<reg_t>(static_cast<signed_reg_t>(hart.get_reg(instr.rs1)) >>
                                              bits<5, 0>(hart.get_reg(instr.rs2))));

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);

    ++instr_index;
    auto next_instr = instructions[instr_index];

    functions[next_instr.id](hart, instructions, instr_index);
}

void Executor::execute_or(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];
    format_execute_instr(hart, &instr);

    hart.set_reg(instr.rd, hart.get_reg(instr.rs1) | hart.get_reg(instr.rs2));

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);

    ++instr_index;
    auto next_instr = instructions[instr_index];

    functions[next_instr.id](hart, instructions, instr_index);
}

void Executor::execute_and(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];
    format_execute_instr(hart, &instr);

    hart.set_reg(instr.rd, hart.get_reg(instr.rs1) & hart.get_reg(instr.rs2));

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);

    ++instr_index;
    auto next_instr = instructions[instr_index];

    functions[next_instr.id](hart, instructions, instr_index);
}

void Executor::execute_addw(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];
    format_execute_instr(hart, &instr);

    hart.set_reg(instr.rd, sext<31>(hart.get_reg(instr.rs1) + hart.get_reg(instr.rs2)));

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);

    ++instr_index;
    auto next_instr = instructions[instr_index];

    functions[next_instr.id](hart, instructions, instr_index);
}

void Executor::execute_sllw(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];
    format_execute_instr(hart, &instr);

    hart.set_reg(instr.rd,
                 sext<31>(hart.get_reg(instr.rs1) << bits<4, 0>(hart.get_reg(instr.rs2))));

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);

    ++instr_index;
    auto next_instr = instructions[instr_index];

    functions[next_instr.id](hart, instructions, instr_index);
}

void Executor::execute_srlw(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];
    format_execute_instr(hart, &instr);

    hart.set_reg(instr.rd, sext<31>(bits<31, 0>(hart.get_reg(instr.rs1)) >>
                                    bits<4, 0>(hart.get_reg(instr.rs2))));

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);

    ++instr_index;
    auto next_instr = instructions[instr_index];

    functions[next_instr.id](hart, instructions, instr_index);
}

void Executor::execute_subw(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];
    format_execute_instr(hart, &instr);

    hart.set_reg(instr.rd, sext<31>(hart.get_reg(instr.rs1) - hart.get_reg(instr.rs2)));

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);

    ++instr_index;
    auto next_instr = instructions[instr_index];

    functions[next_instr.id](hart, instructions, instr_index);
}

void Executor::execute_sraw(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];
    format_execute_instr(hart, &instr);

    hart.set_reg(instr.rd,
                 sext<31>(static_cast<reg_t>(static_cast<signed_reg_t>(hart.get_reg(instr.rs1)) >>
                                             bits<4, 0>(hart.get_reg(instr.rs2)))));

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);

    ++instr_index;
    auto next_instr = instructions[instr_index];

    functions[next_instr.id](hart, instructions, instr_index);
}

// I - type
void Executor::execute_jalr(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];
    format_execute_instr(hart, &instr);

    hart.set_reg(instr.rd, hart.get_pc_next());
    hart.set_next_pc((hart.get_reg(instr.rs1) + instr.imm) & ~uint64_t(1));

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);
}

void Executor::execute_ld(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];
    uint64_t value;

    format_execute_instr(hart, &instr);

    hart.load<uint64_t>(hart.get_reg(instr.rs1) + instr.imm, value);
    hart.set_reg(instr.rd, value);

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);

    ++instr_index;
    auto next_instr = instructions[instr_index];

    functions[next_instr.id](hart, instructions, instr_index);
}

void Executor::execute_lb(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];
    uint64_t value;

    format_execute_instr(hart, &instr);

    hart.load<uint8_t>(hart.get_reg(instr.rs1) + instr.imm, value);
    hart.set_reg(instr.rd, sext<7>(value));

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);

    ++instr_index;
    auto next_instr = instructions[instr_index];

    functions[next_instr.id](hart, instructions, instr_index);
}

void Executor::execute_lbu(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];
    uint64_t value;

    format_execute_instr(hart, &instr);

    hart.load<uint8_t>(hart.get_reg(instr.rs1) + instr.imm, value);
    hart.set_reg(instr.rd, value);

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);

    ++instr_index;
    auto next_instr = instructions[instr_index];

    functions[next_instr.id](hart, instructions, instr_index);
}

void Executor::execute_lh(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];
    uint64_t value;

    format_execute_instr(hart, &instr);

    hart.load<uint16_t>(hart.get_reg(instr.rs1) + instr.imm, value);
    hart.set_reg(instr.rd, sext<15>(value));

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);

    ++instr_index;
    auto next_instr = instructions[instr_index];

    functions[next_instr.id](hart, instructions, instr_index);
}

void Executor::execute_lhu(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];
    uint64_t value;

    format_execute_instr(hart, &instr);

    hart.load<uint16_t>(hart.get_reg(instr.rs1) + instr.imm, value);
    hart.set_reg(instr.rd, value);

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);

    ++instr_index;
    auto next_instr = instructions[instr_index];

    functions[next_instr.id](hart, instructions, instr_index);
}

void Executor::execute_lw(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];
    uint64_t value;

    format_execute_instr(hart, &instr);

    hart.load<uint32_t>(hart.get_reg(instr.rs1) + instr.imm, value);
    hart.set_reg(instr.rd, sext<31>(value));

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);

    ++instr_index;
    auto next_instr = instructions[instr_index];

    functions[next_instr.id](hart, instructions, instr_index);
}

void Executor::execute_lwu(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];
    uint64_t value;

    format_execute_instr(hart, &instr);

    hart.load<uint32_t>(hart.get_reg(instr.rs1) + instr.imm, value);
    hart.set_reg(instr.rd, value);

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);

    ++instr_index;
    auto next_instr = instructions[instr_index];

    functions[next_instr.id](hart, instructions, instr_index);
}

void Executor::execute_addi(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];

    format_execute_instr(hart, &instr);

    hart.set_reg(instr.rd, hart.get_reg(instr.rs1) + static_cast<signed_reg_t>(instr.imm));

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);

    ++instr_index;
    auto next_instr = instructions[instr_index];

    functions[next_instr.id](hart, instructions, instr_index);
}

void Executor::execute_slti(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];

    format_execute_instr(hart, &instr);

    hart.set_reg(instr.rd, static_cast<signed_reg_t>(hart.get_reg(instr.rs1)) <
                               static_cast<signed_reg_t>(instr.imm));

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);

    ++instr_index;
    auto next_instr = instructions[instr_index];

    functions[next_instr.id](hart, instructions, instr_index);
}

void Executor::execute_sltiu(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];

    format_execute_instr(hart, &instr);

    hart.set_reg(instr.rd, hart.get_reg(instr.rs1) < instr.imm);

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);

    ++instr_index;
    auto next_instr = instructions[instr_index];

    functions[next_instr.id](hart, instructions, instr_index);
}

void Executor::execute_andi(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];

    format_execute_instr(hart, &instr);

    hart.set_reg(instr.rd, hart.get_reg(instr.rs1) & instr.imm);

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);

    ++instr_index;
    auto next_instr = instructions[instr_index];

    functions[next_instr.id](hart, instructions, instr_index);
}

void Executor::execute_ori(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];

    format_execute_instr(hart, &instr);

    hart.set_reg(instr.rd, hart.get_reg(instr.rs1) | instr.imm);

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);

    ++instr_index;
    auto next_instr = instructions[instr_index];

    functions[next_instr.id](hart, instructions, instr_index);
}

void Executor::execute_xori(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];

    format_execute_instr(hart, &instr);

    hart.set_reg(instr.rd, hart.get_reg(instr.rs1) ^ instr.imm);

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);

    ++instr_index;
    auto next_instr = instructions[instr_index];

    functions[next_instr.id](hart, instructions, instr_index);
}

void Executor::execute_slli(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];

    format_execute_instr(hart, &instr);

    hart.set_reg(instr.rd, hart.get_reg(instr.rs1) << bits<5, 0>(instr.imm));

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);

    ++instr_index;
    auto next_instr = instructions[instr_index];

    functions[next_instr.id](hart, instructions, instr_index);
}

void Executor::execute_srli(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];

    format_execute_instr(hart, &instr);

    hart.set_reg(instr.rd, hart.get_reg(instr.rs1) >> bits<5, 0>(instr.imm));

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);

    ++instr_index;
    auto next_instr = instructions[instr_index];

    functions[next_instr.id](hart, instructions, instr_index);
}

void Executor::execute_srai(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];

    format_execute_instr(hart, &instr);

    hart.set_reg(instr.rd, static_cast<reg_t>(static_cast<signed_reg_t>(hart.get_reg(instr.rs1)) >>
                                              bits<5, 0>(instr.imm)));

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);

    ++instr_index;
    auto next_instr = instructions[instr_index];

    functions[next_instr.id](hart, instructions, instr_index);
}

void Executor::execute_addiw(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];

    format_execute_instr(hart, &instr);

    hart.set_reg(instr.rd, sext<31>(hart.get_reg(instr.rs1) + instr.imm));

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);

    ++instr_index;
    auto next_instr = instructions[instr_index];

    functions[next_instr.id](hart, instructions, instr_index);
}

void Executor::execute_slliw(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];

    format_execute_instr(hart, &instr);

    hart.set_reg(instr.rd, sext<31>(hart.get_reg(instr.rs1) << bits<4, 0>(instr.imm)));

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);

    ++instr_index;
    auto next_instr = instructions[instr_index];

    functions[next_instr.id](hart, instructions, instr_index);
}

void Executor::execute_srliw(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];

    format_execute_instr(hart, &instr);

    hart.set_reg(instr.rd, sext<31>(bits<31, 0>(hart.get_reg(instr.rs1)) >> bits<4, 0>(instr.imm)));

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);

    ++instr_index;
    auto next_instr = instructions[instr_index];

    functions[next_instr.id](hart, instructions, instr_index);
}

void Executor::execute_sraiw(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];

    format_execute_instr(hart, &instr);

    hart.set_reg(instr.rd,
                 sext<31>(static_cast<reg_t>(static_cast<signed_reg_t>(hart.get_reg(instr.rs1)) >>
                                             bits<4, 0>(instr.imm))));

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);

    ++instr_index;
    auto next_instr = instructions[instr_index];

    functions[next_instr.id](hart, instructions, instr_index);
}

void Executor::execute_ecall(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    hart.set_pc(0);
    // hart.set_next_pc(hart.get_pc_next() + 4);
}
// void Executor::execute_ebreak(Hart &hart, const EncInstr *instructions, size_t instr_index) {}
// void Executor::execute_fence(Hart &hart, const EncInstr *instructions, size_t instr_index) {}
// void Executor::execute_fence_i(Hart &hart, const EncInstr *instructions, size_t instr_index) {}

// S - type
void Executor::execute_sb(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];
    hart.store<uint8_t>(hart.get_reg(instr.rs1) + instr.imm, hart.get_reg(instr.rs2));

    format_execute_instr(hart, &instr);

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);

    ++instr_index;
    auto next_instr = instructions[instr_index];

    functions[next_instr.id](hart, instructions, instr_index);
}

void Executor::execute_sh(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];
    hart.store<uint16_t>(hart.get_reg(instr.rs1) + instr.imm, hart.get_reg(instr.rs2));

    format_execute_instr(hart, &instr);

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);

    ++instr_index;
    auto next_instr = instructions[instr_index];

    functions[next_instr.id](hart, instructions, instr_index);
}

void Executor::execute_sw(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];
    format_execute_instr(hart, &instr);

    hart.store<uint32_t>(hart.get_reg(instr.rs1) + instr.imm, hart.get_reg(instr.rs2));

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);

    ++instr_index;
    auto next_instr = instructions[instr_index];

    functions[next_instr.id](hart, instructions, instr_index);
}

void Executor::execute_sd(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];

    format_execute_instr(hart, &instr);

    hart.store<uint64_t>(hart.get_reg(instr.rs1) + instr.imm, hart.get_reg(instr.rs2));

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);

    ++instr_index;
    auto next_instr = instructions[instr_index];

    functions[next_instr.id](hart, instructions, instr_index);
}

// B - type
void Executor::execute_beq(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];
    if (hart.get_reg(instr.rs1) == hart.get_reg(instr.rs2)) {
        hart.set_next_pc(hart.get_pc() + instr.imm);
    }
    format_execute_instr(hart, &instr);

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);
}

void Executor::execute_bne(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];
    if (hart.get_reg(instr.rs1) != hart.get_reg(instr.rs2)) {
        hart.set_next_pc(hart.get_pc() + instr.imm);
    }
    format_execute_instr(hart, &instr);

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);
}

void Executor::execute_blt(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];
    if (static_cast<signed_reg_t>(hart.get_reg(instr.rs1)) <
        static_cast<signed_reg_t>(hart.get_reg(instr.rs2))) {
        hart.set_next_pc(hart.get_pc() + instr.imm);
    }
    format_execute_instr(hart, &instr);

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);
}

void Executor::execute_bltu(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];
    if (hart.get_reg(instr.rs1) < hart.get_reg(instr.rs2)) {
        hart.set_next_pc(hart.get_pc() + instr.imm);
    }
    format_execute_instr(hart, &instr);

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);
}

void Executor::execute_bge(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];
    if (static_cast<signed_reg_t>(hart.get_reg(instr.rs1)) >=
        static_cast<signed_reg_t>(hart.get_reg(instr.rs2))) {
        hart.set_next_pc(hart.get_pc() + instr.imm);
    }
    format_execute_instr(hart, &instr);

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);
}

void Executor::execute_bgeu(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];
    if (hart.get_reg(instr.rs1) >= hart.get_reg(instr.rs2)) {
        hart.set_next_pc(hart.get_pc() + instr.imm);
    }
    format_execute_instr(hart, &instr);

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);
}

// U - type
void Executor::execute_lui(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];
    format_execute_instr(hart, &instr);

    hart.set_reg(instr.rd, instr.imm);

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);

    ++instr_index;
    auto next_instr = instructions[instr_index];

    functions[next_instr.id](hart, instructions, instr_index);
}

void Executor::execute_auipc(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];

    format_execute_instr(hart, &instr);

    hart.set_reg(instr.rd, hart.get_pc() + instr.imm);

    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);

    ++instr_index;
    auto next_instr = instructions[instr_index];

    functions[next_instr.id](hart, instructions, instr_index);
}

// J - type
void Executor::execute_jal(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    auto instr = instructions[instr_index];

    format_execute_instr(hart, &instr);

    hart.set_reg(instr.rd, hart.get_pc_next());

    auto next_pc = hart.get_pc() + instr.imm;
    hart.set_pc(next_pc);
    hart.set_next_pc(next_pc + 4);
}

void Executor::execute_fake_bb_end(Hart &hart, const EncInstr *instructions, size_t instr_index) {
    hart.set_pc(hart.get_pc_next());
    hart.set_next_pc(hart.get_pc_next() + 4);
}

bool Executor::execute_BB(Hart &hart, BasicBlock &bb) {
    // Logger &myLogger = Logger::getInstance();

    // myLogger.message(Logger::severity_level::standard, "Executor", "start BB execution");

    auto *bb_instructions = bb.getInstructions();
    auto bb_size = bb.getSize();

    // myLogger.message(Logger::severity_level::standard, "Executor", fmt::format("bb size: {:d}",
    // bb_size));

    auto &instruction = bb_instructions[0];
    functions[instruction.id](hart, bb_instructions, 0);  // start execute instructions in bb

    // myLogger.message(Logger::severity_level::standard, "Executor", "end BB execution");
    return true;
}

bool Executor::run(Hart &hart, size_t &instr_counter) {
    // Logger &myLogger = Logger::getInstance();

    // myLogger.message(Logger::severity_level::standard, "Executor",
    //                  fmt::format("pc: {:#x} pc_next: {:#x}", hart.get_pc(), hart.get_pc_next()));
    // myLogger.message(Logger::severity_level::verbose, "Executor", hart.format_registers());

    BasicBlockCache bb_cache{};

    while (true) {
        // myLogger.message(Logger::severity_level::verbose, "Executor",
        //                  fmt::format("New bb iteration begin, pc: {:#x}", hart.get_pc()));

        auto addr = hart.get_pc();

        if (addr == 0) {
            break;
        }

        BasicBlock &bb = bb_cache.find(addr);

        instr_counter += bb.getSize();
        bool bb_in_cache = (addr == bb.getVirtualAddress());

        // myLogger.message(Logger::severity_level::standard, "Executor",
        //                  fmt::format("BB in cache: {}", bb_in_cache));

        if (!bb_in_cache) {
            // myLogger.message(Logger::severity_level::verbose, "Executor",
            //                  fmt::format("add bb {:#x} in cache", hart.get_pc()));
            bool bb_update_status = bb.update(hart);
            assert(bb_update_status);
        }

        // myLogger.message(
        //     Logger::severity_level::verbose, "Executor",
        //     fmt::format("bb start address after update: {:#x}", bb.getVirtualAddress()));

        Executor::execute_BB(hart, bb);
    }

    return true;
}

}  // namespace sim
