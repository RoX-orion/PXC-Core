#include "Epoll.h"
#include "pxc/utils/Status.h"

#include <iostream>
#include <string.h>

namespace pxc {
  void Epoll::init() {
    epoll_fd = epoll_create(1);
    events_.resize(1000);
  }

  void Epoll::subscribe(int native_fd, PollFlags flags) {
    if (epoll_fd == 0) {
      perror("epoll_fd can't be 0");
      exit(-1);
    }
    epoll_event event;
    event.data.fd = native_fd;
    event.events = EPOLLHUP | EPOLLERR | EPOLLET;
#ifdef EPOLLRDHUP
    event.events |= EPOLLRDHUP;
#endif
    if (flags.can_read()) {
      event.events |= EPOLLIN;
    }
    if (flags.can_write()) {
      event.events |= EPOLLOUT;
    }
    // auto native_fd = fd.native_fd().fd();
    // auto *list_node = fd.release_as_list_node();
    // list_root_.put(list_node);
    // event.data.ptr = list_node;

    int err = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, native_fd, &event);
    auto epoll_ctl_errno = errno;
    LOG_IF(FATAL, err == -1) << pxc::Status::PosixError(epoll_ctl_errno, "epoll_ctl ADD failed")
                              << ", epoll_fd = " << epoll_fd << ", fd = " << native_fd;
  }

  void Epoll::unsubscribe(int native_fd /*PollableFdRef fd_ref*/) {
    // auto fd = fd_ref.lock();
    // auto native_fd = fd.native_fd().fd();
    int err = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, native_fd, nullptr);
    // auto epoll_ctl_errno = errno;
    // LOG_IF(FATAL, err == -1) << Status::PosixError(epoll_ctl_errno, "epoll_ctl DEL failed")
    //                          << ", epoll_fd = " << epoll_fd_.fd() << ", fd = " << native_fd
    //                          << ", status = " << fd.native_fd().validate();
  }

  void Epoll::run(int timeout_ms) {
    int ready_n = epoll_wait(epoll_fd, &events_[0], static_cast<int>(events_.size()), timeout_ms);
    int addrlen;
    struct epoll_event ev{};
    int client_fd;
    char buf[1024];
    struct sockaddr_in client_addr{};
    
    /**
     * int socket_fd = accept(server_socket_fd, (struct sockaddr*)&client_addr, &length);
        // socket_fd_list.push_back(socket_fd);
        std::cout << "socketFd: " << socket_fd << std::endl;
        PollFlags flags;
        flags.add_flags(PollFlags::ReadWrite() | PollFlags::Error() | PollFlags::Close());
        epoll.subscribe(socket_fd, flags);
    */
    std::cout<< "\nready fd count: " << ready_n << std::endl;
    for (int i = 0; i < ready_n; i++) {
      // PollFlags flags;
      // std::cout << "ready fd: " << events_[i].data.fd << std::endl;
      int native_fd = events_[i].data.fd;
      std::cout << "native fd: " << native_fd << std::endl;

      //     epoll_event *event = &events_[i];
      //     if (event->events & EPOLLIN) {
      //       event->events &= ~EPOLLIN;
      //       flags = flags | PollFlags::Read();
      //     }
      //     if (event->events & EPOLLOUT) {
      //       event->events &= ~EPOLLOUT;
      //       flags = flags | PollFlags::Write();
      //     }
      // #ifdef EPOLLRDHUP
      //     if (event->events & EPOLLRDHUP) {
      //       event->events &= ~EPOLLRDHUP;
      //       flags = flags | PollFlags::Close();
      //     }
      // #endif
      //     if (event->events & EPOLLHUP) {
      //       event->events &= ~EPOLLHUP;
      //       flags = flags | PollFlags::Close();
      //     }
      //     if (event->events & EPOLLERR) {
      //       event->events &= ~EPOLLERR;
      //       flags = flags | PollFlags::Error();
      //     }
      //     if (event->events) {
      //       std::cout << "Unsupported epoll events: " << static_cast<int32>(event->events);
      //     }
      // auto pollable_fd = PollableFd::from_list_node(static_cast<ListNode *>(event->data.ptr));
      // pollable_fd.add_flags(flags);
      // pollable_fd.release_as_list_node();
    }
  }
} // namespace pxcnet