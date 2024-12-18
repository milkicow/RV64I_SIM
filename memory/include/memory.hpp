#pragma once

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <set>
#include <stdexcept>
#include <string>
#include <type_traits>

#include "common_mem.hpp"
#include "fmt/format.h"
#include "logger.hpp"

namespace sim {

uint64_t my_bits(uint64_t value, size_t high_bit, size_t low_bit);

class Memory {  // PhysMemory
   private:
    size_t m_size;
    uint8_t *m_mem;

    size_t m_page_counter = 0;

    static constexpr size_t default_mem_size = 0x400000;  // 4 GB

   public:
    Memory(size_t size = default_mem_size) : m_size(size) {
        errno = 0;
        void *mmap_result =
            mmap(NULL, m_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (mmap_result == MAP_FAILED) {
            throw std::runtime_error{fmt::format("Can't mmap memory size of {}, errno: {}",
                                                 std::to_string(m_size), std::strerror(errno))};
        }
        m_mem = static_cast<uint8_t *>(mmap_result);

        // Logger &myLogger = Logger::getInstance();
        // myLogger.message(Logger::severity_level::standard, "Memory",
        //                  fmt::format("{} - {}", static_cast<void *>(m_mem),
        //                              static_cast<void *>(m_mem + m_size)));
    }

    uint8_t *get_mem() { return m_mem; }

    physical_address_t getPage() {
        return reinterpret_cast<physical_address_t>(m_mem + 0x1000 * m_page_counter++);
    }
    void reservePages(size_t page_num) { m_page_counter += page_num; }
    size_t getPageCounter() { return m_page_counter; }

    Memory(const Memory &memory) = delete;
    Memory &operator=(const Memory &memory) = delete;

    Memory(Memory &&memory) = delete;
    Memory &operator=(Memory &&memory) = delete;

    ~Memory() = default;

    template <typename ValType>
    void load(physical_address_t addr, uint64_t &value) const {
        static_assert(std::is_integral_v<ValType>);

        physical_address_t addr_page_offset = addr & kPageSizeMask;
        if (addr_page_offset + sizeof(ValType) > kPageSize) {
            std::runtime_error("Misaligned memory load");
        }

        value = *reinterpret_cast<ValType *>(m_mem + addr);
    }

    template <typename ValType>
    void store(physical_address_t addr, uint64_t value) {
        static_assert(std::is_integral_v<ValType>);

        physical_address_t addr_page_offset = addr & kPageSizeMask;
        if (addr_page_offset + sizeof(ValType) > kPageSize) {
            std::runtime_error("Misaligned memory store");
        }

        *reinterpret_cast<ValType *>(m_mem + addr) = value;
    }

    void store(size_t mem_offset, const void *src, size_t count) {
        std::memcpy(m_mem + mem_offset, src, count);
    }
};

[[nodiscard]] size_t getVPN(virtual_address_t virtual_address, size_t level);

class MMU final {
   private:
    Memory &m_phys_memory;
    reg_t m_satp_addr = 0x10;  // FIXME: implement system registers: satp, status
                               // status register &

    size_t m_vpn2_size = 0;
    size_t m_vpn1_size = 0;
    size_t m_vpn0_size = 0;

   public:
    enum class Mode {
        Bare,
        Sv39,
    };

   public:
    MMU(Memory &phys_memory) : m_phys_memory(phys_memory) {}

    MMU(const MMU &mmu) = delete;
    MMU &operator=(const MMU &mmu) = delete;

    MMU(MMU &&mmu) = delete;
    MMU &operator=(MMU &&mmu) = delete;

    ~MMU() = default;

    [[nodiscard]] physical_address_t translate(virtual_address_t virtual_address, Mode mode) const;
    [[nodiscard]] reg_t getSATP() const { return m_satp_addr; };

    void allocaPTEs(size_t vpn2_table_size, size_t vpn1_table_size, size_t vpn0_table_size);
    void fillPTEs(virtual_address_t virtual_address);
};

}  // namespace sim
