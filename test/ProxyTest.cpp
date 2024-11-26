#include "pxc/net/Socks5.h"

#include "pxc/actor/actor.h"

#include "pxc/utils/Random.h"
#include "pxc/utils/common.h"
#include "pxc/utils/port/IPAddress.h"
#include "pxc/utils/port/SocketFd.h"

namespace pxc {
class ProxyTest {
public:
    void start() {
        IPAddress ip_address;
        vector<string> ip_address_strings = {"127.0.0.1"};
        Random::shuffle(ip_address_strings);
        for (auto &ip_address_string : ip_address_strings) {
            ip_address.init_ipv4_port(ip_address_string, 9000).ensure();
        }

        auto r_socket_fd = SocketFd::open(ip_address);
        if (r_socket_fd.is_error()) {
            LOG(ERROR) << "socket建立错误！";
        }
        auto socket_fd = r_socket_fd.move_as_ok();
        LOG(INFO) << "socket建立成功！";


        Socks5 socks5(r_socket_fd.move_as_ok(), ip_address, "", "", nullptr, {});
    }

    ActorShared<ProxyTest> create_reference(int64 token) {
        return actor_shared(this, token);
    }
};
}

int main() {
    pxc::ProxyTest proxy_test;
    proxy_test.start();

    return 0;
}