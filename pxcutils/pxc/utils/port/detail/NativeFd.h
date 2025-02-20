//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2024
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "pxc/utils/port/config.h"

#include "pxc/utils/common.h"
#include "pxc/utils/logging.h"
#include "pxc/utils/Status.h"
#include "pxc/utils/StringBuilder.h"

namespace pxc {

extern int VERBOSITY_NAME(fd);

class NativeFd {
 public:
#if TD_PORT_POSIX
  using Fd = int;
  using Socket = int;
#elif TD_PORT_WINDOWS
  using Fd = HANDLE;
  using Socket = SOCKET;
#endif
  NativeFd() = default;
  explicit NativeFd(Fd fd);
  NativeFd(Fd fd, bool nolog);
#if TD_PORT_WINDOWS
  explicit NativeFd(Socket socket);
#endif
  NativeFd(const NativeFd &) = delete;
  NativeFd &operator=(const NativeFd &) = delete;
  NativeFd(NativeFd &&other) noexcept;
  NativeFd &operator=(NativeFd &&other) noexcept;
  ~NativeFd();

  explicit operator bool() const noexcept;

  Fd fd() const;
  Socket socket() const;

  Status set_is_blocking(bool is_blocking) const;

  Status set_is_blocking_unsafe(bool is_blocking) const;  // may drop other Fd flags on non-Windows

  Status duplicate(const NativeFd &to) const;

  void close();
  Fd release();

  Status validate() const;

 private:
  static Fd empty_fd();

  Fd fd_ = empty_fd();
#if TD_PORT_WINDOWS
  bool is_socket_{false};
#endif
};

StringBuilder &operator<<(StringBuilder &sb, const NativeFd &fd);

}  // namespace pxc
