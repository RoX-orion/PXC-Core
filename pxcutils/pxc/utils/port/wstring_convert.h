//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2024
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "pxc/utils/port/config.h"

#if TD_PORT_WINDOWS

#include "pxc/utils/common.h"
#include "pxc/utils/Slice.h"
#include "pxc/utils/Status.h"

#include <string>

namespace pxc {

Result<std::wstring> to_wstring(CSlice slice);

Result<string> from_wstring(const std::wstring &str);

Result<string> from_wstring(const wchar_t *begin, size_t size);

Result<string> from_wstring(const wchar_t *begin);

}  // namespace pxc

#endif
