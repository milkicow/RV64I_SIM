#pragma once

#include <array>

#include "memory.hpp"
#include "regfile.hpp"

namespace hart {

constexpr size_t g_regfile_size = 32;

using addr_t = uint64_t;


using reg_t = addr_t;
using signed_reg_t = int64_t;

using reg_id_t = uint32_t;  // TODO: add GPRegId enum class with regs names

class Hart final {
   private:
    memory::Memory m_mem{};

    addr_t m_pc, m_pc_next;
    std::array<reg_t, g_regfile_size> m_regfile{};

   public:
    Hart(addr_t start_pc = 0) : m_pc(start_pc), m_pc_next(start_pc) {}

    addr_t get_pc() const noexcept;
    addr_t get_pc_next() const noexcept;
    reg_t get_reg(reg_id_t reg_id) const;

    void set_next_pc(addr_t pc_next) noexcept;
    void set_reg(reg_id_t reg_id, reg_t value);

    template <typename T>
    void load(uint64_t addr, uint64_t &value) const {
        m_mem.load<T>(addr, value);
    }

    template <typename T>
    void store(uint64_t addr, uint64_t value) {
        m_mem.store<T>(addr, value);
    }
};
}  // namespace hart
