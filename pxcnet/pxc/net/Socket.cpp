#include "Socket.h"

//#include ""

#include <sys/socket.h>
// #include <sys/types.h>
#include <list>
#include <arpa/inet.h>
#include <iostream>
namespace pxc {
    void Socket::listener() {
        struct sockaddr_in server_addr;
        server_socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(9000);
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

        bind(server_socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));
        listen(server_socket_fd, 128);

        struct sockaddr_in client_addr;
        socklen_t length = sizeof(client_addr);

//        Epoll epoll;
//        epoll.init();
//        // epoll.event.data.fd = server_socket_fd;
//        // std::list<int> socket_fd_list;
//        int i = 0;
//        PollFlags flags;
//        flags.add_flags(PollFlags::ReadWrite() | PollFlags::Error() | PollFlags::Close());
//        // struct epoll_event ev;
//        // ev.events = EPOLLIN;    // 检测lfd读读缓冲区是否有数据
//        // ev.data.fd = server_socket_fd;
//        // epoll_ctl(epoll.epoll_fd, EPOLL_CTL_ADD, server_socket_fd, &ev);
//        epoll.subscribe(server_socket_fd, flags);
//        std::cout << "server socket subscribe successfully, start loop, server socket fd ";
//        std::cout << server_socket_fd << std::endl;
//        while (true) {
//            // std::cout << i++ << std::endl;
//            epoll.run(-1);
//        }
    }
}