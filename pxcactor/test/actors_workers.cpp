//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2024
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "pxc/actor/actor.h"
#include "pxc/actor/ConcurrentScheduler.h"

#include "pxc/utils/common.h"
#include "pxc/utils/SliceBuilder.h"
#include "pxc/utils/tests.h"
#include "pxc/utils/Time.h"

class PowerWorker final : public pxc::Actor {
 public:
  class Callback {
   public:
    Callback() = default;
    Callback(const Callback &) = delete;
    Callback &operator=(const Callback &) = delete;
    Callback(Callback &&) = delete;
    Callback &operator=(Callback &&) = delete;
    virtual ~Callback() = default;
    virtual void on_ready(int query, int res) = 0;
    virtual void on_closed() = 0;
  };
  void set_callback(pxc::unique_ptr<Callback> callback) {
    callback_ = std::move(callback);
  }
  void task(pxc::uint32 x, pxc::uint32 p) {
    pxc::uint32 res = 1;
    for (pxc::uint32 i = 0; i < p; i++) {
      res *= x;
    }
    callback_->on_ready(x, res);
  }
  void close() {
    callback_->on_closed();
    stop();
  }

 private:
  pxc::unique_ptr<Callback> callback_;
};

class Manager final : public pxc::Actor {
 public:
  Manager(int queries_n, int query_size, pxc::vector<pxc::ActorId<PowerWorker>> workers)
      : workers_(std::move(workers))
      , ref_cnt_(static_cast<int>(workers_.size()))
      , left_query_(queries_n)
      , query_size_(query_size) {
  }

  class Callback final : public PowerWorker::Callback {
   public:
    Callback(pxc::ActorId<Manager> actor_id, int worker_id) : actor_id_(actor_id), worker_id_(worker_id) {
    }
    void on_ready(int query, int result) final {
      pxc::send_closure(actor_id_, &Manager::on_ready, worker_id_, query, result);
    }
    void on_closed() final {
      pxc::send_closure_later(actor_id_, &Manager::on_closed, worker_id_);
    }

   private:
    pxc::ActorId<Manager> actor_id_;
    int worker_id_;
  };

  void start_up() final {
    int i = 0;
    for (auto &worker : workers_) {
      ref_cnt_++;
      pxc::send_closure_later(worker, &PowerWorker::set_callback, pxc::make_unique<Callback>(actor_id(this), i));
      i++;
      pxc::send_closure_later(worker, &PowerWorker::task, 3, query_size_);
      left_query_--;
    }
  }

  void on_ready(int worker_id, int query, int res) {
    ref_cnt_--;
    if (left_query_ == 0) {
      pxc::send_closure(workers_[worker_id], &PowerWorker::close);
    } else {
      ref_cnt_++;
      pxc::send_closure(workers_[worker_id], &PowerWorker::task, 3, query_size_);
      left_query_--;
    }
  }

  void on_closed(int worker_id) {
    ref_cnt_--;
    if (ref_cnt_ == 0) {
      pxc::Scheduler::instance()->finish();
      stop();
    }
  }

 private:
  pxc::vector<pxc::ActorId<PowerWorker>> workers_;
  int ref_cnt_;
  int left_query_;
  int query_size_;
};

static void test_workers(int threads_n, int workers_n, int queries_n, int query_size) {
  pxc::ConcurrentScheduler sched(threads_n, 0);

  pxc::vector<pxc::ActorId<PowerWorker>> workers;
  for (int i = 0; i < workers_n; i++) {
    int thread_id = threads_n ? i % (threads_n - 1) + 2 : 0;
    workers.push_back(sched.create_actor_unsafe<PowerWorker>(thread_id, PSLICE() << "worker" << i).release());
  }
  sched.create_actor_unsafe<Manager>(threads_n ? 1 : 0, "Manager", queries_n, query_size, std::move(workers)).release();

  sched.start();
  while (sched.run_main(10)) {
    // empty
  }
  sched.finish();

  // sched.test_one_thread_run();
}

TEST(Actors, workers_big_query_one_thread) {
  test_workers(0, 10, 1000, 300000);
}

TEST(Actors, workers_big_query_two_threads) {
  test_workers(2, 10, 1000, 300000);
}

TEST(Actors, workers_big_query_nine_threads) {
  test_workers(9, 10, 1000, 300000);
}

TEST(Actors, workers_small_query_one_thread) {
  test_workers(0, 10, 100000, 1);
}

TEST(Actors, workers_small_query_two_threads) {
  test_workers(2, 10, 100000, 1);
}

TEST(Actors, workers_small_query_nine_threads) {
  test_workers(9, 10, 10000, 1);
}

class SenderActor;

class ReceiverActor final : public pxc::Actor {
 public:
  void receive(pxc::ActorId<SenderActor>) {
  }
};

class SenderActor final : public pxc::Actor {
 public:
  explicit SenderActor(pxc::ActorId<ReceiverActor> actor_id) : actor_id_(std::move(actor_id)) {
  }

 private:
  pxc::ActorId<ReceiverActor> actor_id_;

  void loop() final {
    for (int i = 0; i < 10000; i++) {
      send_closure(actor_id_, &ReceiverActor::receive, actor_id(this));
    }
    set_timeout_in(0.001);
  }
};

TEST(Actors, send_closure_while_finish) {
  pxc::ConcurrentScheduler sched(1, 0);

  auto receiver = sched.create_actor_unsafe<ReceiverActor>(0, "ReceiverActor").release();
  sched.create_actor_unsafe<SenderActor>(1, "SenderActor", receiver).release();

  sched.start();
  auto end_time = pxc::Time::now() + 0.2;
  while (pxc::Time::now() < end_time) {
    sched.run_main(0.1);
  }
  sched.finish();
}
