#include "decoder.hpp"
#include "instruction.hpp"

int main() {
    instruction::EncInstr enc_insn{};
    decoder::Decoder::decode_instruction(0x13, enc_insn);
}
