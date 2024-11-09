#include <cstdint>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "decoder.hpp"
#include "instruction.hpp"

namespace instruction {

struct ExpectedEncInstr : EncInstr {
    ExpectedEncInstr(InstrId id_, uint8_t rd_, uint8_t rs1_,
                     uint8_t rs2_, uint32_t imm_) :
                     EncInstr(id_, rd_, rs1_, rs2_, imm_) {}

    ExpectedEncInstr(InstrId id_) :
                     EncInstr(id_, 0, 0, 0, 0) {}

    ExpectedEncInstr(InstrId id_, uint32_t imm_) : EncInstr(id_, 0, 0, 0, imm_) {}
    ExpectedEncInstr() : EncInstr() {}

    constexpr bool operator==(const EncInstr& rhs)
    {
        return (id == rhs.id) && (imm == rhs.imm)  && (rd == rhs.rd) &&
               (rs1 == rhs.rs1) && ( rs1 == rhs.rs1);
    }
};

struct DecoderTest : public testing::Test {
    EncInstr enc_insn{};
    ExpectedEncInstr expected_insn{};
    bool check_inst_decode(int32_t opcode, InstrId expected_opcode) {
        decoder::Decoder::decode_instruction(opcode, enc_insn);
        return (enc_insn.id == expected_opcode);
    }

