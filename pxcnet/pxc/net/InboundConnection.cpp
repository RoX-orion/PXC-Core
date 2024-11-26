#include "InboundConnection.h"
#include "Epoll.h"

#include "pxc/utils/port/ServerSocketFd.h"

namespace pxc {
//    enum class InBoundType {Socks5, Http};
    InboundConnection::InboundConnection(SocketFd fd)
        : socket_fd_(std::move(fd)){
    }

    void InboundConnection::start_up() {
        pxc::Scheduler::subscribe(socket_fd_.get_poll_info().extract_pollable_fd(this));
    }

    void InboundConnection::loop() {
        LOG(INFO) << "start inbound connection loop!!!";
        auto status = do_loop();
        if (status.is_error()) {
            pxc::Scheduler::unsubscribe(socket_fd_.get_poll_info().get_pollable_fd_ref());
            stop();
            LOG(ERROR) << "CLOSE: " << status;
        }
    }

    pxc::Status InboundConnection::do_loop() {
        sync_with_poll(socket_fd_);
        TRY_STATUS(read_loop());
        TRY_STATUS(write_loop());
        if (can_close_local(socket_fd_)) {
            return pxc::Status::Error("CLOSE");
        }
        return pxc::Status::OK();
    }
    pxc::Status InboundConnection::write_loop() {
        while (can_write_local(socket_fd_) && write_pos_ < write_buf_.size()) {
            LOG(INFO) << "write data";
            TRY_RESULT(written, socket_fd_.write(pxc::Slice(write_buf_).substr(write_pos_)));
            write_pos_ += written;
            if (write_pos_ == write_buf_.size()) {
                write_pos_ = 0;
                write_buf_.clear();
            }
        }
        return pxc::Status::OK();
    }
    pxc::Status InboundConnection::read_loop() {
        while (can_read_local(socket_fd_)) {
            LOG(INFO) << "read data";
            TRY_RESULT(read_size, socket_fd_.read(pxc::MutableSlice(read_buf.data(), read_buf.size())));
            for (const auto& element : read_buf) {
                std::cout << element;
            }
            std::cout << "\n";
            write_buf_.append("123456");

//            for (size_t i = 0; i < read_size; i++) {
//                if (read_buf[i] == '\n') {
//                    read_new_lines++;
//                    if (read_new_lines == 2) {
//                        read_new_lines = 0;
//                        write_buf_.append(hello_);
//                    }
//                }
//            }
        }
        return pxc::Status::OK();
    }

    void InboundConnection::init() {
//        auto r_socket = ServerSocketFd::open(port_, server_address_);
//        if (r_socket.is_error()) {
//            LOG(ERROR) << "Can't open server socket: " << r_socket.error();
////            set_timeout_in(5);
////            exit(-1);
//            return;
//        }
//        server_fd_ = r_socket.move_as_ok();
//
//        poll_.init();
//        PollFlags flags;
//        flags.add_flags(PollFlags::ReadWrite() | PollFlags::Error() | PollFlags::Close());
//        auto server_socket_fd = server_fd_.get_native_fd().fd();
//
//        poll_.subscribe(server_fd_.get_poll_info().extract_pollable_fd(this), flags);
//        std::cout << "server socket subscribe successfully, start loop, server socket fd ";
//        std::cout << server_socket_fd << std::endl;
//        while (true) {
//            poll_.run(-1);
//        }
    }

//    void InboundConnection::loop() {
//        send_closure(callback_, &Callback::handle, std::move(fd_));
//        struct epoll_event ev{};
//        int addrlen;
//        int client_fd;
//        char buf[1024];
//        struct sockaddr_in client_addr{};
//        if (native_fd == server_socket_fd) { // client connection event
//            std::cout << "client connection event" << std::endl;
//            socklen_t length = sizeof(client_addr);
//            if ((client_fd = accept(server_socket_fd, (struct sockaddr *)&client_addr, &length)) == -1) {
//                perror("Server-accept() error!\n");
//                close(client_fd);
//            } else {
//                PollFlags flags;
//                flags.add_flags(PollFlags::ReadWrite());
//                std::cout << "handle here1" << std::endl;
//                Epoll::subscribe(client_fd, flags);
////            socks5.
//            }
//        } else {
//            std::cout << "io event " << std::endl;
//            if (events_[i].events & EPOLLHUP) {
//                std::cout << "EPOLLHUP" << std::endl;
//                if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, native_fd, &ev) < 0) {
//                    perror("epoll_ctl epoll hup\n");
//                }
//                close(client_fd);
//            }
//            if (events_[i].events & EPOLLIN) {
//                std::cout << "EPOLLIN" << std::endl;
//                size_t len;
//                bool disconnect_event = true;
//
//            } else if(events_[i].events & EPOLLOUT) {
//
//            }
//        }
//    }
} // pxc