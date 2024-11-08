#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "decoder.hpp"
#include "instruction.hpp"

namespace instruction {

struct DecoderTest : public testing::Test {
    EncInstr enc_insn{};

    bool check_inst_decode(int32_t opcode, InstrId expected_opcode) {
        decoder::Decoder::decode_instruction(opcode, enc_insn);
        return enc_insn.id == expected_opcode;
    }
};


TEST_F(DecoderTest, LUI) {
    EXPECT_EQ(check_inst_decode(0x37, InstrId::LUI), true);
}

TEST_F(DecoderTest, ADD) {
    EXPECT_EQ(check_inst_decode(0x33, InstrId::ADD), true);
}

TEST_F(DecoderTest, SUB) {
    EXPECT_EQ(check_inst_decode(0x40000033, InstrId::SUB), true);
}

TEST_F(DecoderTest, SLL) {
    EXPECT_EQ(check_inst_decode(0x1033, InstrId::SLL), true);
}

TEST_F(DecoderTest, SLT) {
    EXPECT_EQ(check_inst_decode(0x2033, InstrId::SLT), true);
}

TEST_F(DecoderTest, SLTU) {
    EXPECT_EQ(check_inst_decode(0x3033, InstrId::SLTU), true);
}

TEST_F(DecoderTest, XOR) {
    EXPECT_EQ(check_inst_decode(0x4033, InstrId::XOR), true);
}

TEST_F(DecoderTest, SRL) {
    EXPECT_EQ(check_inst_decode(0x5033, InstrId::SRL), true);
}

TEST_F(DecoderTest, SRA) {
    EXPECT_EQ(check_inst_decode(0x40005033, InstrId::SRA), true);
}

TEST_F(DecoderTest, OR) {
    EXPECT_EQ(check_inst_decode(0x6033, InstrId::OR), true);
}

TEST_F(DecoderTest, AND) {
    EXPECT_EQ(check_inst_decode(0x7033, InstrId::AND), true);
}

TEST_F(DecoderTest, ADDW) {
    EXPECT_EQ(check_inst_decode(0x3b, InstrId::ADDW), true);
}

TEST_F(DecoderTest, SLLW) {
    EXPECT_EQ(check_inst_decode(0x103b, InstrId::SLLW), true);
}

TEST_F(DecoderTest, SRLW) {
    EXPECT_EQ(check_inst_decode(0x503b, InstrId::SRLW), true);
}

TEST_F(DecoderTest, SUBW) {
    EXPECT_EQ(check_inst_decode(0x4000003b, InstrId::SUBW), true);
}

TEST_F(DecoderTest, SRAW) {
    EXPECT_EQ(check_inst_decode(0x4000503b, InstrId::SRAW), true);
}




TEST_F(DecoderTest, JALR) {
    EXPECT_EQ(check_inst_decode(0x67, InstrId::JALR), true);
}

TEST_F(DecoderTest, LB) {
    EXPECT_EQ(check_inst_decode(0x3, InstrId::LB), true);
}

TEST_F(DecoderTest, LH) {
    EXPECT_EQ(check_inst_decode(0x1003, InstrId::LH), true);
}

TEST_F(DecoderTest, LW) {
    EXPECT_EQ(check_inst_decode(0x2003, InstrId::LW), true);
}

TEST_F(DecoderTest, LBU) {
    EXPECT_EQ(check_inst_decode(0x4003, InstrId::LBU), true);
}

TEST_F(DecoderTest, LHU) {
    EXPECT_EQ(check_inst_decode(0x5003, InstrId::LHU), true);
}

TEST_F(DecoderTest, ADDI) {
    EXPECT_EQ(check_inst_decode(0x13, InstrId::ADDI), true);
}

TEST_F(DecoderTest, SLTI) {
    EXPECT_EQ(check_inst_decode(0x2013, InstrId::SLTI), true);
}

TEST_F(DecoderTest, SLTIU) {
    EXPECT_EQ(check_inst_decode(0x3013, InstrId::SLTIU), true);
}

TEST_F(DecoderTest, XORI) {
    EXPECT_EQ(check_inst_decode(0x4013, InstrId::XORI), true);
}

TEST_F(DecoderTest, ORI) {
    EXPECT_EQ(check_inst_decode(0x6013, InstrId::ORI), true);
}

TEST_F(DecoderTest, ANDI) {
    EXPECT_EQ(check_inst_decode(0x7013, InstrId::ANDI), true);
}

TEST_F(DecoderTest, LWU) {
    EXPECT_EQ(check_inst_decode(0x6003, InstrId::LWU), true);
}

TEST_F(DecoderTest, LD) {
    EXPECT_EQ(check_inst_decode(0x3003, InstrId::LD), true);
}

TEST_F(DecoderTest, SLLI) {
    EXPECT_EQ(check_inst_decode(0x1013, InstrId::SLLI), true);
}

TEST_F(DecoderTest, SRLI) {
    EXPECT_EQ(check_inst_decode(0x5013, InstrId::SRLI), true);
}

TEST_F(DecoderTest, SRAI) {
    EXPECT_EQ(check_inst_decode(0x40005013, InstrId::SRAI), true);
}

TEST_F(DecoderTest, ADDIW) {
    EXPECT_EQ(check_inst_decode(0x1b, InstrId::ADDIW), true);
}

TEST_F(DecoderTest, SLLIW) {
    EXPECT_EQ(check_inst_decode(0x101b, InstrId::SLLIW), true);
}

TEST_F(DecoderTest, SRLIW) {
    EXPECT_EQ(check_inst_decode(0x501b, InstrId::SRLIW), true);
}

TEST_F(DecoderTest, SRAIW) {
    EXPECT_EQ(check_inst_decode(0x4000501b, InstrId::SRAIW), true);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

}  // namespace instruction