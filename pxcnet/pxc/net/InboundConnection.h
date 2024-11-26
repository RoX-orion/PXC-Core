#pragma once

#include "pxc/actor/actor.h"

#include "pxc//utils/BufferedFd.h"
#include "pxc/utils/port/Poll.h"
#include "pxc/utils/port/ServerSocketFd.h"

namespace pxc {
class InboundConnection final : public Actor {
public:
    class Callback : public Actor {
    public:
        virtual void handle(SocketFd fd) = 0;
    };

    explicit InboundConnection(SocketFd fd);

    void init();
    void start_up() final;
    void loop() final;

    Status do_loop();

    Status write_loop();

    Status read_loop();
//    void loop(int native_fd);

private:
    Poll poll_;
    SocketFd socket_fd_;
    int port_{};
    ServerSocketFd server_fd_;
    ActorShared<Callback> callback_;
    const string server_address_;
    std::array<char, 1024> read_buf{};
    size_t read_new_lines{0};

    pxc::string hello_;
    pxc::string write_buf_;
    size_t write_pos_{0};

//    void loop() override;
    };
} // pxcnet
