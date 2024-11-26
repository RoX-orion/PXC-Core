//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2024
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "pxc/actor/actor.h"
#include "pxc/actor/ConcurrentScheduler.h"
#include "pxc/actor/MultiPromise.h"
#include "pxc/actor/PromiseFuture.h"
#include "pxc/actor/SleepActor.h"

#include "pxc/utils/common.h"
#include "pxc/utils/logging.h"
#include "pxc/utils/MpscPollableQueue.h"
#include "pxc/utils/Observer.h"
#include "pxc/utils/port/FileFd.h"
#include "pxc/utils/port/path.h"
#include "pxc/utils/port/thread.h"
#include "pxc/utils/Promise.h"
#include "pxc/utils/Slice.h"
#include "pxc/utils/Status.h"
#include "pxc/utils/StringBuilder.h"
#include "pxc/utils/tests.h"
#include "pxc/utils/Time.h"

#include <memory>
#include <tuple>

static const size_t BUF_SIZE = 1024 * 1024;
static char buf[BUF_SIZE];
static char buf2[BUF_SIZE];
static pxc::StringBuilder sb(pxc::MutableSlice(buf, BUF_SIZE - 1));
static pxc::StringBuilder sb2(pxc::MutableSlice(buf2, BUF_SIZE - 1));

static pxc::vector<std::shared_ptr<pxc::MpscPollableQueue<pxc::EventFull>>> create_queues() {
#if TD_THREAD_UNSUPPORTED || TD_EVENTFD_UNSUPPORTED
  return {};
#else
  auto res = std::make_shared<pxc::MpscPollableQueue<pxc::EventFull>>();
  res->init();
  return {res};
#endif
}

TEST(Actors, SendLater) {
  sb.clear();
  pxc::Scheduler scheduler;
  scheduler.init(0, create_queues(), nullptr);

  auto guard = scheduler.get_guard();
  class Worker final : public pxc::Actor {
   public:
    void f() {
      sb << "A";
    }
  };
  auto id = pxc::create_actor<Worker>("Worker");
  scheduler.run_no_guard(pxc::Timestamp::in(1));
  pxc::send_closure(id, &Worker::f);
  pxc::send_closure_later(id, &Worker::f);
  pxc::send_closure(id, &Worker::f);
  ASSERT_STREQ("A", sb.as_cslice().c_str());
  scheduler.run_no_guard(pxc::Timestamp::in(1));
  ASSERT_STREQ("AAA", sb.as_cslice().c_str());
}

class X {
 public:
  X() {
    sb << "[cnstr_default]";
  }
  X(const X &) {
    sb << "[cnstr_copy]";
  }
  X(X &&) noexcept {
    sb << "[cnstr_move]";
  }
  X &operator=(const X &) {
    sb << "[set_copy]";
    return *this;
  }
  X &operator=(X &&) noexcept {
    sb << "[set_move]";
    return *this;
  }
  ~X() = default;
};

class XReceiver final : public pxc::Actor {
 public:
  void by_const_ref(const X &) {
    sb << "[by_const_ref]";
  }
  void by_lvalue_ref(const X &) {
    sb << "[by_lvalue_ref]";
  }
  void by_value(X) {
    sb << "[by_value]";
  }
};

