//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2024
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "pxc/utils/port/config.h"

#ifdef TD_POLL_KQUEUE

#include "pxc/utils/common.h"
#include "pxc/utils/List.h"
#include "pxc/utils/port/detail/NativeFd.h"
#include "pxc/utils/port/detail/PollableFd.h"
#include "pxc/utils/port/PollBase.h"
#include "pxc/utils/port/PollFlags.h"

#include <cstdint>

#include <sys/types.h>  // must be included before sys/event.h, which depends on sys/types.h on FreeBSD

#include <sys/event.h>

namespace pxc {
namespace detail {

class KQueue final : public PollBase {
 public:
  KQueue() = default;
  KQueue(const KQueue &) = delete;
  KQueue &operator=(const KQueue &) = delete;
  KQueue(KQueue &&) = delete;
  KQueue &operator=(KQueue &&) = delete;
  ~KQueue() final;

  void init() final;

  void clear() final;

  void subscribe(PollableFd fd, PollFlags flags) final;

  void unsubscribe(PollableFdRef fd) final;

  void unsubscribe_before_close(PollableFdRef fd) final;

  void run(int timeout_ms) final;

  static bool is_edge_triggered() {
    return true;
  }

 private:
  vector<struct kevent> events_;
  int changes_n_;
  NativeFd kq_;
  ListNode list_root_;

  int update(int nevents, const timespec *timeout, bool may_fail = false);

  void invalidate(int native_fd);

  void flush_changes(bool may_fail = false);

  void add_change(std::uintptr_t ident, int16 filter, uint16 flags, uint32 fflags, std::intptr_t data, void *udata);
};

}  // namespace detail
}  // namespace pxc

#endif
