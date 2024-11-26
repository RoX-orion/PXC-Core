#include "pxc/utils/port/SocketFd.h"
#include "pxc/utils/logging.h"
#include "pxc/utils/Random.h"
#include "pxc/utils/common.h"

#include <unistd.h>

namespace pxc {
class Test {
public:
    static int start() {
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

        char sendline[4096];
        fgets(sendline, 4096, stdin);
        int flag = 0;
        flag = send(socket_fd.get_native_fd().fd(), sendline, strlen(sendline), 0);
        if(flag < 0) {
            LOG(ERROR) << "发送失败！" << flag;
        }
//        sleep(5 * 1000);

        return 0;
    }
};
}

int main() {
    pxc::Test::start();
}