TEST(Actors, simple_pass_event_arguments) {
  pxc::Scheduler scheduler;
  scheduler.init(0, create_queues(), nullptr);

  auto guard = scheduler.get_guard();
  auto id = pxc::create_actor<XReceiver>("XR").release();
  scheduler.run_no_guard(pxc::Timestamp::in(1));

  X x;

  // check tuple
  // std::tuple<X> tx;
  // sb.clear();
  // std::tuple<X> ty(std::move(tx));
  // tx = std::move(ty);
  // ASSERT_STREQ("[cnstr_move]", sb.as_cslice().c_str());

  // Send temporary object

  // Tmp-->ConstRef
  sb.clear();
  pxc::send_closure(id, &XReceiver::by_const_ref, X());
  ASSERT_STREQ("[cnstr_default][by_const_ref]", sb.as_cslice().c_str());

  // Tmp-->ConstRef (Delayed)
  sb.clear();
  pxc::send_closure_later(id, &XReceiver::by_const_ref, X());
  scheduler.run_no_guard(pxc::Timestamp::in(1));
  // LOG(ERROR) << sb.as_cslice();
  ASSERT_STREQ("[cnstr_default][cnstr_move][by_const_ref]", sb.as_cslice().c_str());

  // Tmp-->LvalueRef
  sb.clear();
  pxc::send_closure(id, &XReceiver::by_lvalue_ref, X());
  ASSERT_STREQ("[cnstr_default][by_lvalue_ref]", sb.as_cslice().c_str());

  // Tmp-->LvalueRef (Delayed)
  sb.clear();
  pxc::send_closure_later(id, &XReceiver::by_lvalue_ref, X());
  scheduler.run_no_guard(pxc::Timestamp::in(1));
  ASSERT_STREQ("[cnstr_default][cnstr_move][by_lvalue_ref]", sb.as_cslice().c_str());

  // Tmp-->Value
  sb.clear();
  pxc::send_closure(id, &XReceiver::by_value, X());
  ASSERT_STREQ("[cnstr_default][cnstr_move][by_value]", sb.as_cslice().c_str());

  // Tmp-->Value (Delayed)
  sb.clear();
  pxc::send_closure_later(id, &XReceiver::by_value, X());
  scheduler.run_no_guard(pxc::Timestamp::in(1));
  ASSERT_STREQ("[cnstr_default][cnstr_move][cnstr_move][by_value]", sb.as_cslice().c_str());

  // Var-->ConstRef
  sb.clear();
  pxc::send_closure(id, &XReceiver::by_const_ref, x);
  ASSERT_STREQ("[by_const_ref]", sb.as_cslice().c_str());

  // Var-->ConstRef (Delayed)
  sb.clear();
  pxc::send_closure_later(id, &XReceiver::by_const_ref, x);
  scheduler.run_no_guard(pxc::Timestamp::in(1));
  ASSERT_STREQ("[cnstr_copy][by_const_ref]", sb.as_cslice().c_str());

  // Var-->LvalueRef
  // Var-->LvalueRef (Delayed)
  // CE or strange behaviour

  // Var-->Value
  sb.clear();
  pxc::send_closure(id, &XReceiver::by_value, x);
  ASSERT_STREQ("[cnstr_copy][by_value]", sb.as_cslice().c_str());

  // Var-->Value (Delayed)
  sb.clear();
  pxc::send_closure_later(id, &XReceiver::by_value, x);
  scheduler.run_no_guard(pxc::Timestamp::in(1));
  ASSERT_STREQ("[cnstr_copy][cnstr_move][by_value]", sb.as_cslice().c_str());
}

class PrintChar final : public pxc::Actor {
 public:
  PrintChar(char c, int cnt) : char_(c), cnt_(cnt) {
  }
  void start_up() final {
    yield();
  }
  void wakeup() final {
    if (cnt_ == 0) {
      stop();
    } else {
      sb << char_;
      cnt_--;
      yield();
    }
  }

 private:
  char char_;
  int cnt_;
};

//
// Yield must add actor to the end of queue
//
TEST(Actors, simple_hand_yield) {
  pxc::Scheduler scheduler;
  scheduler.init(0, create_queues(), nullptr);
  sb.clear();
  int cnt = 1000;
  {
    auto guard = scheduler.get_guard();
    pxc::create_actor<PrintChar>("PrintA", 'A', cnt).release();
    pxc::create_actor<PrintChar>("PrintB", 'B', cnt).release();
    pxc::create_actor<PrintChar>("PrintC", 'C', cnt).release();
  }
  scheduler.run(pxc::Timestamp::in(1));
  pxc::string expected;
  for (int i = 0; i < cnt; i++) {
    expected += "ABC";
  }
  ASSERT_STREQ(expected.c_str(), sb.as_cslice().c_str());
}

class Ball {
 public:
  friend void start_migrate(Ball &ball, pxc::int32 sched_id) {
    sb << "start";
  }
  friend void finish_migrate(Ball &ball) {
    sb2 << "finish";
  }
};

class Pong final : public pxc::Actor {
 public:
  void pong(Ball ball) {
    pxc::Scheduler::instance()->finish();
  }
};

class Ping final : public pxc::Actor {
 public:
  explicit Ping(pxc::ActorId<Pong> pong) : pong_(pong) {
  }
  void start_up() final {
    pxc::send_closure(pong_, &Pong::pong, Ball());
  }

 private:
  pxc::ActorId<Pong> pong_;
};

