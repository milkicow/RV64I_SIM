#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "decoder.hpp"
#include "instruction.hpp"

namespace instruction {

enum class InstrType : uint8_t { R, I, S, B, U, J };

static InstrType get_instruction_type(EncInstr inst) {
    switch (inst.id) {
        // R - type
        case InstrId::ADD:
        case InstrId::SUB:
        case InstrId::SLL:
        case InstrId::SLT:
        case InstrId::SLTU:
        case InstrId::XOR:
        case InstrId::SRL:
        case InstrId::SRA:
        case InstrId::OR:
        case InstrId::AND:
        case InstrId::ADDW:
        case InstrId::SLLW:
        case InstrId::SRLW:
        case InstrId::SUBW:
        case InstrId::SRAW: {
            return InstrType::R;
        }
            // I - type

        case InstrId::JALR:
        case InstrId::LB:
        case InstrId::LH:
        case InstrId::LW:
        case InstrId::LBU:
        case InstrId::LHU:
        case InstrId::ADDI:
        case InstrId::SLTI:
        case InstrId::SLTIU:
        case InstrId::XORI:
        case InstrId::ORI:
        case InstrId::ANDI:
        case InstrId::LWU:
        case InstrId::LD:
        case InstrId::SLLI:
        case InstrId::SRLI:
        case InstrId::SRAI:
        case InstrId::ADDIW:
        case InstrId::SLLIW:
        case InstrId::SRLIW:
        case InstrId::SRAIW: {
            // case InstrId::ECALL:
            // case InstrId::EBREAK:
            // case InstrId::FENCE:
            // case InstrId::FENCE_I:
            // hint instruction
            // case InstrId::PAUSE:
            return InstrType::I;
        }

        // S - type
        case InstrId::SB:
        case InstrId::SH:
        case InstrId::SW:
        case InstrId::SD: {
            return InstrType::S;
        }
        // B - type
        case InstrId::BEQ:
        case InstrId::BNE:
        case InstrId::BLT:
        case InstrId::BGE:
        case InstrId::BLTU:
        case InstrId::BGEU: {
            return InstrType::B;
        }
        // U - type
        case InstrId::LUI:
        case InstrId::AUIPC: {
            return InstrType::U;
        }
        // J - type
        case InstrId::JAL: {
            return InstrType::J;
        }
        default: {
            std::ostringstream oss{};
            oss << std::hex << std::to_string(static_cast<instr_t>(inst.id));
            throw std::invalid_argument("Recieved unknown instruction id: " + oss.str());
        }
    }
}



struct DecoderTest : public testing::Test {
    EncInstr enc_insn{};

