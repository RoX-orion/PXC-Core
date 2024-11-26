//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2024
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "pxc/utils/FlatHashTable.h"

#include "pxc/utils/bits.h"
#include "pxc/utils/Random.h"

namespace pxc {
namespace detail {

uint32 normalize_flat_hash_table_size(uint32 size) {
  return pxc::max(static_cast<uint32>(1) << (32 - count_leading_zeroes32(size)), static_cast<uint32>(8));
}

uint32 get_random_flat_hash_table_bucket(uint32 bucket_count_mask) {
  return Random::fast_uint32() & bucket_count_mask;
}

}  // namespace detail
}  // namespace pxc