TEST(Actors, simple_migrate) {
  sb.clear();
  sb2.clear();

  pxc::ConcurrentScheduler scheduler(2, 0);
  auto pong = scheduler.create_actor_unsafe<Pong>(2, "Pong").release();
  scheduler.create_actor_unsafe<Ping>(1, "Ping", pong).release();
  scheduler.start();
  while (scheduler.run_main(10)) {
  }
  scheduler.finish();
#if TD_THREAD_UNSUPPORTED || TD_EVENTFD_UNSUPPORTED
  ASSERT_STREQ("", sb.as_cslice().c_str());
  ASSERT_STREQ("", sb2.as_cslice().c_str());
#else
  ASSERT_STREQ("start", sb.as_cslice().c_str());
  ASSERT_STREQ("finish", sb2.as_cslice().c_str());
#endif
}

class OpenClose final : public pxc::Actor {
 public:
  explicit OpenClose(int cnt) : cnt_(cnt) {
  }
  void start_up() final {
    yield();
  }
  void wakeup() final {
    auto observer = reinterpret_cast<pxc::ObserverBase *>(123);
    pxc::CSlice file_name = "server";
    if (cnt_ > 0) {
      auto r_file_fd = pxc::FileFd::open(file_name, pxc::FileFd::Read | pxc::FileFd::Create);
      LOG_CHECK(r_file_fd.is_ok()) << r_file_fd.error();
      auto file_fd = r_file_fd.move_as_ok();
      { auto pollable_fd = file_fd.get_poll_info().extract_pollable_fd(observer); }
      file_fd.close();
      cnt_--;
      yield();
    } else {
      pxc::Scheduler::instance()->finish();
    }
  }

 private:
  int cnt_;
};

TEST(Actors, open_close) {
  pxc::ConcurrentScheduler scheduler(2, 0);
  int cnt = 10000;  // TODO(perf) optimize
  scheduler.create_actor_unsafe<OpenClose>(1, "A", cnt).release();
  scheduler.create_actor_unsafe<OpenClose>(2, "B", cnt).release();
  scheduler.start();
  while (scheduler.run_main(10)) {
  }
  scheduler.finish();
  pxc::unlink("server").ignore();
}

class MsgActor : public pxc::Actor {
 public:
  virtual void msg() = 0;
};

class Slave final : public pxc::Actor {
 public:
  pxc::ActorId<MsgActor> msg;
  explicit Slave(pxc::ActorId<MsgActor> msg) : msg(msg) {
  }
  void hangup() final {
    pxc::send_closure(msg, &MsgActor::msg);
  }
};

class MasterActor final : public MsgActor {
 public:
  void loop() final {
    alive_ = true;
    slave = pxc::create_actor<Slave>("Slave", static_cast<pxc::ActorId<MsgActor>>(actor_id(this)));
    stop();
  }
  pxc::ActorOwn<Slave> slave;

  MasterActor() = default;
  MasterActor(const MasterActor &) = delete;
  MasterActor &operator=(const MasterActor &) = delete;
  MasterActor(MasterActor &&) = delete;
  MasterActor &operator=(MasterActor &&) = delete;
  ~MasterActor() final {
    alive_ = 987654321;
  }
  void msg() final {
    CHECK(alive_ == 123456789);
  }
  pxc::uint64 alive_ = 123456789;
};

TEST(Actors, call_after_destruct) {
  pxc::Scheduler scheduler;
  scheduler.init(0, create_queues(), nullptr);
  {
    auto guard = scheduler.get_guard();
    pxc::create_actor<MasterActor>("Master").release();
  }
  scheduler.run(pxc::Timestamp::in(1));
}

class LinkTokenSlave final : public pxc::Actor {
 public:
  explicit LinkTokenSlave(pxc::ActorShared<> parent) : parent_(std::move(parent)) {
  }
  void add(pxc::uint64 link_token) {
    CHECK(link_token == get_link_token());
  }
  void close() {
    stop();
  }

 private:
  pxc::ActorShared<> parent_;
};

class LinkTokenMasterActor final : public pxc::Actor {
 public:
  explicit LinkTokenMasterActor(int cnt) : cnt_(cnt) {
  }
  void start_up() final {
    child_ = pxc::create_actor<LinkTokenSlave>("Slave", actor_shared(this, 123)).release();
    yield();
  }
  void loop() final {
    for (int i = 0; i < 100 && cnt_ > 0; cnt_--, i++) {
      auto token = static_cast<pxc::uint64>(cnt_) + 1;
      switch (i % 4) {
        case 0: {
          pxc::send_closure(pxc::ActorShared<LinkTokenSlave>(child_, token), &LinkTokenSlave::add, token);
          break;
        }
        case 1: {
          pxc::send_closure_later(pxc::ActorShared<LinkTokenSlave>(child_, token), &LinkTokenSlave::add, token);
          break;
        }
        case 2: {
          pxc::EventCreator::closure(pxc::ActorShared<LinkTokenSlave>(child_, token), &LinkTokenSlave::add, token)
              .try_emit();
          break;
        }
        case 3: {
          pxc::EventCreator::closure(pxc::ActorShared<LinkTokenSlave>(child_, token), &LinkTokenSlave::add, token)
              .try_emit_later();
          break;
        }
      }
    }
    if (cnt_ == 0) {
      pxc::send_closure(child_, &LinkTokenSlave::close);
    } else {
      yield();
    }
  }

