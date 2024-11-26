cmake_minimum_required(VERSION 3.24.1)

project(PXC-Core VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(SOURCE
  Bootstrap.cpp
)
add_subdirectory(pxcutils)

if (NOT OPENSSL_FOUND)
	find_package(OpenSSL)
endif()
if (OPENSSL_FOUND)
	message(STATUS "Found OpenSSL: ${OPENSSL_INCLUDE_DIR} ${OPENSSL_LIBRARIES}")
endif()

if (NOT OPENSSL_FOUND)
	message(WARNING "Can't find OpenSSL: stop TDLib building")
	return()
endif()

add_subdirectory(pxcnet)
add_subdirectory(pxcactor)
add_subdirectory(test)

#if (NOT CMAKE_CROSSCOMPILING)
#	add_subdirectory(test)
#endif()

add_executable(Server Bootstrap.cpp)
target_link_libraries(Server PUBLIC pxcnet)