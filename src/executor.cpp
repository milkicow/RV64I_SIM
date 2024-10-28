#include "executor.hpp"

#include "decoder.hpp"
#include "hart.hpp"

namespace executor {

// R - type
void Executor::execute_add(hart::Hart &hart, const instruction::EncInstr &instr) {
    hart.set_reg(instr.rd, hart.get_reg(instr.rs1) + hart.get_reg(instr.rs2));
}

void Executor::execute_sub(hart::Hart &hart, const instruction::EncInstr &instr) {
    hart.set_reg(instr.rd, hart.get_reg(instr.rs1) - hart.get_reg(instr.rs2));
}

void Executor::execute_slt(hart::Hart &hart, const instruction::EncInstr &instr) {
    hart.set_reg(instr.rd, (static_cast<hart::signed_reg_t>(hart.get_reg(instr.rs1)) <
                            static_cast<hart::signed_reg_t>(hart.get_reg(instr.rs2))));
}

void Executor::execute_sltu(hart::Hart &hart, const instruction::EncInstr &instr) {
    hart.set_reg(instr.rd, hart.get_reg(instr.rs1) < hart.get_reg(instr.rs2));
}

void Executor::execute_xor(hart::Hart &hart, const instruction::EncInstr &instr) {
    hart.set_reg(instr.rd, hart.get_reg(instr.rs1) ^ hart.get_reg(instr.rs2));
}

void Executor::execute_sll(hart::Hart &hart, const instruction::EncInstr &instr) {
    hart.set_reg(instr.rd, hart.get_reg(instr.rs1) << decoder::bits<5, 0>(hart.get_reg(instr.rs2)));
}

void Executor::execute_srl(hart::Hart &hart, const instruction::EncInstr &instr) {
    hart.set_reg(instr.rd, hart.get_reg(instr.rs1) >> decoder::bits<5, 0>(hart.get_reg(instr.rs2)));
}

void Executor::execute_sra(hart::Hart &hart, const instruction::EncInstr &instr) {
    hart.set_reg(instr.rd, static_cast<hart::reg_t>(
                               static_cast<hart::signed_reg_t>(hart.get_reg(instr.rs1)) >>
                               decoder::bits<5, 0>(hart.get_reg(instr.rs2))));
}

void Executor::execute_or(hart::Hart &hart, const instruction::EncInstr &instr) {
    hart.set_reg(instr.rd, hart.get_reg(instr.rs1) | hart.get_reg(instr.rs2));
}

void Executor::execute_and(hart::Hart &hart, const instruction::EncInstr &instr) {
    hart.set_reg(instr.rd, hart.get_reg(instr.rs1) & hart.get_reg(instr.rs2));
}

void Executor::execute_addw(hart::Hart &hart, const instruction::EncInstr &instr) {
    hart.set_reg(instr.rd, decoder::sext<31>(hart.get_reg(instr.rs1) + hart.get_reg(instr.rs2)));
}

void Executor::execute_sllw(hart::Hart &hart, const instruction::EncInstr &instr) {
    hart.set_reg(instr.rd, decoder::sext<31>(hart.get_reg(instr.rs1)
                                             << decoder::bits<4, 0>(hart.get_reg(instr.rs2))));
}

void Executor::execute_srlw(hart::Hart &hart, const instruction::EncInstr &instr) {
    hart.set_reg(instr.rd, decoder::sext<31>(decoder::bits<31, 0>(hart.get_reg(instr.rs1)) >>
                                             decoder::bits<4, 0>(hart.get_reg(instr.rs2))));
}

void Executor::execute_subw(hart::Hart &hart, const instruction::EncInstr &instr) {
    hart.set_reg(instr.rd, decoder::sext<31>(hart.get_reg(instr.rs1) - hart.get_reg(instr.rs2)));
}

void Executor::execute_sraw(hart::Hart &hart, const instruction::EncInstr &instr) {
    hart.set_reg(instr.rd, decoder::sext<31>(static_cast<hart::reg_t>(
                               static_cast<hart::signed_reg_t>(hart.get_reg(instr.rs1)) >>
                               decoder::bits<4, 0>(hart.get_reg(instr.rs2)))));
}

// I - type
void Executor::execute_jalr(hart::Hart &hart, const instruction::EncInstr &instr) {
    hart.set_reg(instr.rd, hart.get_pc_next());
    hart.set_next_pc((hart.get_reg(instr.rs1) + instr.imm) & ~uint64_t(1));
}

void Executor::execute_ld(hart::Hart &hart, const instruction::EncInstr &instr) {
    uint64_t value;
    hart.load<uint64_t>(hart.get_reg(instr.rs1) + instr.imm, value);
    hart.set_reg(instr.rd, value);
}

void Executor::execute_lb(hart::Hart &hart, const instruction::EncInstr &instr) {  // CHECK
    uint64_t value;
    hart.load<uint8_t>(hart.get_reg(instr.rs1) + instr.imm, value);
    hart.set_reg(instr.rd, decoder::sext<7>(value));
}

void Executor::execute_lbu(hart::Hart &hart, const instruction::EncInstr &instr) {
    uint64_t value;
    hart.load<uint8_t>(hart.get_reg(instr.rs1) + instr.imm, value);
    hart.set_reg(instr.rd, value);
}

void Executor::execute_lh(hart::Hart &hart, const instruction::EncInstr &instr) {
    uint64_t value;
    hart.load<uint16_t>(hart.get_reg(instr.rs1) + instr.imm, value);
    hart.set_reg(instr.rd, decoder::sext<15>(value));
}

void Executor::execute_lhu(hart::Hart &hart, const instruction::EncInstr &instr) {
    uint64_t value;
    hart.load<uint16_t>(hart.get_reg(instr.rs1) + instr.imm, value);
    hart.set_reg(instr.rd, value);
}

void Executor::execute_lw(hart::Hart &hart, const instruction::EncInstr &instr) {
    uint64_t value;
    hart.load<uint32_t>(hart.get_reg(instr.rs1) + instr.imm, value);
    hart.set_reg(instr.rd, decoder::sext<31>(value));
}

void Executor::execute_lwu(hart::Hart &hart, const instruction::EncInstr &instr) {
    uint64_t value;
    hart.load<uint32_t>(hart.get_reg(instr.rs1) + instr.imm, value);
    hart.set_reg(instr.rd, value);
}

void Executor::execute_addi(hart::Hart &hart, const instruction::EncInstr &instr) {
    hart.set_reg(instr.rd, hart.get_reg(instr.rs1) + static_cast<hart::signed_reg_t>(instr.imm));
}

void Executor::execute_slti(hart::Hart &hart, const instruction::EncInstr &instr) {
    hart.set_reg(instr.rd, static_cast<hart::signed_reg_t>(hart.get_reg(instr.rs1)) <
                               static_cast<hart::signed_reg_t>(instr.imm));
}

void Executor::execute_sltiu(hart::Hart &hart, const instruction::EncInstr &instr) {
    hart.set_reg(instr.rd, hart.get_reg(instr.rs1) < instr.imm);
}

void Executor::execute_andi(hart::Hart &hart, const instruction::EncInstr &instr) {
    hart.set_reg(instr.rd, hart.get_reg(instr.rs1) & instr.imm);
}

void Executor::execute_ori(hart::Hart &hart, const instruction::EncInstr &instr) {
    hart.set_reg(instr.rd, hart.get_reg(instr.rs1) | instr.imm);
}

void Executor::execute_xori(hart::Hart &hart, const instruction::EncInstr &instr) {
    hart.set_reg(instr.rd, hart.get_reg(instr.rs1) ^ instr.imm);
}

void Executor::execute_slli(hart::Hart &hart, const instruction::EncInstr &instr) {
    hart.set_reg(instr.rd, hart.get_reg(instr.rs1) << decoder::bits<5, 0>(instr.imm));
}

void Executor::execute_srli(hart::Hart &hart, const instruction::EncInstr &instr) {
    hart.set_reg(instr.rd, hart.get_reg(instr.rs1) >> decoder::bits<5, 0>(instr.imm));
}

void Executor::execute_srai(hart::Hart &hart, const instruction::EncInstr &instr) {
    hart.set_reg(instr.rd, static_cast<hart::reg_t>(
                               static_cast<hart::signed_reg_t>(hart.get_reg(instr.rs1)) >>
                               decoder::bits<5, 0>(instr.imm)));
}

void Executor::execute_addiw(hart::Hart &hart, const instruction::EncInstr &instr) {
    hart.set_reg(instr.rd, decoder::sext<31>(hart.get_reg(instr.rs1) + instr.imm));
}

void Executor::execute_slliw(hart::Hart &hart, const instruction::EncInstr &instr) {
    hart.set_reg(instr.rd,
                 decoder::sext<31>(hart.get_reg(instr.rs1) << decoder::bits<4, 0>(instr.imm)));
}

void Executor::execute_srliw(hart::Hart &hart, const instruction::EncInstr &instr) {
    hart.set_reg(instr.rd, decoder::sext<31>(decoder::bits<31, 0>(hart.get_reg(instr.rs1)) >>
                                             decoder::bits<4, 0>(instr.imm)));
}

void Executor::execute_sraiw(hart::Hart &hart, const instruction::EncInstr &instr) {
    hart.set_reg(instr.rd, decoder::sext<31>(static_cast<hart::reg_t>(
                               static_cast<hart::signed_reg_t>(hart.get_reg(instr.rs1)) >>
                               decoder::bits<4, 0>(instr.imm))));
}

// void Executor::execute_ecall(hart::Hart &hart, const instruction::EncInstr &instr) {}
// void Executor::execute_ebreak(hart::Hart &hart, const instruction::EncInstr &instr) {}
// void Executor::execute_fence(hart::Hart &hart, const instruction::EncInstr &instr) {}
// void Executor::execute_fence_i(hart::Hart &hart, const instruction::EncInstr &instr) {}

// S - type
void Executor::execute_sb(hart::Hart &hart, const instruction::EncInstr &instr) {
    hart.store<uint8_t>(hart.get_reg(instr.rs1) + instr.imm,
                        static_cast<uint8_t>(hart.get_reg(instr.rs2)));
}

void Executor::execute_sh(hart::Hart &hart, const instruction::EncInstr &instr) {
    hart.store<uint16_t>(hart.get_reg(instr.rs1) + instr.imm,
                         static_cast<uint16_t>(hart.get_reg(instr.rs2)));
}

void Executor::execute_sw(hart::Hart &hart, const instruction::EncInstr &instr) {
    hart.store<uint32_t>(hart.get_reg(instr.rs1) + instr.imm,
                         static_cast<uint32_t>(hart.get_reg(instr.rs2)));
}

void Executor::execute_sd(hart::Hart &hart, const instruction::EncInstr &instr) {
    hart.store<uint64_t>(hart.get_reg(instr.rs1) + instr.imm, hart.get_reg(instr.rs2));
}

// B - type
void Executor::execute_beq(hart::Hart &hart, const instruction::EncInstr &instr) {
    if (hart.get_reg(instr.rs1) == hart.get_reg(instr.rs2)) {
        hart.set_next_pc(hart.get_pc() + instr.imm);
    }
}

void Executor::execute_bne(hart::Hart &hart, const instruction::EncInstr &instr) {
    if (hart.get_reg(instr.rs1) != hart.get_reg(instr.rs2)) {
        hart.set_next_pc(hart.get_pc() + instr.imm);
    }
}
void Executor::execute_blt(hart::Hart &hart, const instruction::EncInstr &instr) {
    if (static_cast<hart::signed_reg_t>(hart.get_reg(instr.rs1)) <
        static_cast<hart::signed_reg_t>(hart.get_reg(instr.rs2))) {
        hart.set_next_pc(hart.get_pc() + instr.imm);
    }
}

void Executor::execute_bltu(hart::Hart &hart, const instruction::EncInstr &instr) {
    if (hart.get_reg(instr.rs1) < hart.get_reg(instr.rs2)) {
        hart.set_next_pc(hart.get_pc() + instr.imm);
    }
}
void Executor::execute_bge(hart::Hart &hart, const instruction::EncInstr &instr) {
    if (static_cast<hart::signed_reg_t>(hart.get_reg(instr.rs1)) >=
        static_cast<hart::signed_reg_t>(hart.get_reg(instr.rs2))) {
        hart.set_next_pc(hart.get_pc() + instr.imm);
    }
}
void Executor::execute_bgeu(hart::Hart &hart, const instruction::EncInstr &instr) {
    if (hart.get_reg(instr.rs1) >= hart.get_reg(instr.rs2)) {
        hart.set_next_pc(hart.get_pc() + instr.imm);
    }
}

// U - type
void Executor::execute_lui(hart::Hart &hart, const instruction::EncInstr &instr) {
    hart.set_reg(instr.rd, instr.imm);
}

void Executor::execute_auipc(hart::Hart &hart, const instruction::EncInstr &instr) {
    hart.set_reg(instr.rd, hart.get_pc() + instr.imm);
}

// J - type
void Executor::execute_jal(hart::Hart &hart, const instruction::EncInstr &instr) {
    hart.set_reg(instr.rd, hart.get_pc_next());
    hart.set_next_pc(hart.get_reg(instr.rs1) + instr.imm);
}

}  // namespace executor
