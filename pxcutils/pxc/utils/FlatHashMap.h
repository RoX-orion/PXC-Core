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
#include "pxc/utils/MapNode.h"

#include <functional>
//#include <unordered_map>

namespace pxc {

template <class KeyT, class ValueT, class HashT = Hash<KeyT>, class EqT = std::equal_to<KeyT>>
using FlatHashMap = FlatHashTable<MapNode<KeyT, ValueT, EqT>, HashT, EqT>;
//using FlatHashMap = FlatHashMapChunks<KeyT, ValueT, HashT, EqT>;
//using FlatHashMap = std::unordered_map<KeyT, ValueT, HashT, EqT>;

}  // namespace pxc