  void hangup_shared() final {
    CHECK(get_link_token() == 123);
    pxc::Scheduler::instance()->finish();
    stop();
  }

 private:
  int cnt_;
  pxc::ActorId<LinkTokenSlave> child_;
};

TEST(Actors, link_token) {
  pxc::ConcurrentScheduler scheduler(0, 0);
  auto cnt = 100000;
  scheduler.create_actor_unsafe<LinkTokenMasterActor>(0, "A", cnt).release();
  scheduler.start();
  while (scheduler.run_main(10)) {
  }
  scheduler.finish();
}

TEST(Actors, promise) {
  int value = -1;
  pxc::Promise<int> p1 = pxc::PromiseCreator::lambda([&](int x) { value = x; });
  p1.set_error(pxc::Status::Error("Test error"));
  ASSERT_EQ(0, value);
  pxc::Promise<pxc::int32> p2 = pxc::PromiseCreator::lambda([&](pxc::Result<pxc::int32> x) { value = 1; });
  p2.set_error(pxc::Status::Error("Test error"));
  ASSERT_EQ(1, value);
}

class LaterSlave final : public pxc::Actor {
 public:
  explicit LaterSlave(pxc::ActorShared<> parent) : parent_(std::move(parent)) {
  }

 private:
  pxc::ActorShared<> parent_;

  void hangup() final {
    sb << "A";
    pxc::send_closure(actor_id(this), &LaterSlave::finish);
  }
  void finish() {
    sb << "B";
    stop();
  }
};

class LaterMasterActor final : public pxc::Actor {
  int cnt_ = 3;
  pxc::vector<pxc::ActorOwn<LaterSlave>> children_;
  void start_up() final {
    for (int i = 0; i < cnt_; i++) {
      children_.push_back(pxc::create_actor<LaterSlave>("B", actor_shared(this)));
    }
    yield();
  }
  void loop() final {
    children_.clear();
  }
  void hangup_shared() final {
    if (!--cnt_) {
      pxc::Scheduler::instance()->finish();
      stop();
    }
  }
};

TEST(Actors, later) {
  sb.clear();
  pxc::ConcurrentScheduler scheduler(0, 0);
  scheduler.create_actor_unsafe<LaterMasterActor>(0, "A").release();
  scheduler.start();
  while (scheduler.run_main(10)) {
  }
  scheduler.finish();
  ASSERT_STREQ(sb.as_cslice().c_str(), "AAABBB");
}

class MultiPromise2 final : public pxc::Actor {
 public:
  void start_up() final {
    auto promise = pxc::PromiseCreator::lambda([](pxc::Result<pxc::Unit> result) {
      result.ensure();
      pxc::Scheduler::instance()->finish();
    });

    pxc::MultiPromiseActorSafe multi_promise{"MultiPromiseActor2"};
    multi_promise.add_promise(std::move(promise));
    for (int i = 0; i < 10; i++) {
      pxc::create_actor<pxc::SleepActor>("Sleep", 0.1, multi_promise.get_promise()).release();
    }
  }
};

class MultiPromise1 final : public pxc::Actor {
 public:
  void start_up() final {
    auto promise = pxc::PromiseCreator::lambda([](pxc::Result<pxc::Unit> result) {
      CHECK(result.is_error());
      pxc::create_actor<MultiPromise2>("B").release();
    });
    pxc::MultiPromiseActorSafe multi_promise{"MultiPromiseActor1"};
    multi_promise.add_promise(std::move(promise));
  }
};

TEST(Actors, MultiPromise) {
  pxc::ConcurrentScheduler scheduler(0, 0);
  scheduler.create_actor_unsafe<MultiPromise1>(0, "A").release();
  scheduler.start();
  while (scheduler.run_main(10)) {
  }
  scheduler.finish();
}

