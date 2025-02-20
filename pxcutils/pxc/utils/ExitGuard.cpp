//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2024
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "pxc/utils/ExitGuard.h"

#include "pxc/utils/logging.h"

namespace pxc {

std::atomic<bool> ExitGuard::is_exited_{false};

ExitGuard::~ExitGuard() {
  is_exited_.store(true, std::memory_order_relaxed);
  set_verbosity_level(VERBOSITY_NAME(FATAL));
}

}  // namespace pxc
