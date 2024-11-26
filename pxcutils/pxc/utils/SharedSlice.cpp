//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2024
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "pxc/utils/SharedSlice.h"

#include "pxc/utils/buffer.h"

namespace pxc {

BufferSlice SharedSlice::clone_as_buffer_slice() const {
  return BufferSlice{as_slice()};
}

}  // namespace pxc
