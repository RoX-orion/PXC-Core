//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2024
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "pxc/utils/buffer.h"
#include "pxc/utils/common.h"
#include "pxc/utils/SharedSlice.h"
#include "pxc/utils/Slice.h"
#include "pxc/utils/Status.h"

namespace pxc {

Result<BufferSlice> read_file(CSlice path, int64 size = -1, int64 offset = 0);
Result<string> read_file_str(CSlice path, int64 size = -1, int64 offset = 0);
Result<SecureString> read_file_secure(CSlice path, int64 size = -1, int64 offset = 0);

Status copy_file(CSlice from, CSlice to, int64 size = -1) TD_WARN_UNUSED_RESULT;

struct WriteFileOptions {
  bool need_sync = false;
  bool need_lock = true;
};
Status write_file(CSlice to, Slice data, WriteFileOptions options = {}) TD_WARN_UNUSED_RESULT;

string clean_filename(CSlice name);

// writes data to file and ensures that the file is either fully overridden, or is left intact
// uses path_tmp to temporary store data, then calls rename
Status atomic_write_file(CSlice path, Slice data, CSlice path_tmp = {});

}  // namespace pxc