    bool check_inst_decode(int32_t opcode, ExpectedEncInstr expected_inst) {
        decoder::Decoder::decode_instruction(opcode, enc_insn);
        return (enc_insn == expected_inst);
    }
};

TEST_F(DecoderTest, LUI) {
    expected_insn = ExpectedEncInstr(InstrId::LUI);
    EXPECT_EQ(check_inst_decode(0x37, expected_insn), true);
}

TEST_F(DecoderTest, ADD) {
    expected_insn = ExpectedEncInstr(InstrId::ADD);
    EXPECT_EQ(check_inst_decode(0x33, expected_insn), true);
}

TEST_F(DecoderTest, SUB) {
    expected_insn = ExpectedEncInstr(InstrId::SUB);
    EXPECT_EQ(check_inst_decode(0x40000033, expected_insn), true);
}

TEST_F(DecoderTest, SLL) {
    expected_insn = ExpectedEncInstr(InstrId::SLL);
    EXPECT_EQ(check_inst_decode(0x1033, expected_insn), true);
}

TEST_F(DecoderTest, SLT) {
    expected_insn = ExpectedEncInstr(InstrId::SLT);
    EXPECT_EQ(check_inst_decode(0x2033, expected_insn), true);
}

TEST_F(DecoderTest, SLTU) {
    expected_insn = ExpectedEncInstr(InstrId::SLTU);
    EXPECT_EQ(check_inst_decode(0x3033, expected_insn), true);
}

TEST_F(DecoderTest, XOR) {
    expected_insn = ExpectedEncInstr(InstrId::XOR);
    EXPECT_EQ(check_inst_decode(0x4033, expected_insn), true);
}

TEST_F(DecoderTest, SRL) {
    expected_insn = ExpectedEncInstr(InstrId::SRL);
    EXPECT_EQ(check_inst_decode(0x5033, expected_insn), true);
}

TEST_F(DecoderTest, SRA) {
    expected_insn = ExpectedEncInstr(InstrId::SRA);
    EXPECT_EQ(check_inst_decode(0x40005033, expected_insn), true);
}

TEST_F(DecoderTest, OR) {
    expected_insn = ExpectedEncInstr(InstrId::OR);
    EXPECT_EQ(check_inst_decode(0x6033, expected_insn), true);
}

TEST_F(DecoderTest, AND) {
    expected_insn = ExpectedEncInstr(InstrId::AND);
    EXPECT_EQ(check_inst_decode(0x7033, expected_insn), true);
}

TEST_F(DecoderTest, ADDW) {
    expected_insn = ExpectedEncInstr(InstrId::ADDW);
    EXPECT_EQ(check_inst_decode(0x3b, expected_insn), true);
}

TEST_F(DecoderTest, SLLW) {
    expected_insn = ExpectedEncInstr(InstrId::SLLW);
    EXPECT_EQ(check_inst_decode(0x103b, expected_insn), true);
}

TEST_F(DecoderTest, SRLW) {
    expected_insn = ExpectedEncInstr(InstrId::SRLW);
    EXPECT_EQ(check_inst_decode(0x503b, expected_insn), true);
}

TEST_F(DecoderTest, SUBW) {
    expected_insn = ExpectedEncInstr(InstrId::SUBW);
    EXPECT_EQ(check_inst_decode(0x4000003b, expected_insn), true);
}

TEST_F(DecoderTest, SRAW) {
    expected_insn = ExpectedEncInstr(InstrId::SRAW);
    EXPECT_EQ(check_inst_decode(0x4000503b, expected_insn), true);
}



TEST_F(DecoderTest, JALR) {
    expected_insn = ExpectedEncInstr(InstrId::JALR);
    EXPECT_EQ(check_inst_decode(0x67, expected_insn), true);
}

TEST_F(DecoderTest, LB) {
    expected_insn = ExpectedEncInstr(InstrId::LB);
    EXPECT_EQ(check_inst_decode(0x3, expected_insn), true);
}

TEST_F(DecoderTest, LH) {
    expected_insn = ExpectedEncInstr(InstrId::LH);
    EXPECT_EQ(check_inst_decode(0x1003, expected_insn), true);
}

TEST_F(DecoderTest, LW) {
    expected_insn = ExpectedEncInstr(InstrId::LW);
    EXPECT_EQ(check_inst_decode(0x2003, expected_insn), true);
}

TEST_F(DecoderTest, LBU) {
    expected_insn = ExpectedEncInstr(InstrId::LBU);
    EXPECT_EQ(check_inst_decode(0x4003, expected_insn), true);
}

TEST_F(DecoderTest, LHU) {
    expected_insn = ExpectedEncInstr(InstrId::LHU);
    EXPECT_EQ(check_inst_decode(0x5003, expected_insn), true);
}

TEST_F(DecoderTest, ADDI) {
    expected_insn = ExpectedEncInstr(InstrId::ADDI);
    EXPECT_EQ(check_inst_decode(0x13, expected_insn), true);
}

TEST_F(DecoderTest, SLTI) {
    expected_insn = ExpectedEncInstr(InstrId::SLTI);
    EXPECT_EQ(check_inst_decode(0x2013, expected_insn), true);
}

TEST_F(DecoderTest, SLTIU) {
    expected_insn = ExpectedEncInstr(InstrId::SLTIU);
    EXPECT_EQ(check_inst_decode(0x3013, expected_insn), true);
}

TEST_F(DecoderTest, XORI) {
    expected_insn = ExpectedEncInstr(InstrId::XORI);
    EXPECT_EQ(check_inst_decode(0x4013, expected_insn), true);
}

TEST_F(DecoderTest, ORI) {
    expected_insn = ExpectedEncInstr(InstrId::ORI);
    EXPECT_EQ(check_inst_decode(0x6013, expected_insn), true);
}

TEST_F(DecoderTest, ANDI) {
    expected_insn = ExpectedEncInstr(InstrId::ANDI);
    EXPECT_EQ(check_inst_decode(0x7013, expected_insn), true);
}

TEST_F(DecoderTest, LWU) {
    expected_insn = ExpectedEncInstr(InstrId::LWU);
    EXPECT_EQ(check_inst_decode(0x6003, expected_insn), true);
}

TEST_F(DecoderTest, LD) {
    expected_insn = ExpectedEncInstr(InstrId::LD);
    EXPECT_EQ(check_inst_decode(0x3003, expected_insn), true);
}

TEST_F(DecoderTest, SLLI) {
    expected_insn = ExpectedEncInstr(InstrId::SLLI);
    EXPECT_EQ(check_inst_decode(0x1013, expected_insn), true);
}

TEST_F(DecoderTest, SRLI) {
    expected_insn = ExpectedEncInstr(InstrId::SRLI);
    EXPECT_EQ(check_inst_decode(0x5013, expected_insn), true);
}

TEST_F(DecoderTest, SRAI) {
    // The first 6 bits are set at 0100000 (1024) for distinction with SRLI
    expected_insn = ExpectedEncInstr(InstrId::SRAI, (int32_t)1024);
    EXPECT_EQ(check_inst_decode(0x40005013, expected_insn), true);
}

TEST_F(DecoderTest, ADDIW) {
    expected_insn = ExpectedEncInstr(InstrId::ADDIW);
    EXPECT_EQ(check_inst_decode(0x1b, expected_insn), true);
}

TEST_F(DecoderTest, SLLIW) {
    expected_insn = ExpectedEncInstr(InstrId::SLLIW);
    EXPECT_EQ(check_inst_decode(0x101b, expected_insn), true);
}

TEST_F(DecoderTest, SRLIW) {
    expected_insn = ExpectedEncInstr(InstrId::SRLIW);
    EXPECT_EQ(check_inst_decode(0x501b, expected_insn), true);
}

TEST_F(DecoderTest, SRAIW) {
    // The first 6 bits are set at 0100000 (1024) for distinction with SRLIW
    expected_insn = ExpectedEncInstr(InstrId::SRAIW, 1024);
    EXPECT_EQ(check_inst_decode(0x4000501b, expected_insn), true);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

}  // namespace instruction