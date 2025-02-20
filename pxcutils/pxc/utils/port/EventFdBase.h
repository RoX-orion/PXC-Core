//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2024
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "pxc/utils/common.h"
#include "pxc/utils/port/detail/PollableFd.h"
#include "pxc/utils/Status.h"

namespace pxc {
class EventFdBase {
 public:
  EventFdBase() = default;
  EventFdBase(const EventFdBase &) = delete;
  EventFdBase &operator=(const EventFdBase &) = delete;
  EventFdBase(EventFdBase &&) = default;
  EventFdBase &operator=(EventFdBase &&) = default;
  virtual ~EventFdBase() = default;

  virtual void init() = 0;
  virtual bool empty() = 0;
  virtual void close() = 0;
  virtual PollableFdInfo &get_poll_info() = 0;
  virtual Status get_pending_error() TD_WARN_UNUSED_RESULT = 0;
  virtual void release() = 0;
  virtual void acquire() = 0;
  virtual void wait(int timeout_ms) = 0;
};
}  // namespace pxc
