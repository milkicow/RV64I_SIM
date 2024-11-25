#pragma once

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <cerrno>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>
#include <cstdint>
#include <cstring>

#include "fmt/format.h"
#include "logger.hpp"

namespace memory {

class Memory {
   private:
    size_t m_size;
    uint8_t *m_mem;

    static constexpr size_t default_mem_size = 0x100000;  // 1 GB

   public:
    Memory(size_t size = default_mem_size) : m_size(size) {
        errno = 0;
        void *mmap_result =
            mmap(NULL, m_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (mmap_result == MAP_FAILED) {
            throw std::runtime_error{fmt::format("Can't mmap memory size of {} with errno: {}",
                                                 std::to_string(m_size), std::strerror(errno))};
        }
        m_mem = static_cast<uint8_t *>(mmap_result);

        Logger &myLogger = Logger::getInstance();
        myLogger.message(Logger::severity_level::standard, "Memory",
                         fmt::format("{} - {}", static_cast<void *>(m_mem),
                                     static_cast<void *>(m_mem + m_size)));
    }

    template <typename ValType>
    void load(uint64_t addr, uint64_t &value) const {
        // check alignment
        value = *reinterpret_cast<ValType *>(m_mem + addr);
    }

    template <typename ValType>
    void store(uint64_t addr, uint64_t value) {
        // check alignment
        *reinterpret_cast<ValType *>(m_mem + addr) = value;
    }

    void store(size_t mem_offset, const void *src, size_t count) {
        std::memcpy(m_mem + mem_offset, src, count);
    }
};

}  // namespace memory
