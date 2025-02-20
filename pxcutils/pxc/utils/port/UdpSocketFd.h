//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2024
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "pxc/utils/port/config.h"

#include "pxc/utils/buffer.h"
#include "pxc/utils/optional.h"
#include "pxc/utils/port/detail/NativeFd.h"
#include "pxc/utils/port/detail/PollableFd.h"
#include "pxc/utils/port/IPAddress.h"
#include "pxc/utils/Slice.h"
#include "pxc/utils/Span.h"
#include "pxc/utils/Status.h"

#include <memory>

namespace pxc {
namespace detail {
class UdpSocketFdImpl;
class UdpSocketFdImplDeleter {
 public:
  void operator()(UdpSocketFdImpl *impl);
};
}  // namespace detail

struct UdpMessage {
  IPAddress address;
  BufferSlice data;
  Status error;
};

class UdpSocketFd {
 public:
  UdpSocketFd();
  UdpSocketFd(UdpSocketFd &&) noexcept;
  UdpSocketFd &operator=(UdpSocketFd &&) noexcept;
  ~UdpSocketFd();

  UdpSocketFd(const UdpSocketFd &) = delete;
  UdpSocketFd &operator=(const UdpSocketFd &) = delete;

  Result<uint32> maximize_snd_buffer(uint32 max_size = 0);
  Result<uint32> maximize_rcv_buffer(uint32 max_size = 0);

  static Result<UdpSocketFd> open(const IPAddress &address) TD_WARN_UNUSED_RESULT;

  PollableFdInfo &get_poll_info();
  const PollableFdInfo &get_poll_info() const;
  const NativeFd &get_native_fd() const;

  void close();
  bool empty() const;

  static bool is_critical_read_error(const Status &status);

#if TD_PORT_POSIX
  struct OutboundMessage {
    const IPAddress *to;
    Slice data;
  };
  struct InboundMessage {
    IPAddress *from;
    MutableSlice data;
    Status *error;
  };

  Status send_message(const OutboundMessage &message, bool &is_sent) TD_WARN_UNUSED_RESULT;
  Status receive_message(InboundMessage &message, bool &is_received) TD_WARN_UNUSED_RESULT;

  Status send_messages(Span<OutboundMessage> messages, size_t &count) TD_WARN_UNUSED_RESULT;
  Status receive_messages(MutableSpan<InboundMessage> messages, size_t &count) TD_WARN_UNUSED_RESULT;
#elif TD_PORT_WINDOWS
  Result<optional<UdpMessage> > receive();

  void send(UdpMessage message);

  Status flush_send();
#endif

 private:
  static constexpr uint32 DEFAULT_UDP_MAX_SND_BUFFER_SIZE = (1 << 24);
  static constexpr uint32 DEFAULT_UDP_MAX_RCV_BUFFER_SIZE = (1 << 24);
  std::unique_ptr<detail::UdpSocketFdImpl, detail::UdpSocketFdImplDeleter> impl_;
  explicit UdpSocketFd(unique_ptr<detail::UdpSocketFdImpl> impl);
};

}  // namespace pxc
