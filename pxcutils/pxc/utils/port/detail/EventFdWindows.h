//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2024
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "pxc/utils/port/config.h"

#ifdef TD_EVENTFD_WINDOWS

#include "pxc/utils/common.h"
#include "pxc/utils/port/detail/NativeFd.h"
#include "pxc/utils/port/detail/PollableFd.h"
#include "pxc/utils/port/EventFdBase.h"
#include "pxc/utils/Status.h"

namespace pxc {
namespace detail {

class EventFdWindows final : public EventFdBase {
  NativeFd event_;

 public:
  EventFdWindows() = default;

  void init() final;

  bool empty() final;

  void close() final;

  Status get_pending_error() final TD_WARN_UNUSED_RESULT;

  PollableFdInfo &get_poll_info() final;

  void release() final;

  void acquire() final;

  void wait(int timeout_ms) final;
};

}  // namespace detail
}  // namespace pxc

#endif
