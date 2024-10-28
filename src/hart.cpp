#include "hart.hpp"

namespace hart {

uint64_t Hart::get_pc() const noexcept { return m_pc; }
uint64_t Hart::get_pc_next() const noexcept { return m_pc_next; }
reg_t Hart::get_reg(reg_id_t reg_id) const { return m_regfile[reg_id]; }

void Hart::set_next_pc(uint64_t pc_next) noexcept { m_pc_next = pc_next; }
void Hart::set_reg(reg_id_t reg_id, reg_t value) {
    m_regfile[reg_id] = value;
    m_regfile[0] = 0;
};

}  // namespace hart
