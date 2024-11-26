//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2024
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "pxc/utils/common.h"
#include "pxc/utils/Slice.h"
#include "pxc/utils/StackAllocator.h"
#include "pxc/utils/StringBuilder.h"

#define PSLICE() ::pxc::detail::Slicify() & ::pxc::SliceBuilder().ref()
#define PSTRING() ::pxc::detail::Stringify() & ::pxc::SliceBuilder().ref()

namespace pxc {

class SliceBuilder {
 public:
  template <class T>
  SliceBuilder &operator<<(T &&other) {
    sb_ << other;
    return *this;
  }

  MutableCSlice as_cslice() {
    return sb_.as_cslice();
  }

  SliceBuilder &ref() {
    return *this;
  }

 private:
  static const size_t DEFAULT_BUFFER_SIZE = 1024;
  decltype(StackAllocator::alloc(0)) buffer_ = StackAllocator::alloc(DEFAULT_BUFFER_SIZE);
  StringBuilder sb_ = StringBuilder(buffer_.as_slice(), true);
};

namespace detail {
class Slicify {
 public:
  CSlice operator&(SliceBuilder &slice_builder) {
    return slice_builder.as_cslice();
  }
};

class Stringify {
 public:
  string operator&(SliceBuilder &slice_builder) {
    return slice_builder.as_cslice().str();
  }
};
}  // namespace detail

}  // namespace pxc
