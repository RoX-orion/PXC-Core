//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2024
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "pxc/utils/port/config.h"

#ifdef TD_POLL_EPOLL

#include "pxc/utils/common.h"
#include "pxc/utils/List.h"
#include "pxc/utils/port/detail/NativeFd.h"
#include "pxc/utils/port/detail/PollableFd.h"
#include "pxc/utils/port/PollBase.h"
#include "pxc/utils/port/PollFlags.h"

#include <sys/epoll.h>

namespace pxc {
namespace detail {

class Epoll final : public PollBase {
 public:
  Epoll() = default;
  Epoll(const Epoll &) = delete;
  Epoll &operator=(const Epoll &) = delete;
  Epoll(Epoll &&) = delete;
  Epoll &operator=(Epoll &&) = delete;
  ~Epoll() final = default;

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
  NativeFd epoll_fd_;
  vector<struct epoll_event> events_;
  ListNode list_root_;

};

}  // namespace detail
}  // namespace pxc

#endif
