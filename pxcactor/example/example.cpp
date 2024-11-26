//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2024
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "pxc/actor/actor.h"
#include "pxc/actor/ConcurrentScheduler.h"

#include "pxc/utils/logging.h"
#include "pxc/utils/Time.h"

class Worker final : public pxc::Actor {
 public:
  void ping(int x) {
    LOG(ERROR) << "Receive ping " << x;
  }
};

class MainActor final : public pxc::Actor {
 public:
  void start_up() final {
    LOG(ERROR) << "Start up";
    set_timeout_in(10);
    worker_ = pxc::create_actor_on_scheduler<Worker>("Worker", 1);
    send_closure(worker_, &Worker::ping, 123);
  }

  void timeout_expired() final {
    LOG(ERROR) << "Timeout expired";
    pxc::Scheduler::instance()->finish();
  }

 private:
  pxc::ActorOwn<Worker> worker_;
};

int main() {
  pxc::ConcurrentScheduler scheduler(4 /*thread_count*/, 0);
  scheduler.start();
  {
    auto guard = scheduler.get_main_guard();
    pxc::create_actor_on_scheduler<MainActor>("Main actor", 0).release();
  }
  while (!scheduler.is_finished()) {
    scheduler.run_main(pxc::Timestamp::in(10));
  }
  scheduler.finish();
}
