#include "memory.hpp"

#include <assert.h>

#include <stdexcept>

#include "common.hpp"
#include "common_mem.hpp"
#include "page_table_entry.hpp"

namespace sim {

[[nodiscard]] uint64_t my_bits(uint64_t value, size_t high_bit, size_t low_bit) {
    assert(high_bit >= low_bit);

    return ((uint64_t(1) << (high_bit - low_bit + 1)) - 1) & (value >> low_bit);
}

[[nodiscard]] size_t getVPN(virtual_address_t virtual_address, size_t level) {
    assert(level < 3 && "Sv39 does not support VPN level higher than 2");

    size_t low_bit = kPageBitSize + level * kVPNBitSize;
    size_t high_bit = low_bit + kVPNBitSize;

    return my_bits(virtual_address, high_bit, low_bit);
}

[[nodiscard]] physical_address_t MMU::translate(virtual_address_t virtual_address,
                                                MMU::Mode mode) const {
    // check user_mode: S or U

    // Logger &myLogger = Logger::getInstance();
    // myLogger.message(Logger::standard, "translate",
    //                  fmt::format("Bare: {:#x}", reinterpret_cast<physical_address_t>(
    //                                                 m_phys_memory.get_mem() + virtual_address)));

    if (mode == Mode::Bare) {
        return virtual_address;
    }

    // 1.
    size_t table_ppn = m_satp_addr * kPageSize;
    size_t level = 2;  // Sv39 max level

    // myLogger.message(Logger::standard, "translate", fmt::format("table_ppn: {:#x}", table_ppn));

    pte_t pte_value = 0;

    // 2.
    while (true) {
        physical_address_t pte_addr = table_ppn + getVPN(virtual_address, level) * sizeof(pte_t);

        // myLogger.message(Logger::standard, "translate", fmt::format("pte_addr: {:#x}", pte_addr));

        m_phys_memory.load<pte_t>(pte_addr, pte_value);
        // myLogger.message(Logger::standard, "translate", fmt::format("pte_value: {:#x}", pte_value));

        PageTableEntry Pte(pte_value);
        // 3.
        // bool reserved_bits_is_null = ((Pte.n() | Pte.pbmt() | Pte.reserved() | Pte.rsw()) == 0);
        // if ((Pte.valid() == 0) || (Pte.r() == 0 && Pte.w() == 1) || !reserved_bits_is_null) {
        //     throw std::runtime_error("page fault");
        // }

        // 4.
        // if (Pte.r() == 1 || Pte.x() == 1) {
        //     // find leaf PTE
        //     break;
        // }

        if (level == 0) {
            // throw std::runtime_error("page fault");
            break;
        }

        --level;
        // table_ppn = Pte.getPPN(level) * kPageSize;
        table_ppn = pte_value;

        // myLogger.message(Logger::standard, "translate", fmt::format("table_ppn: {:#x}", table_ppn));
    }

    // 5. checkLeafFlags

    // 6.
    // if (level > 0) {
    //     if (my_bits(pte_value, level - 1, 0) != 0) {
    //         throw std::runtime_error("page fault");
    //     }
    // }

    // check access and dirty flags

    // create final pte addr from Pte struct

    // m_phys_memory.load()

    PageTableEntry Pte(pte_value);

    physical_address_t offset = my_bits(virtual_address, kPageBitSize - 1, 0);  // check offset
    physical_address_t page_address = Pte.getPageAddress();
    return page_address + offset;

    // return m_phys_memory.load(pte);
}

void MMU::allocaPTEs(size_t vpn2_table_size, size_t vpn1_table_size, size_t vpn0_table_size) {
    m_vpn2_size = vpn2_table_size;
    m_vpn1_size = vpn1_table_size;
    m_vpn0_size = vpn0_table_size;

    // Logger &myLogger = Logger::getInstance();

    m_phys_memory.reservePages(m_satp_addr + 1 + m_vpn2_size + m_vpn2_size * m_vpn1_size);
    // myLogger.message(Logger::standard, "memory",
    //                  fmt::format("page_counter: {:d}", m_phys_memory.getPageCounter()));
}

void MMU::fillPTEs(virtual_address_t virtual_address) {
    Logger &myLogger = Logger::getInstance();

    auto vpn2 = getVPN(virtual_address, 2);
    auto vpn1 = getVPN(virtual_address, 1);
    auto vpn0 = getVPN(virtual_address, 0);

    physical_address_t vpn2_table_start = m_satp_addr * kPageSize;
    physical_address_t vpn1_table_start = vpn2_table_start + kPageSize;
    physical_address_t vpn0_table_start =
        vpn1_table_start + kPageSize * m_vpn2_size + vpn2 * m_vpn2_size + vpn1 * kPageSize;

    // myLogger.message(Logger::standard, "memory",
    //                  fmt::format("vpn2_table_start: {:#x}", vpn2_table_start));
    // myLogger.message(Logger::standard, "memory",
    //                  fmt::format("vpn1_table_start: {:#x}", vpn1_table_start));
    // myLogger.message(Logger::standard, "memory",
    //                  fmt::format("vpn0_table_start: {:#x}", vpn0_table_start));

    m_phys_memory.store<physical_address_t>(vpn2_table_start + vpn2 * sizeof(pte_t),
                                            vpn1_table_start);

    myLogger.message(Logger::standard, "memory",
                     fmt::format("store: [{:#x}] = {:#x}", vpn2_table_start + vpn2 * sizeof(pte_t),
                                 vpn1_table_start));

    m_phys_memory.store<physical_address_t>(vpn1_table_start + vpn1 * sizeof(pte_t),
                                            vpn0_table_start);

    myLogger.message(Logger::standard, "memory",
                     fmt::format("store: [{:#x}] = {:#x}", vpn1_table_start + vpn1 * sizeof(pte_t),
                                 vpn0_table_start));

    auto pg_addr = m_phys_memory.getPage();

    m_phys_memory.store<physical_address_t>(vpn0_table_start + vpn0 * sizeof(pte_t), pg_addr);

    myLogger.message(
        Logger::standard, "memory",
        fmt::format("store: [{:#x}] = {:#x}", vpn0_table_start + vpn0 * sizeof(pte_t), pg_addr));
}

}  // namespace sim