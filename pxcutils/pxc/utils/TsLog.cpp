//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2024
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "pxc/utils/TsLog.h"

#include "pxc/utils/ExitGuard.h"

namespace pxc {

void TsLog::enter_critical() {
  while (lock_.test_and_set(std::memory_order_acquire) && !ExitGuard::is_exited()) {
    // spin
  }
}

static ExitGuard exit_guard;

}  // namespace pxc