class FastPromise final : public pxc::Actor {
 public:
  void start_up() final {
    pxc::PromiseFuture<int> pf;
    auto promise = pf.move_promise();
    auto future = pf.move_future();
    promise.set_value(123);
    CHECK(future.move_as_ok() == 123);
    pxc::Scheduler::instance()->finish();
  }
};

TEST(Actors, FastPromise) {
  pxc::ConcurrentScheduler scheduler(0, 0);
  scheduler.create_actor_unsafe<FastPromise>(0, "A").release();
  scheduler.start();
  while (scheduler.run_main(10)) {
  }
  scheduler.finish();
}

class StopInTeardown final : public pxc::Actor {
  void loop() final {
    stop();
  }
  void tear_down() final {
    stop();
    pxc::Scheduler::instance()->finish();
  }
};

TEST(Actors, stop_in_teardown) {
  pxc::ConcurrentScheduler scheduler(0, 0);
  scheduler.create_actor_unsafe<StopInTeardown>(0, "A").release();
  scheduler.start();
  while (scheduler.run_main(10)) {
  }
  scheduler.finish();
}

class AlwaysWaitForMailbox final : public pxc::Actor {
 public:
  void start_up() final {
    pxc::create_actor<pxc::SleepActor>("Sleep", 0.1,
                                     pxc::PromiseCreator::lambda([actor_id = actor_id(this), ptr = this](pxc::Unit) {
                                       pxc::send_closure(actor_id, &AlwaysWaitForMailbox::g);
                                       pxc::send_closure(actor_id, &AlwaysWaitForMailbox::g);
                                       CHECK(!ptr->was_f_);
                                     }))
        .release();
  }

  void f() {
    was_f_ = true;
    pxc::Scheduler::instance()->finish();
  }
  void g() {
    pxc::send_closure(actor_id(this), &AlwaysWaitForMailbox::f);
  }

 private:
  bool was_f_{false};
};

TEST(Actors, always_wait_for_mailbox) {
  pxc::ConcurrentScheduler scheduler(0, 0);
  scheduler.create_actor_unsafe<AlwaysWaitForMailbox>(0, "A").release();
  scheduler.start();
  while (scheduler.run_main(10)) {
  }
  scheduler.finish();
}

#if !TD_THREAD_UNSUPPORTED && !TD_EVENTFD_UNSUPPORTED
TEST(Actors, send_from_other_threads) {
  pxc::ConcurrentScheduler scheduler(1, 0);
  int thread_n = 10;
  class Listener final : public pxc::Actor {
   public:
    explicit Listener(int cnt) : cnt_(cnt) {
    }
    void dec() {
      if (--cnt_ == 0) {
        pxc::Scheduler::instance()->finish();
      }
    }

   private:
    int cnt_;
  };

  auto A = scheduler.create_actor_unsafe<Listener>(1, "A", thread_n).release();
  scheduler.start();
  pxc::vector<pxc::thread> threads(thread_n);
  for (auto &thread : threads) {
    thread = pxc::thread([&A, &scheduler] {
      auto guard = scheduler.get_send_guard();
      pxc::send_closure(A, &Listener::dec);
    });
  }
  while (scheduler.run_main(10)) {
  }
  for (auto &thread : threads) {
    thread.join();
  }
  scheduler.finish();
}
#endif

class DelayedCall final : public pxc::Actor {
 public:
  void on_called(int *step) {
    CHECK(*step == 0);
    *step = 1;
  }
};

class MultiPromiseSendClosureLaterTest final : public pxc::Actor {
 public:
  void start_up() final {
    delayed_call_ = pxc::create_actor<DelayedCall>("DelayedCall").release();
    mpa_.add_promise(pxc::PromiseCreator::lambda([this](pxc::Unit) {
      CHECK(step_ == 1);
      step_++;
      pxc::Scheduler::instance()->finish();
    }));
    auto lock = mpa_.get_promise();
    pxc::send_closure_later(delayed_call_, &DelayedCall::on_called, &step_);
    lock.set_value(pxc::Unit());
  }

  void tear_down() final {
    CHECK(step_ == 2);
  }

 private:
  int step_ = 0;
  pxc::MultiPromiseActor mpa_{"MultiPromiseActor"};
  pxc::ActorId<DelayedCall> delayed_call_;
};

TEST(Actors, MultiPromiseSendClosureLater) {
  pxc::ConcurrentScheduler scheduler(0, 0);
  scheduler.create_actor_unsafe<MultiPromiseSendClosureLaterTest>(0, "MultiPromiseSendClosureLaterTest").release();
  scheduler.start();
  while (scheduler.run_main(1)) {
  }
  scheduler.finish();
}
