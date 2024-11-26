//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2024
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "pxc/actor/actor.h"

#include "pxc/utils/common.h"
#include "pxc/utils/port/ServerSocketFd.h"
#include "pxc/utils/port/SocketFd.h"
#include "pxc/utils/Slice.h"

namespace pxc {

class TcpListener final : public Actor {
 public:
  class Callback : public Actor {
   public:
    virtual void accept(SocketFd fd) = 0;
  };

  TcpListener(int port, ActorShared<Callback> callback, Slice server_address = Slice("0.0.0.0"));
  void hangup() final;

 private:
  int port_;
  ServerSocketFd server_fd_;
  ActorShared<Callback> callback_;
  const string server_address_;
  void start_up() final;
  void tear_down() final;
  void loop() final;
};

}  // namespace pxc
