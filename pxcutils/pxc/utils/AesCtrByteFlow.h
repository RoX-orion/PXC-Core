//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2024
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "pxc/utils/ByteFlow.h"
#include "pxc/utils/common.h"
#include "pxc/utils/crypto.h"
#include "pxc/utils/Slice.h"
#include "pxc/utils/Status.h"
#include "pxc/utils/UInt.h"

namespace pxc {

#if TD_HAVE_OPENSSL
class AesCtrByteFlow final : public ByteFlowInplaceBase {
 public:
  void init(const UInt256 &key, const UInt128 &iv) {
    state_.init(as_slice(key), as_slice(iv));
  }
  void init(AesCtrState &&state) {
    state_ = std::move(state);
  }
  AesCtrState move_aes_ctr_state() {
    return std::move(state_);
  }
  bool loop() final {
    bool result = false;
    auto ready = input_->prepare_read();
    if (!ready.empty()) {
      state_.encrypt(ready, MutableSlice(const_cast<char *>(ready.data()), ready.size()));
      input_->confirm_read(ready.size());
      output_.advance_end(ready.size());
      result = true;
    }

    if (!is_input_active_) {
      finish(Status::OK());  // End of input stream.
    }
    return result;
  }

 private:
  AesCtrState state_;
};
#endif

}  // namespace pxc
