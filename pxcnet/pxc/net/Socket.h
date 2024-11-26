#pragma once

#include "Epoll.h"

namespace pxc {
    class Socket {
    private:
        int server_socket_fd;
    public:
        Socket() = default;
        ~Socket() = default;
        void listener();
        int get_socket_fd() {
            return server_socket_fd;
        }
    };
}
