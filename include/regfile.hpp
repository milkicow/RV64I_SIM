#pragma once

#include <array>

namespace regfile {

constexpr size_t g_regfile_size = 32;

using reg_t = uint64_t;

class RegFile final {
   private:
    using regfile_t = std::array<reg_t, g_regfile_size>;
    regfile_t m_regfile{};

   public:
    using regfile_const_iterator = regfile_t::const_iterator;
    using regfile_iterator = regfile_t::iterator;

    regfile_const_iterator cbegin() const { return m_regfile.cbegin(); }
    regfile_const_iterator cend() const { return m_regfile.cend(); }

    regfile_const_iterator begin() const { return cbegin(); }
    regfile_const_iterator end() const { return cend(); }

    regfile_iterator begin() { return m_regfile.begin(); }
    regfile_iterator end() { return m_regfile.end(); }

    const reg_t& operator[](size_t n) const { return m_regfile[n]; }

    reg_t& operator[](size_t n) { return m_regfile[n]; }
};

}  // namespace regfile
