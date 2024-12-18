#include "page_table_entry.hpp"

#include <assert.h>

#include "memory.hpp"

namespace sim {

[[nodiscard]] size_t PageTableEntry::getPPN(size_t level) {
    assert(level < 3 && "Sv39 does not support PPN level higher than 2");
    constexpr size_t kPPNLowBitStep = 9;
    size_t low_bit = kPPNLowBit + level * kPPNLowBitStep;

    size_t kPPNSizeBit = 9;
    if (level == 2) {
        kPPNSizeBit = 26;
    }
    size_t high_bit = low_bit + kPPNSizeBit;
    return my_bits(m_pte, high_bit, low_bit);
}

[[nodiscard]] physical_address_t PageTableEntry::getPageAddress() {
    return my_bits(m_pte, kPPNHighBit, kPPNLowBit) << kPPNLowBit;
}

}  // namespace sim