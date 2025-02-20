//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2024
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "pxc/actor/actor.h"

#include "pxc/utils/common.h"
#include "pxc/utils/Promise.h"

namespace pxc {

class SleepActor final : public Actor {
 public:
  SleepActor(double timeout, Promise<> promise) : timeout_(timeout), promise_(std::move(promise)) {
  }

 private:
  double timeout_;
  Promise<> promise_;

  void start_up() final {
    set_timeout_in(timeout_);
  }
  void timeout_expired() final {
    promise_.set_value(Unit());
    stop();
  }
};

template <>
class ActorTraits<SleepActor> {
 public:
  static constexpr bool need_context = false;
  static constexpr bool need_start_up = true;
};

}  // namespace pxc
