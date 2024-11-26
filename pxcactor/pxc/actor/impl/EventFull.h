//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2024
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "pxc/actor/impl/EventFull-decl.h"
#include "pxc/actor/impl/Scheduler-decl.h"

#include "pxc/utils/common.h"

#include <utility>

namespace pxc {

inline void EventFull::try_emit_later() {
  if (empty()) {
    return;
  }
  auto link_token = data_.link_token;
  send_event_later(ActorShared<>(actor_id_, link_token), std::move(data_));
  data_.clear();
  CHECK(empty());
}

inline void EventFull::try_emit() {
  if (empty()) {
    return;
  }
  auto link_token = data_.link_token;
  send_event(ActorShared<>(actor_id_, link_token), std::move(data_));
  data_.clear();
  CHECK(empty());
}

}  // namespace pxc
