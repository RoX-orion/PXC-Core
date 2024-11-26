//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2024
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "pxc/utils/common.h"
#include "pxc/utils/logging.h"
#include "pxc/utils/MpscPollableQueue.h"
#include "pxc/utils/port/thread.h"
#include "pxc/utils/Slice.h"
#include "pxc/utils/Status.h"

namespace pxc {

#if !TD_THREAD_UNSUPPORTED

class AsyncFileLog final : public LogInterface {
 public:
  AsyncFileLog() = default;
  AsyncFileLog(const AsyncFileLog &) = delete;
  AsyncFileLog &operator=(const AsyncFileLog &) = delete;
  AsyncFileLog(AsyncFileLog &&) = delete;
  AsyncFileLog &operator=(AsyncFileLog &&) = delete;
  ~AsyncFileLog();

  Status init(string path, int64 rotate_threshold, bool redirect_stderr = true);

 private:
  struct Query {
    enum class Type : int32 { Log, AfterRotation, Close };
    Type type_ = Type::Log;
    string data_;
  };

  string path_;
  unique_ptr<MpscPollableQueue<Query>> queue_;
  thread logging_thread_;

  vector<string> get_file_paths() final;

  void after_rotation() final;

  void do_append(int log_level, CSlice slice) final;
};

#endif

}  // namespace pxc
