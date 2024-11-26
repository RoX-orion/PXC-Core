if ((CMAKE_MAJOR_VERSION LESS 3) OR (CMAKE_VERSION VERSION_LESS "3.24.1"))
    message(FATAL_ERROR "CMake >= 3.24.1 is required")
endif()

if (NOT DEFINED CMAKE_INSTALL_LIBDIR)
    set(CMAKE_INSTALL_LIBDIR "lib")
endif()

if (NOT ZLIB_FOUND)
    find_package(ZLIB)
endif()
if (ZLIB_FOUND)
    set(TD_HAVE_ZLIB 1)
    message(STATUS "Found ZLIB: ${ZLIB_INCLUDE_DIR} ${ZLIB_LIBRARIES}")

    # OpenSSL internally depends on zlib
    if (NOT OPENSSL_FOUND)
        find_package(OpenSSL)
    endif()
    if (OPENSSL_FOUND)
        set(TD_HAVE_OPENSSL 1)
    endif()
endif()

if (NOT CRC32C_FOUND)
    find_package(Crc32c QUIET)
endif()
if (CRC32C_FOUND)
    set(TD_HAVE_CRC32C 1)
endif()

if (TD_WITH_ABSEIL)
    find_package(ABSL QUIET)
    if (ABSL_FOUND)
        set(TD_HAVE_ABSL 1)
    endif()
endif()

configure_file(pxc/utils/config.h.in pxc/utils/config.h @ONLY)

# TDUTILS
#set_source_files_properties(${TDMIME_AUTO} PROPERTIES GENERATED TRUE)
#if (CLANG OR GCC)
#    set_property(SOURCE ${TDMIME_AUTO} APPEND_STRING PROPERTY COMPILE_FLAGS " -Wno-conversion")
#elseif (MSVC)
#    set_property(SOURCE ${TDMIME_AUTO} APPEND_STRING PROPERTY COMPILE_FLAGS " /wd4267")
#endif()
#if (CLANG)
#    set_property(SOURCE ${TDMIME_AUTO} APPEND_STRING PROPERTY COMPILE_FLAGS " -Wno-deprecated-register")
#endif()

