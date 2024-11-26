//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2024
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "pxc/net/TransparentProxy.h"

#include "pxc/utils/Status.h"

#include <hash_map>

namespace pxc {

class Socks5 final/* : public TransparentProxy*/ {
public:
//        using TransparentProxy::TransparentProxy;
    enum class Socks5State {
        Auth, Request
    };
    class Connection {
    private:
        Socks5State state;
        int client_fd;
    };
    
private:
    __gnu_cxx::hash_map<int, int> map;
    enum class State {
        SendGreeting,
        WaitGreetingResponse,
        WaitPasswordResponse,
        WaitIpAddressResponse
    } state_ = State::SendGreeting;


    void handle_connect();
    void handle_auth(int native_fd, char buf[]);
    void handle_request(int native_fd, char buf[]);
//        void send_greeting();
//        Status wait_greeting_response();
//        Status send_username_password();
//
//        Status wait_password_response();
//
//        void send_ip_address();
//        Status wait_ip_address_response();
//
//        Status loop_impl() final;

};

}  // namespace pxc
