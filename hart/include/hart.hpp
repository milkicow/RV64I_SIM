#pragma once

#include <fmt/format.h>

#include <array>
#include <cstdint>
#include <sstream>

#include "common.hpp"
#include "logger.hpp"
#include "memory.hpp"
#include "tlb.hpp"

namespace sim {

class Hart final {
   private:
    Memory m_mem;

    addr_t m_pc, m_pc_next;
    std::array<reg_t, g_regfile_size> m_regfile{};

    MMU m_mmu;
    TLB m_tlb;

   private:
    void load_elf_file(std::string &elf_file);

   public:
    Hart(std::string &elf_file) : m_mem(), m_mmu(m_mem) { load_elf_file(elf_file); }

    std::string format_registers();

    addr_t get_pc() const noexcept { return m_pc; }
    addr_t get_pc_next() const noexcept { return m_pc_next; }
    reg_t get_reg(reg_id_t reg_id) const { return m_regfile[reg_id]; }

    void set_pc(addr_t pc) noexcept { m_pc = pc; }
    void set_next_pc(addr_t pc_next) noexcept { m_pc_next = pc_next; }
    void set_reg(reg_id_t reg_id, reg_t value) {
        Logger &myLogger = Logger::getInstance();
        std::string log_str{};
        if (reg_id) {
            log_str = fmt::format("\tx{}: {:x} -> {:x}", reg_id, m_regfile[reg_id], value);
        }
        myLogger.message(Logger::severity_level::trace, "", log_str);

        m_regfile[reg_id] = value;
        m_regfile[0] = 0;
    }

    template <typename ValType>
    void load(virtual_address_t addr, uint64_t &value) const {
        static_assert(std::is_integral_v<ValType>);

        virtual_address_t addr_page_offset = addr & kPageSizeMask;
        if (addr_page_offset + sizeof(ValType) > kPageSize) {
            std::runtime_error("Misaligned memory load");
        }

        auto phys_addr = m_mmu.translate(addr, MMU::Mode::Bare);
        m_mem.load<ValType>(phys_addr, value);
    }

    template <typename ValType>
    void store(virtual_address_t addr, uint64_t value) {
        static_assert(std::is_integral_v<ValType>);

        virtual_address_t addr_page_offset = addr & kPageSizeMask;
        if (addr_page_offset + sizeof(ValType) > kPageSize) {
            std::runtime_error("Misaligned memory store");
        }

        Logger &myLogger = Logger::getInstance();

        std::string log_str{};
        log_str = fmt::format("\tDATA_MEM[{:04x}] = {:x}", addr, value);
        myLogger.message(Logger::severity_level::trace, "", log_str);

        auto phys_addr = m_mmu.translate(addr, MMU::Mode::Bare);
        m_mem.store<ValType>(phys_addr, value);
    }
};
}  // namespace sim
