//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2024
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

//#include "pxc/pxcutils/FlatHashMapChunks.h"
#include "pxc/utils/FlatHashTable.h"
#include "pxc/utils/HashTableUtils.h"
#include "pxc/utils/SetNode.h"

#include <functional>
//#include <unordered_set>

namespace pxc {

template <class KeyT, class HashT = Hash<KeyT>, class EqT = std::equal_to<KeyT>>
using FlatHashSet = FlatHashTable<SetNode<KeyT, EqT>, HashT, EqT>;
//using FlatHashSet = FlatHashSetChunks<KeyT, HashT, EqT>;
//using FlatHashSet = std::unordered_set<KeyT, HashT, EqT>;

}  // namespace pxc
