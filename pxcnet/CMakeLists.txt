
if (NOT DEFINED CMAKE_INSTALL_LIBDIR)
    set(CMAKE_INSTALL_LIBDIR "lib")
endif()

if (NOT OPENSSL_FOUND)
    find_package(OpenSSL REQUIRED)
    find_package(ZLIB REQUIRED)
endif()

set(PXC_NET_SOURCE
	pxc/net/Epoll.cpp
	pxc/net/PollFlags.cpp
	pxc/net/Socket.cpp
	pxc/net/Socks5.cpp
	pxc/net/InboundConnection.cpp
	pxc/net/OutBoundConnection.cpp
	pxc/net/TcpListener.cpp
	pxc/net/TransparentProxy.cpp

	pxc/net/Epoll.h
	pxc/net/PollFlags.h
	pxc/net/Socket.h
	pxc/net/Socks5.h
	pxc/net/TcpListener.h
	pxc/net/TransparentProxy.h
	pxc/net/InboundConnection.h
	pxc/net/InboundConnection.h
	pxc/net/OutBoundConnection.h
)

#SET(TEST_SOURCE
#    ${CMAKE_CURRENT_SOURCE_DIR}/test/SocketFdTest.cpp
#    PARENT_SCOPE
#)
#add_executable(test-tdutils EXCLUDE_FROM_ALL ${TEST_SOURCE})
#add_library(test STATIC ${TEST_SOURCE})
add_library(pxcnet STATIC ${PXC_NET_SOURCE})
target_link_libraries(pxcnet PUBLIC pxcutils pxcactor)

target_include_directories(pxcnet PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})