//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2024
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "pxc/utils/port/config.h"

#ifdef TD_POLL_POLL

#include "pxc/utils/common.h"
#include "pxc/utils/port/detail/PollableFd.h"
#include "pxc/utils/port/PollBase.h"
#include "pxc/utils/port/PollFlags.h"

#include <poll.h>

namespace pxc {
namespace detail {

class Poll final : public PollBase {
 public:
  Poll() = default;
  Poll(const Poll &) = delete;
  Poll &operator=(const Poll &) = delete;
  Poll(Poll &&) = delete;
  Poll &operator=(Poll &&) = delete;
  ~Poll() final = default;

  void init() final;

  void clear() final;

  void subscribe(PollableFd fd, PollFlags flags) final;

  void unsubscribe(PollableFdRef fd) final;

  void unsubscribe_before_close(PollableFdRef fd) final;

  void run(int timeout_ms) final;

  static bool is_edge_triggered() {
    return false;
  }

 private:
  vector<pollfd> pollfds_;
  vector<PollableFd> fds_;
};

}  // namespace detail
}  // namespace pxc

#endif
