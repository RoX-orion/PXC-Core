//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2024
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "pxc/utils/port/config.h"

#include "pxc/utils/port/detail/ThreadPthread.h"
#include "pxc/utils/port/detail/ThreadStl.h"

namespace pxc {

// clang-format off

#if TD_THREAD_PTHREAD
  using thread = detail::ThreadPthread;
  namespace this_thread = detail::this_thread_pthread;
#elif TD_THREAD_STL
  using thread = detail::ThreadStl;
  namespace this_thread = detail::this_thread_stl;
#elif TD_THREAD_UNSUPPORTED
#else
  #error "Thread's implementation is not defined"
#endif

// clang-format on

}  // namespace pxc
