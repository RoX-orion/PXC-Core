//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2024
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "pxc/utils/check.h"

#include "pxc/utils/logging.h"
#include "pxc/utils/Slice.h"
#include "pxc/utils/SliceBuilder.h"

namespace pxc {
namespace detail {

void process_check_error(const char *message, const char *file, int line) {
  ::pxc::Logger(*log_interface, log_options, VERBOSITY_NAME(FATAL), Slice(file), line, Slice())
      << "Check `" << message << "` failed";
  ::pxc::process_fatal_error(PSLICE() << "Check `" << message << "` failed in " << file << " at " << line << '\n');
}

}  // namespace detail
}  // namespace pxc
