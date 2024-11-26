//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2024
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "pxc/actor/actor.h"
#include "pxc/actor/ConcurrentScheduler.h"
#include "pxc/actor/MultiTimeout.h"

#include "pxc/utils/common.h"
#include "pxc/utils/logging.h"
#include "pxc/utils/Random.h"
#include "pxc/utils/tests.h"

TEST(MultiTimeout, bug) {
  pxc::ConcurrentScheduler sched(0, 0);

  sched.start();
  pxc::unique_ptr<pxc::MultiTimeout> multi_timeout;
  struct Data {
    pxc::MultiTimeout *multi_timeout;
  };
  Data data;

  {
    auto guard = sched.get_main_guard();
    multi_timeout = pxc::make_unique<pxc::MultiTimeout>("MultiTimeout");
    data.multi_timeout = multi_timeout.get();
    multi_timeout->set_callback([](void *void_data, pxc::int64 key) {
      auto &data = *static_cast<Data *>(void_data);
      if (key == 1) {
        data.multi_timeout->cancel_timeout(key + 1);
        data.multi_timeout->set_timeout_in(key + 2, 1);
      } else {
        pxc::Scheduler::instance()->finish();
      }
    });
    multi_timeout->set_callback_data(&data);
    multi_timeout->set_timeout_in(1, 1);
    multi_timeout->set_timeout_in(2, 2);
  }

  while (sched.run_main(10)) {
    // empty
  }
  sched.finish();
}

class TimeoutManager final : public pxc::Actor {
  static pxc::int32 count;

 public:
  TimeoutManager() {
    count++;

    test_timeout_.set_callback(on_test_timeout_callback);
    test_timeout_.set_callback_data(static_cast<void *>(this));
  }
  TimeoutManager(const TimeoutManager &) = delete;
  TimeoutManager &operator=(const TimeoutManager &) = delete;
  TimeoutManager(TimeoutManager &&) = delete;
  TimeoutManager &operator=(TimeoutManager &&) = delete;
  ~TimeoutManager() final {
    count--;
    LOG(INFO) << "Destroy TimeoutManager";
  }

  static void on_test_timeout_callback(void *timeout_manager_ptr, pxc::int64 id) {
    CHECK(count >= 0);
    if (count == 0) {
      LOG(ERROR) << "Receive timeout after manager was closed";
      return;
    }

    auto manager = static_cast<TimeoutManager *>(timeout_manager_ptr);
    send_closure_later(manager->actor_id(manager), &TimeoutManager::test_timeout);
  }

  void test_timeout() {
    CHECK(count > 0);
    // we must yield scheduler, so run_main breaks immediately, if timeouts are handled immediately
    pxc::Scheduler::instance()->yield();
  }

  pxc::MultiTimeout test_timeout_{"TestTimeout"};
};

pxc::int32 TimeoutManager::count;

TEST(MultiTimeout, Destroy) {
  pxc::ConcurrentScheduler sched(0, 0);

  auto timeout_manager = sched.create_actor_unsafe<TimeoutManager>(0, "TimeoutManager");
  TimeoutManager *manager = timeout_manager.get().get_actor_unsafe();
  sched.start();
  int cnt = 100;
  while (sched.run_main(cnt == 100 || cnt <= 0 ? 0.001 : 10)) {
    auto guard = sched.get_main_guard();
    cnt--;
    if (cnt > 0) {
      for (int i = 0; i < 2; i++) {
        manager->test_timeout_.set_timeout_in(pxc::Random::fast(0, 1000000000), pxc::Random::fast(2, 5) / 1000.0);
      }
    } else if (cnt == 0) {
      timeout_manager.reset();
    } else if (cnt == -10) {
      pxc::Scheduler::instance()->finish();
    }
  }
  sched.finish();
}
