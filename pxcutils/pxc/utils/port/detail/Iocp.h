//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2024
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "pxc/utils/port/config.h"

#ifdef TD_PORT_WINDOWS

#include "pxc/utils/common.h"
#include "pxc/utils/Context.h"
#include "pxc/utils/port/detail/NativeFd.h"
#include "pxc/utils/Status.h"

#include <memory>

namespace pxc {
namespace detail {

class IocpRef;
class Iocp final : public Context<Iocp> {
 public:
  Iocp() = default;
  Iocp(const Iocp &) = delete;
  Iocp &operator=(const Iocp &) = delete;
  Iocp(Iocp &&) = delete;
  Iocp &operator=(Iocp &&) = delete;
  ~Iocp();

  class Callback {
   public:
    virtual ~Callback() = default;
    virtual void on_iocp(Result<size_t> r_size, WSAOVERLAPPED *overlapped) = 0;
  };

  void init();
  void subscribe(const NativeFd &fd, Callback *callback);
  void post(size_t size, Callback *callback, WSAOVERLAPPED *overlapped);
  void loop();
  void interrupt_loop();
  void clear();

  IocpRef get_ref() const;

 private:
  std::shared_ptr<NativeFd> iocp_handle_;
};

class IocpRef {
 public:
  IocpRef() = default;
  IocpRef(const Iocp &) = delete;
  IocpRef &operator=(const Iocp &) = delete;
  IocpRef(IocpRef &&) = default;
  IocpRef &operator=(IocpRef &&) = default;

  explicit IocpRef(std::weak_ptr<NativeFd> iocp_handle);

  bool post(size_t size, Iocp::Callback *callback, WSAOVERLAPPED *overlapped);

 private:
  std::weak_ptr<NativeFd> iocp_handle_;
};

}  // namespace detail
}  // namespace pxc

#endif
