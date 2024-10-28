#include "instruction.hpp"

#include <sstream>
#include <stdexcept>
#include <string>

namespace instruction {

InstrType EncInstr::get_instruction_type() {
    switch (id) {
        // R - type
        case instruction::InstrId::ADD:
        case instruction::InstrId::SUB:
        case instruction::InstrId::SLL:
        case instruction::InstrId::SLT:
        case instruction::InstrId::SLTU:
        case instruction::InstrId::XOR:
        case instruction::InstrId::SRL:
        case instruction::InstrId::SRA:
        case instruction::InstrId::OR:
        case instruction::InstrId::AND:
        case instruction::InstrId::ADDW:
        case instruction::InstrId::SLLW:
        case instruction::InstrId::SRLW:
        case instruction::InstrId::SUBW:
        case instruction::InstrId::SRAW: {
            return InstrType::R;
        }
            // I - type

        case instruction::InstrId::JALR:
        case instruction::InstrId::LB:
        case instruction::InstrId::LH:
        case instruction::InstrId::LW:
        case instruction::InstrId::LBU:
        case instruction::InstrId::LHU:
        case instruction::InstrId::ADDI:
        case instruction::InstrId::SLTI:
        case instruction::InstrId::SLTIU:
        case instruction::InstrId::XORI:
        case instruction::InstrId::ORI:
        case instruction::InstrId::ANDI:
        case instruction::InstrId::LWU:
        case instruction::InstrId::LD:
        case instruction::InstrId::SLLI:
        case instruction::InstrId::SRLI:
        case instruction::InstrId::SRAI:
        case instruction::InstrId::ADDIW:
        case instruction::InstrId::SLLIW:
        case instruction::InstrId::SRLIW:
        case instruction::InstrId::SRAIW: {
            // case instruction::InstrId::ECALL:
            // case instruction::InstrId::EBREAK:
            // case instruction::InstrId::FENCE:
            // case instruction::InstrId::FENCE_I:
            // hint instruction
            // case instruction::InstrId::PAUSE:
            return InstrType::I;
        }

        // S - type
        case instruction::InstrId::SB:
        case instruction::InstrId::SH:
        case instruction::InstrId::SW:
        case instruction::InstrId::SD: {
            return InstrType::S;
        }
        // B - type
        case instruction::InstrId::BEQ:
        case instruction::InstrId::BNE:
        case instruction::InstrId::BLT:
        case instruction::InstrId::BGE:
        case instruction::InstrId::BLTU:
        case instruction::InstrId::BGEU: {
            return InstrType::B;
        }
        // U - type
        case instruction::InstrId::LUI:
        case instruction::InstrId::AUIPC: {
            return InstrType::U;
        }
        // J - type
        case instruction::InstrId::JAL: {
            return InstrType::J;
        }
        default: {
            std::ostringstream oss{};
            oss << std::hex << std::to_string(static_cast<instr_t>(id));
            throw std::invalid_argument("Recieved unknown instruction id: " + oss.str());
        }
    }
}

}  // namespace instruction
