#pragma once

#include "PollFlags.h"
#include "Socks5.h"

#include "pxc/utils/port/PollBase.h"

#include <sys/socket.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <vector>
#include <iostream>

namespace pxc {
class Epoll final : public PollBase {
private:
  int socket_fd;
  int epoll_fd;
  std::vector<struct epoll_event> events_;
    Socks5 socks5;
  // struct epoll_event events_[1000];
public:
  // int max_event = 1000;
  // struct epoll_event event;
  int client_fd = -1;
  Epoll() = default;
  ~Epoll() = default;
  void init();
  void subscribe(int socket_fd, PollFlags flags);
  void unsubscribe(int native_fd);
  void run(int timeout_ms);
};

} // namespace pxcnet