set(PXC_UTILS_SOURCE
    pxc/utils/port/Clocks.cpp
    pxc/utils/port/FileFd.cpp
    pxc/utils/port/IPAddress.cpp
    pxc/utils/port/MemoryMapping.cpp
    pxc/utils/port/path.cpp
    pxc/utils/port/platform.cpp
    pxc/utils/port/PollFlags.cpp
    pxc/utils/port/rlimit.cpp
    pxc/utils/port/ServerSocketFd.cpp
    pxc/utils/port/signals.cpp
    pxc/utils/port/sleep.cpp
    pxc/utils/port/SocketFd.cpp
    pxc/utils/port/stacktrace.cpp
    pxc/utils/port/Stat.cpp
    pxc/utils/port/StdStreams.cpp
    pxc/utils/port/thread_local.cpp
    pxc/utils/port/UdpSocketFd.cpp
    pxc/utils/port/uname.cpp
    pxc/utils/port/user.cpp
    pxc/utils/port/wstring_convert.cpp

    pxc/utils/port/detail/Epoll.cpp
    pxc/utils/port/detail/EventFdBsd.cpp
    pxc/utils/port/detail/EventFdLinux.cpp
    pxc/utils/port/detail/EventFdWindows.cpp
    pxc/utils/port/detail/Iocp.cpp
    pxc/utils/port/detail/KQueue.cpp
    pxc/utils/port/detail/NativeFd.cpp
    pxc/utils/port/detail/Poll.cpp
    pxc/utils/port/detail/Select.cpp
    pxc/utils/port/detail/ThreadIdGuard.cpp
    pxc/utils/port/detail/ThreadPthread.cpp
    pxc/utils/port/detail/WineventPoll.cpp

#    ${TDMIME_AUTO}

    pxc/utils/AsyncFileLog.cpp
    pxc/utils/base64.cpp
    pxc/utils/BigNum.cpp
    pxc/utils/buffer.cpp
    pxc/utils/BufferedUdp.cpp
    pxc/utils/check.cpp
    pxc/utils/crypto.cpp
    pxc/utils/emoji.cpp
    pxc/utils/ExitGuard.cpp
    pxc/utils/FileLog.cpp
    pxc/utils/filesystem.cpp
    pxc/utils/find_boundary.cpp
    pxc/utils/FlatHashTable.cpp
    pxc/utils/FloodControlGlobal.cpp
    pxc/utils/Gzip.cpp
    pxc/utils/GzipByteFlow.cpp
    pxc/utils/Hints.cpp
    pxc/utils/HttpUrl.cpp
    pxc/utils/JsonBuilder.cpp
    pxc/utils/logging.cpp
    pxc/utils/misc.cpp
    pxc/utils/MpmcQueue.cpp
    pxc/utils/OptionParser.cpp
    pxc/utils/PathView.cpp
    pxc/utils/Random.cpp
    pxc/utils/SharedSlice.cpp
    pxc/utils/Slice.cpp
    pxc/utils/StackAllocator.cpp
    pxc/utils/Status.cpp
    pxc/utils/StringBuilder.cpp
    pxc/utils/tests.cpp
    pxc/utils/Time.cpp
    pxc/utils/Timer.cpp
    pxc/utils/tl_parsers.cpp
    pxc/utils/translit.cpp
    pxc/utils/TsCerr.cpp
    pxc/utils/TsFileLog.cpp
    pxc/utils/TsLog.cpp
    pxc/utils/unicode.cpp
    pxc/utils/utf8.cpp

    pxc/utils/port/Clocks.h
    pxc/utils/port/config.h
    pxc/utils/port/CxCli.h
    pxc/utils/port/EventFd.h
    pxc/utils/port/EventFdBase.h
    pxc/utils/port/FileFd.h
    pxc/utils/port/FromApp.h
    pxc/utils/port/IPAddress.h
    pxc/utils/port/IoSlice.h
    pxc/utils/port/MemoryMapping.h
    pxc/utils/port/Mutex.h
    pxc/utils/port/path.h
    pxc/utils/port/platform.h
    pxc/utils/port/Poll.h
    pxc/utils/port/PollBase.h
    pxc/utils/port/PollFlags.h
    pxc/utils/port/rlimit.h
    pxc/utils/port/RwMutex.h
    pxc/utils/port/ServerSocketFd.h
    pxc/utils/port/signals.h
    pxc/utils/port/sleep.h
    pxc/utils/port/SocketFd.h
    pxc/utils/port/stacktrace.h
    pxc/utils/port/Stat.h
    pxc/utils/port/StdStreams.h
    pxc/utils/port/thread.h
    pxc/utils/port/thread_local.h
    pxc/utils/port/UdpSocketFd.h
    pxc/utils/port/uname.h
    pxc/utils/port/user.h
    pxc/utils/port/wstring_convert.h

    pxc/utils/port/detail/Epoll.h
    pxc/utils/port/detail/EventFdBsd.h
    pxc/utils/port/detail/EventFdLinux.h
    pxc/utils/port/detail/EventFdWindows.h
    pxc/utils/port/detail/Iocp.h
    pxc/utils/port/detail/KQueue.h
    pxc/utils/port/detail/NativeFd.h
    pxc/utils/port/detail/Poll.h
    pxc/utils/port/detail/PollableFd.h
    pxc/utils/port/detail/Select.h
    pxc/utils/port/detail/skip_eintr.h
    pxc/utils/port/detail/ThreadIdGuard.h
    pxc/utils/port/detail/ThreadPthread.h
    pxc/utils/port/detail/ThreadStl.h
    pxc/utils/port/detail/WineventPoll.h

    pxc/utils/AesCtrByteFlow.h
    pxc/utils/algorithm.h
    pxc/utils/as.h
    pxc/utils/AsyncFileLog.h
    pxc/utils/AtomicRead.h
    pxc/utils/base64.h
    pxc/utils/benchmark.h
    pxc/utils/BigNum.h
    pxc/utils/bits.h
    pxc/utils/buffer.h
    pxc/utils/BufferedFd.h
    pxc/utils/BufferedReader.h
    pxc/utils/BufferedUdp.h
    pxc/utils/ByteFlow.h
    pxc/utils/CancellationToken.h
    pxc/utils/ChainScheduler.h
    pxc/utils/ChangesProcessor.h
    pxc/utils/check.h
    pxc/utils/Closure.h
    pxc/utils/CombinedLog.h
    pxc/utils/common.h
    pxc/utils/ConcurrentHashTable.h
    pxc/utils/Container.h
    pxc/utils/Context.h
    pxc/utils/crypto.h
    pxc/utils/DecTree.h
    pxc/utils/Destructor.h
    pxc/utils/emoji.h
    pxc/utils/Enumerator.h
    pxc/utils/EpochBasedMemoryReclamation.h
    pxc/utils/ExitGuard.h
    pxc/utils/FileLog.h
    pxc/utils/filesystem.h
    pxc/utils/fixed_vector.h
    pxc/utils/find_boundary.h
    pxc/utils/FlatHashMap.h
    pxc/utils/FlatHashMapChunks.h
    pxc/utils/FlatHashSet.h
    pxc/utils/FlatHashTable.h
    pxc/utils/FloodControlFast.h
    pxc/utils/FloodControlGlobal.h
    pxc/utils/FloodControlStrict.h
    pxc/utils/format.h
    pxc/utils/Gzip.h
    pxc/utils/GzipByteFlow.h
    pxc/utils/Hash.h
    pxc/utils/HashMap.h
    pxc/utils/HashSet.h
    pxc/utils/HashTableUtils.h
    pxc/utils/HazardPointers.h
    pxc/utils/Heap.h
    pxc/utils/Hints.h
    pxc/utils/HttpUrl.h
    pxc/utils/int_types.h
    pxc/utils/invoke.h
    pxc/utils/JsonBuilder.h
    pxc/utils/List.h
    pxc/utils/logging.h
    pxc/utils/MapNode.h
    pxc/utils/MemoryLog.h
    pxc/utils/misc.h
    pxc/utils/MovableValue.h
    pxc/utils/MpmcQueue.h
    pxc/utils/MpmcWaiter.h
    pxc/utils/MpscPollableQueue.h
    pxc/utils/MpscLinkQueue.h
    pxc/utils/Named.h
    pxc/utils/NullLog.h
    pxc/utils/ObjectPool.h
    pxc/utils/Observer.h
    pxc/utils/optional.h
    pxc/utils/OptionParser.h
    pxc/utils/OrderedEventsProcessor.h
    pxc/utils/overloaded.h
    pxc/utils/Parser.h
    pxc/utils/PathView.h
    pxc/utils/Promise.h
    pxc/utils/queue.h
    pxc/utils/Random.h
    pxc/utils/ScopeGuard.h
    pxc/utils/SetNode.h
    pxc/utils/SharedObjectPool.h
    pxc/utils/SharedSlice.h
    pxc/utils/Slice-decl.h
    pxc/utils/Slice.h
    pxc/utils/SliceBuilder.h
    pxc/utils/Span.h
    pxc/utils/SpinLock.h
    pxc/utils/StackAllocator.h
    pxc/utils/Status.h
    pxc/utils/StealingQueue.h
    pxc/utils/Storer.h
    pxc/utils/StorerBase.h
    pxc/utils/StringBuilder.h
    pxc/utils/tests.h
    pxc/utils/ThreadLocalStorage.h
    pxc/utils/ThreadSafeCounter.h
    pxc/utils/Time.h
    pxc/utils/TimedStat.h
    pxc/utils/Timer.h
    pxc/utils/tl_helpers.h
    pxc/utils/tl_parsers.h
    pxc/utils/tl_storers.h
    pxc/utils/TlDowncastHelper.h
    pxc/utils/TlStorerToString.h
    pxc/utils/translit.h
    pxc/utils/TsCerr.h
    pxc/utils/TsFileLog.h
    pxc/utils/TsList.h
    pxc/utils/TsLog.h
    pxc/utils/type_traits.h
    pxc/utils/UInt.h
    pxc/utils/uint128.h
    pxc/utils/unicode.h
    pxc/utils/unique_ptr.h
    pxc/utils/unique_value_ptr.h
    pxc/utils/utf8.h
    pxc/utils/Variant.h
    pxc/utils/VectorQueue.h
    pxc/utils/WaitFreeHashMap.h
    pxc/utils/WaitFreeHashSet.h
    pxc/utils/WaitFreeVector.h
)