    bool check_inst_decode(int32_t opcode, InstrType expected_type, InstrId expected_opcode) {
        decoder::Decoder::decode_instruction(opcode, enc_insn);
        return (get_instruction_type(enc_insn) == expected_type) && (enc_insn.id == expected_opcode);
    }
};


TEST_F(DecoderTest, LUI) {
    EXPECT_EQ(check_inst_decode(0x37, InstrType::U, InstrId::LUI), true);
}

TEST_F(DecoderTest, ADD) {
    EXPECT_EQ(check_inst_decode(0x33, InstrType::R, InstrId::ADD), true);
}

TEST_F(DecoderTest, SUB) {
    EXPECT_EQ(check_inst_decode(0x40000033, InstrType::R, InstrId::SUB), true);
}

TEST_F(DecoderTest, SLL) {
    EXPECT_EQ(check_inst_decode(0x1033, InstrType::R, InstrId::SLL), true);
}

TEST_F(DecoderTest, SLT) {
    EXPECT_EQ(check_inst_decode(0x2033, InstrType::R, InstrId::SLT), true);
}

TEST_F(DecoderTest, SLTU) {
    EXPECT_EQ(check_inst_decode(0x3033, InstrType::R, InstrId::SLTU), true);
}

TEST_F(DecoderTest, XOR) {
    EXPECT_EQ(check_inst_decode(0x4033, InstrType::R, InstrId::XOR), true);
}

TEST_F(DecoderTest, SRL) {
    EXPECT_EQ(check_inst_decode(0x5033, InstrType::R, InstrId::SRL), true);
}

TEST_F(DecoderTest, SRA) {
    EXPECT_EQ(check_inst_decode(0x40005033, InstrType::R, InstrId::SRA), true);
}

TEST_F(DecoderTest, OR) {
    EXPECT_EQ(check_inst_decode(0x6033, InstrType::R, InstrId::OR), true);
}

TEST_F(DecoderTest, AND) {
    EXPECT_EQ(check_inst_decode(0x7033, InstrType::R, InstrId::AND), true);
}

TEST_F(DecoderTest, ADDW) {
    EXPECT_EQ(check_inst_decode(0x3b, InstrType::R, InstrId::ADDW), true);
}

TEST_F(DecoderTest, SLLW) {
    EXPECT_EQ(check_inst_decode(0x103b, InstrType::R, InstrId::SLLW), true);
}

TEST_F(DecoderTest, SRLW) {
    EXPECT_EQ(check_inst_decode(0x503b, InstrType::R, InstrId::SRLW), true);
}

TEST_F(DecoderTest, SUBW) {
    EXPECT_EQ(check_inst_decode(0x4000003b, InstrType::R, InstrId::SUBW), true);
}

TEST_F(DecoderTest, SRAW) {
    EXPECT_EQ(check_inst_decode(0x4000503b, InstrType::R, InstrId::SRAW), true);
}




TEST_F(DecoderTest, JALR) {
    EXPECT_EQ(check_inst_decode(0x67, InstrType::I, InstrId::JALR), true);
}

TEST_F(DecoderTest, LB) {
    EXPECT_EQ(check_inst_decode(0x3, InstrType::I, InstrId::LB), true);
}

TEST_F(DecoderTest, LH) {
    EXPECT_EQ(check_inst_decode(0x1003, InstrType::I, InstrId::LH), true);
}

TEST_F(DecoderTest, LW) {
    EXPECT_EQ(check_inst_decode(0x2003, InstrType::I, InstrId::LW), true);
}

TEST_F(DecoderTest, LBU) {
    EXPECT_EQ(check_inst_decode(0x4003, InstrType::I, InstrId::LBU), true);
}

TEST_F(DecoderTest, LHU) {
    EXPECT_EQ(check_inst_decode(0x5003, InstrType::I, InstrId::LHU), true);
}

TEST_F(DecoderTest, ADDI) {
    EXPECT_EQ(check_inst_decode(0x13, InstrType::I, InstrId::ADDI), true);
}

TEST_F(DecoderTest, SLTI) {
    EXPECT_EQ(check_inst_decode(0x2013, InstrType::I, InstrId::SLTI), true);
}

TEST_F(DecoderTest, SLTIU) {
    EXPECT_EQ(check_inst_decode(0x3013, InstrType::I, InstrId::SLTIU), true);
}

TEST_F(DecoderTest, XORI) {
    EXPECT_EQ(check_inst_decode(0x4013, InstrType::I, InstrId::XORI), true);
}

TEST_F(DecoderTest, ORI) {
    EXPECT_EQ(check_inst_decode(0x6013, InstrType::I, InstrId::ORI), true);
}

TEST_F(DecoderTest, ANDI) {
    EXPECT_EQ(check_inst_decode(0x7013, InstrType::I, InstrId::ANDI), true);
}

TEST_F(DecoderTest, LWU) {
    EXPECT_EQ(check_inst_decode(0x6003, InstrType::I, InstrId::LWU), true);
}

TEST_F(DecoderTest, LD) {
    EXPECT_EQ(check_inst_decode(0x3003, InstrType::I, InstrId::LD), true);
}

TEST_F(DecoderTest, SLLI) {
    EXPECT_EQ(check_inst_decode(0x1013, InstrType::I, InstrId::SLLI), true);
}

TEST_F(DecoderTest, SRLI) {
    EXPECT_EQ(check_inst_decode(0x5013, InstrType::I, InstrId::SRLI), true);
}

TEST_F(DecoderTest, SRAI) {
    EXPECT_EQ(check_inst_decode(0x40005013, InstrType::I, InstrId::SRAI), true);
}

TEST_F(DecoderTest, ADDIW) {
    EXPECT_EQ(check_inst_decode(0x1b, InstrType::I, InstrId::ADDIW), true);
}

TEST_F(DecoderTest, SLLIW) {
    EXPECT_EQ(check_inst_decode(0x101b, InstrType::I, InstrId::SLLIW), true);
}

TEST_F(DecoderTest, SRLIW) {
    EXPECT_EQ(check_inst_decode(0x501b, InstrType::I, InstrId::SRLIW), true);
}

TEST_F(DecoderTest, SRAIW) {
    EXPECT_EQ(check_inst_decode(0x4000501b, InstrType::I, InstrId::SRAIW), true);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

}  // namespace instruction