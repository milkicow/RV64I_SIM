#pragma once

#include <sys/mman.h>

#include <cerrno>
#include <cstddef>
#include <stdexcept>
#include <string>

namespace memory {

class Memory {
   private:
    size_t m_size;
    std::byte *m_mem;

    static constexpr size_t default_mem_size = 0x100000; // 400kB 

   public:
    Memory(size_t size = default_mem_size) : m_size(size) {
        errno = 0;
        void *mmap_result =
            mmap(NULL, m_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (mmap_result == MAP_FAILED) {
            throw std::runtime_error("Can't mmap memory size of" + std::to_string(m_size) +
                                     "with errno: " + std::strerror(errno));
        }
        m_mem = static_cast<std::byte *>(mmap_result);
    }


    template<typename T>
    void load(uint64_t addr, uint64_t &value) const {
        // check alignment
        value = *reinterpret_cast<T*>(m_mem + addr);
    }

    template<typename T>
    void store(uint64_t addr, uint64_t value) {
        // check alignment
        *reinterpret_cast<T*>(m_mem + addr) = value;
    }
};

}  // namespace memory
