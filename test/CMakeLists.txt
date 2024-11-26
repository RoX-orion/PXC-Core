if ((CMAKE_MAJOR_VERSION LESS 3) OR (CMAKE_VERSION VERSION_LESS "3.24.1"))
	message(FATAL_ERROR "CMake >= 3.24.1 is required")
endif()

if (NOT OPENSSL_FOUND)
	find_package(OpenSSL REQUIRED)
	find_package(ZLIB REQUIRED)
endif()

add_executable(test_socket SocketFdTest.cpp)
target_link_libraries(test_socket PRIVATE pxcnet pxcutils)

add_executable(test_proxy ProxyTest.cpp)
target_link_libraries(test_proxy PRIVATE pxcnet pxcutils pxcactor)
#add_library(test STATIC ${TEST})
#target_link_libraries(test PUBLIC pxcnet)