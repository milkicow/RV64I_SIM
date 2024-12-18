#pragma once
#include <cstddef>
#include <cstdint>
#include <assert.h>

namespace sim {

using addr_t = uint64_t;

using reg_t = addr_t;
using signed_reg_t = int64_t;

using reg_id_t = uint32_t;  // TODO: add GPRegId enum class with regs names

constexpr size_t g_regfile_size = 32;

using virtual_address_t = uint64_t;

using host_address_t = uint64_t;
using host_address_t = uint64_t;
using host_address_ptr_t = uint8_t *;

using physical_address_t = host_address_t;

constexpr size_t kPageBitSize = 12;
constexpr size_t kPageSize = 1 << kPageBitSize;
constexpr size_t kPageSizeMask = kPageSize - 1;

constexpr size_t kVPNBitSize = 9;

} // namespace sim