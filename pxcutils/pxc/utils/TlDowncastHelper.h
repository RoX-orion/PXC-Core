//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2024
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "pxc/utils/common.h"
#include "pxc/utils/TlStorerToString.h"

namespace pxc {

template <class T>
class TlDowncastHelper final : public T {
 public:
  explicit TlDowncastHelper(int32 constructor) : constructor_(constructor) {
  }
  int32 get_id() const final {
    return constructor_;
  }
  void store(TlStorerToString &s, const char *field_name) const final {
  }

 private:
  int32 constructor_{0};
};

}  // namespace pxc
