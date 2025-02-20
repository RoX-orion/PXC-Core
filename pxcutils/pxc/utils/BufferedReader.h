//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2024
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "pxc/utils/common.h"
#include "pxc/utils/port/FileFd.h"
#include "pxc/utils/Slice.h"
#include "pxc/utils/Status.h"

namespace pxc {

class BufferedReader {
 public:
  explicit BufferedReader(FileFd &file, size_t buff_size = 8152)
      : file_(file), buff_(buff_size), begin_pos_(0), end_pos_(0) {
  }

  Result<size_t> read(MutableSlice slice) TD_WARN_UNUSED_RESULT;

 private:
  FileFd &file_;
  vector<char> buff_;
  size_t begin_pos_;
  size_t end_pos_;
};

inline Result<size_t> BufferedReader::read(MutableSlice slice) {
  size_t available = end_pos_ - begin_pos_;
  if (available >= slice.size()) {
    // have enough data in buffer
    slice.copy_from({&buff_[begin_pos_], slice.size()});
    begin_pos_ += slice.size();
    return slice.size();
  }

  if (available) {
    slice.copy_from({&buff_[begin_pos_], available});
    begin_pos_ += available;
    slice.remove_prefix(available);
  }

  if (slice.size() > buff_.size() / 2) {
    TRY_RESULT(result, file_.read(slice));
    return result + available;
  }

  TRY_RESULT(result, file_.read({&buff_[0], buff_.size()}));
  begin_pos_ = 0;
  end_pos_ = result;

  size_t left = min(end_pos_, slice.size());
  slice.copy_from({&buff_[0], left});
  begin_pos_ = left;
  return left + available;
}

}  // namespace pxc
