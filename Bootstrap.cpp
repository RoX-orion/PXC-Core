#include "pxc/net/Socket.h"
#include "pxc/net/InboundConnection.h"

#include "pxc/actor/actor.h"
#include "pxc/actor/ConcurrentScheduler.h"
#include "pxc/net/TcpListener.h"

#include "pxc/utils/port/detail/PollableFd.h"

const int N = 0;
static int cnt = 0;

class HelloWorld final : public pxc::InboundConnection::Callback {
public:
    void handle(pxc::SocketFd fd) final {
        int socket_fd = fd.get_native_fd().fd();
        LOG(INFO) << "socket_fd: " << socket_fd;
        pxc::Scheduler::subscribe(fd.get_poll_info().extract_pollable_fd(this));
        // LOG(ERROR) << *query;
//        td::HttpHeaderCreator hc;
//        td::Slice content = "hello world";
//        //auto content = td::BufferSlice("hello world");
//        hc.init_ok();
//        hc.set_keep_alive();
//        hc.set_content_size(content.size());
//        hc.add_header("Server", "TDLib/test");
//        hc.add_header("Date", "Thu Dec 14 01:41:50 2017");
//        hc.add_header("Content-Type:", "text/html");
//
//        auto res = hc.finish(content);
//        LOG_IF(FATAL, res.is_error()) << res.error();
//        send_closure(connection, &td::HttpInboundConnection::write_next, td::BufferSlice(res.ok()));
//        send_closure(connection.release(), &td::HttpInboundConnection::write_ok);
    }
    void hangup() final {
        LOG(ERROR) << "CLOSE ";
        stop();
    }
};

class Server final : public pxc::TcpListener::Callback {
public:
    void start_up() final {
        listener_ =
            pxc::create_actor<pxc::TcpListener>("Listener", 8082, pxc::ActorOwn<pxc::TcpListener::Callback>(actor_id(this)));
    }
    void accept(pxc::SocketFd fd) final {
//        pxc::MutableSlice slice;
//        fd.read(slice);
        int socket_fd = fd.get_native_fd().fd();
        LOG(INFO) << "socket_fd: " << socket_fd;
//        char buf[1024];
//        int n = recv(socket_fd, buf, 1024, 0);
//        if (n > 0) {
//            buf[n] = '\0';
//            std::cout << "buffer:" << buf << std::endl;
//        }

//        pxc::Scheduler::subscribe(fd.get_poll_info().extract_pollable_fd(this));
//        pos_++;
//        auto scheduler_id = pos_ % (N != 0 ? N : 1) + (N != 0);
        pxc::create_actor_on_scheduler<pxc::InboundConnection>("InboundConnection", 0, std::move(fd)).release();
    }
    void hangup() final {
        // may be it should be default?..
        LOG(ERROR) << "Hanging up..";
        stop();
    }

private:
    pxc::ActorOwn<pxc::TcpListener> listener_;
    int pos_{0};
};


int main() {
//  net::Socket server_socket;
//  server_socket.listener();

    auto scheduler = pxc::make_unique<pxc::ConcurrentScheduler>(N, 0);
    scheduler->create_actor_unsafe<Server>(0, "Server").release();
    scheduler->start();
    while (scheduler->run_main(10)) {
        // empty
    }
    scheduler->finish();
//
//    pxc::InboundConnection inboundConnection = pxc::InboundConnection(0, pxc::ActorShared());
//    inboundConnection.init();

  return 0;
}