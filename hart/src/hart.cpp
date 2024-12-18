#include "hart.hpp"

#include <exception>
#include <ranges>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include "common_mem.hpp"
#include "elfio/elfio.hpp"
#include "logger.hpp"

namespace sim {

std::string Hart::format_registers() {
    std::ostringstream oss{};
    for (size_t i = 0; i < m_regfile.size(); ++i) {
        oss << "\nregister[" << i << "] = " << std::dec << m_regfile[i];
    }
    return oss.str();
}

static std::tuple<size_t, size_t, size_t> count_tree_size(std::vector<virtual_address_t> pages) {
    std::set<size_t> ptn2{};
    std::set<size_t> ptn1{};
    std::set<size_t> ptn0{};

    for (auto &page : pages) {
        ptn2.insert(getVPN(page, 2));
        ptn1.insert(getVPN(page, 1));
        ptn0.insert(getVPN(page, 0));
    }

    return {ptn2.size(), ptn1.size(), ptn0.size()};
}

void Hart::load_elf_file(std::string &elf_file) {
    ELFIO::elfio reader;
    reader.load(elf_file);

    if (reader.get_class() != ELFIO::ELFCLASS64) {
        throw std::runtime_error{"Elf file class doesn't match with ELFCLASS64"};
    }

    auto is_segment_loadable = [](const auto &segment) {
        return segment->get_type() == ELFIO::PT_LOAD;
    };

    Logger &myLogger = Logger::getInstance();

    std::vector<virtual_address_t> pages{};

    for (auto &segment : reader.segments | std::views::filter(is_segment_loadable)) {
        auto segment_address = segment->get_virtual_address();
        int segment_size = segment->get_memory_size();
        size_t i = 0;
        while (segment_size > 0) {
            pages.push_back(segment_address + i * kPageSize);
            segment_size -= kPageSize;
            ++i;
        }

        myLogger.message(
            Logger::standard, "hart",
            fmt::format("segment virtual address: {:#x}", segment->get_virtual_address()));
        myLogger.message(Logger::standard, "hart",
                         fmt::format("segment size: {:#x}", segment->get_memory_size()));

        m_mem.store(segment->get_virtual_address(), segment->get_data(),
                    segment->get_memory_size());
    }

    size_t vpn2_table_size = 0;
    size_t vpn1_table_size = 0;
    size_t vpn0_table_size = 0;

    pages.push_back(0x90000);

    std::tie(vpn2_table_size, vpn1_table_size, vpn0_table_size) = count_tree_size(pages);

    myLogger.message(Logger::standard, "hart",
                     fmt::format("vpn2_table_size: {:d}", vpn2_table_size));
    myLogger.message(Logger::standard, "hart",
                     fmt::format("vpn1_table_size: {:d}", vpn1_table_size));
    myLogger.message(Logger::standard, "hart",
                     fmt::format("vpn0_table_size: {:d}", vpn0_table_size));

    m_mmu.allocaPTEs(vpn2_table_size, vpn1_table_size, vpn0_table_size);

    for (auto &page : pages) {
        m_mmu.fillPTEs(page);
    }

    auto translated = m_mmu.translate(pages[0] + 256, MMU::Mode::Sv39);
    myLogger.message(Logger::standard, "hart", fmt::format("translated: {:#x}", translated));

    myLogger.message(Logger::standard, "hart",
                     fmt::format("page_counter: {:d}", m_mem.getPageCounter()));

    // for (auto &page : pages) {
    //     myLogger.message(Logger::standard, "hart",
    //                      fmt::format("page virtual address: {:#x}", page));
    //     m_mmu.collectVPNs(page);
    // }

    auto start_pc = reader.get_entry();
    m_pc = start_pc;
    m_pc_next = start_pc + 4;

    set_reg(2, 0x90000);
}

// uint64_t Hart::get_pc() const noexcept { return m_pc; }

// uint64_t Hart::get_pc_next() const noexcept { return m_pc_next; }

// reg_t Hart::get_reg(reg_id_t reg_id) const { return m_regfile[reg_id]; }

// void Hart::set_pc(addr_t pc) noexcept { m_pc = pc; }

// void Hart::set_next_pc(addr_t pc_next) noexcept { m_pc_next = pc_next; }

// void Hart::set_reg(reg_id_t reg_id, reg_t value) {
//     m_regfile[reg_id] = value;
//     m_regfile[0] = 0;
// };

}  // namespace sim
