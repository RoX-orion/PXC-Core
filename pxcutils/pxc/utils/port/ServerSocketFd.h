//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2024
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "pxc/utils/port/detail/NativeFd.h"
#include "pxc/utils/port/detail/PollableFd.h"
#include "pxc/utils/port/SocketFd.h"

#include "pxc/utils/Slice.h"
#include "pxc/utils/Status.h"

#include <memory>

namespace pxc {
namespace detail {
class ServerSocketFdImpl;
class ServerSocketFdImplDeleter {
 public:
  void operator()(ServerSocketFdImpl *impl);
};
}  // namespace detail

class ServerSocketFd {
 public:
  ServerSocketFd();
  ServerSocketFd(const ServerSocketFd &) = delete;
  ServerSocketFd &operator=(const ServerSocketFd &) = delete;
  ServerSocketFd(ServerSocketFd &&) noexcept;
  ServerSocketFd &operator=(ServerSocketFd &&) noexcept;
  ~ServerSocketFd();

  static Result<ServerSocketFd> open(int32 port, CSlice addr = CSlice("0.0.0.0")) TD_WARN_UNUSED_RESULT;

  PollableFdInfo &get_poll_info();
  const PollableFdInfo &get_poll_info() const;

  Status get_pending_error() TD_WARN_UNUSED_RESULT;

  Result<SocketFd> accept() TD_WARN_UNUSED_RESULT;

  void close();
  bool empty() const;

  const NativeFd &get_native_fd() const;

 private:
  std::unique_ptr<detail::ServerSocketFdImpl, detail::ServerSocketFdImplDeleter> impl_;
  explicit ServerSocketFd(unique_ptr<detail::ServerSocketFdImpl> impl);
};
}  // namespace pxc
