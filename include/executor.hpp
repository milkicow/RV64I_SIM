#include "hart.hpp"
#include "instruction.hpp"

namespace executor {

class Executor {
    // R - type
    static void execute_add(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_sub(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_sll(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_slt(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_sltu(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_xor(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_srl(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_sra(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_or(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_and(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_addw(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_sllw(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_srlw(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_subw(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_sraw(hart::Hart &hart, const instruction::EncInstr &instr);

    // I - type
    static void execute_jalr(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_lb(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_lh(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_lw(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_lbu(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_lhu(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_addi(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_slti(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_sltiu(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_xori(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_ori(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_andi(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_lwu(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_ld(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_slli(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_srli(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_srai(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_addiw(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_slliw(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_srliw(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_sraiw(hart::Hart &hart, const instruction::EncInstr &instr);
    // static void execute_ecall(hart::Hart &hart, const instruction::EncInstr &instr);
    // static void execute_ebreak(hart::Hart &hart, const instruction::EncInstr &instr);
    // static void execute_fence(hart::Hart &hart, const instruction::EncInstr &instr);
    // static void execute_fence_i(hart::Hart &hart, const instruction::EncInstr &instr);

    // static void execute_pause(hart::Hart &hart, const instruction::EncInstr &instr);

    // S - type
    static void execute_sb(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_sh(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_sw(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_sd(hart::Hart &hart, const instruction::EncInstr &instr);

    // B - type
    static void execute_beq(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_bne(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_blt(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_bge(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_bltu(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_bgeu(hart::Hart &hart, const instruction::EncInstr &instr);

    // U - type
    static void execute_lui(hart::Hart &hart, const instruction::EncInstr &instr);
    static void execute_auipc(hart::Hart &hart, const instruction::EncInstr &instr);

    // J - type
    static void execute_jal(hart::Hart &hart, const instruction::EncInstr &instr);
};

}  // namespace executor
