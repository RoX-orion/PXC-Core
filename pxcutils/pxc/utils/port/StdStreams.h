//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2024
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "pxc/utils/buffer.h"
#include "pxc/utils/common.h"
#include "pxc/utils/port/detail/PollableFd.h"
#include "pxc/utils/port/FileFd.h"
#include "pxc/utils/Status.h"

#include <limits>
#include <memory>

namespace pxc {

FileFd &Stdin();
FileFd &Stdout();
FileFd &Stderr();

namespace detail {
class BufferedStdinImpl;
class BufferedStdinImplDeleter {
 public:
  void operator()(BufferedStdinImpl *impl);
};
}  // namespace detail

class BufferedStdin {
 public:
  BufferedStdin();
  BufferedStdin(const BufferedStdin &) = delete;
  BufferedStdin &operator=(const BufferedStdin &) = delete;
  BufferedStdin(BufferedStdin &&) noexcept;
  BufferedStdin &operator=(BufferedStdin &&) noexcept;
  ~BufferedStdin();
  ChainBufferReader &input_buffer();
  PollableFdInfo &get_poll_info();
  const PollableFdInfo &get_poll_info() const;
  Result<size_t> flush_read(size_t max_read = std::numeric_limits<size_t>::max()) TD_WARN_UNUSED_RESULT;

 private:
  std::unique_ptr<detail::BufferedStdinImpl, detail::BufferedStdinImplDeleter> impl_;
};

}  // namespace pxc
