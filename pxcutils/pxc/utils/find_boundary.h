//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2024
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "pxc/utils/buffer.h"
#include "pxc/utils/common.h"
#include "pxc/utils/Slice.h"

namespace pxc {

bool find_boundary(ChainBufferReader range, Slice boundary, size_t &already_read);

}  // namespace pxc