add_library(pxcutils STATIC ${PXC_UTILS_SOURCE})

#target_include_directories(pxcutils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(pxcutils PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)

if (OPENSSL_FOUND)
    target_link_libraries(pxcutils PRIVATE ${OPENSSL_CRYPTO_LIBRARY} ${CMAKE_DL_LIBS} ${ZLIB_LIBRARIES})
    target_include_directories(pxcutils SYSTEM PRIVATE ${OPENSSL_INCLUDE_DIR})

    if (WIN32)
        if (MINGW)
            target_link_libraries(pxcutils PRIVATE ws2_32 mswsock crypt32)
        else()
            target_link_libraries(pxcutils PRIVATE ws2_32 Mswsock Crypt32)
        endif()
    endif()
endif()

if (ZLIB_FOUND)
    target_link_libraries(pxcutils PRIVATE ${ZLIB_LIBRARIES})
    target_include_directories(pxcutils SYSTEM PRIVATE ${ZLIB_INCLUDE_DIR})
endif()

if (CRC32C_FOUND)
    target_link_libraries(pxcutils PRIVATE crc32c)
endif()
if (ABSL_FOUND)
    target_link_libraries(pxcutils PUBLIC absl::flat_hash_map absl::flat_hash_set absl::hash)
endif()

if (WIN32)
    # find_library for system libraries doesn't work for UWP builds
    # find_library(WS2_32_LIBRARY ws2_32)
    # find_library(MSWSOCK_LIBRARY Mswsock)
    # target_link_libraries(pxcutils PRIVATE ${WS2_32_LIBRARY} ${MSWSOCK_LIBRARY})
    if (MINGW)
        target_link_libraries(pxcutils PRIVATE ws2_32 mswsock normaliz psapi)
    else()
        target_link_libraries(pxcutils PRIVATE ws2_32 Mswsock Normaliz psapi)
    endif()
    if (NOT CMAKE_SYSTEM_NAME STREQUAL "WindowsStore")
        target_link_libraries(pxcutils PRIVATE shell32)
    endif()
endif()

if (ANDROID)
    target_link_libraries(pxcutils PRIVATE log)
endif()

install(TARGETS pxcutils EXPORT TdStaticTargets
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)
# target_include_directories(pxcutils INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})