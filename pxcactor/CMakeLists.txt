if ((CMAKE_MAJOR_VERSION LESS 3) OR (CMAKE_VERSION VERSION_LESS "3.24.1"))
  message(FATAL_ERROR "CMake >= 3.24.1 is required")
endif()

if (NOT DEFINED CMAKE_INSTALL_LIBDIR)
  set(CMAKE_INSTALL_LIBDIR "lib")
endif()

#SOURCE SETS
set(PXCACTOR_SOURCE
  pxc/actor/ConcurrentScheduler.cpp
  pxc/actor/impl/Scheduler.cpp
  pxc/actor/MultiPromise.cpp
  pxc/actor/MultiTimeout.cpp

  pxc/actor/actor.h
  pxc/actor/ConcurrentScheduler.h
  pxc/actor/impl/Actor-decl.h
  pxc/actor/impl/Actor.h
  pxc/actor/impl/ActorId-decl.h
  pxc/actor/impl/ActorId.h
  pxc/actor/impl/ActorInfo-decl.h
  pxc/actor/impl/ActorInfo.h
  pxc/actor/impl/EventFull-decl.h
  pxc/actor/impl/EventFull.h
  pxc/actor/impl/Event.h
  pxc/actor/impl/Scheduler-decl.h
  pxc/actor/impl/Scheduler.h
  pxc/actor/MultiPromise.h
  pxc/actor/MultiTimeout.h
  pxc/actor/PromiseFuture.h
  pxc/actor/SchedulerLocalStorage.h
  pxc/actor/SignalSlot.h
  pxc/actor/SleepActor.h
  pxc/actor/Timeout.h
)

set(TDACTOR_TEST_SOURCE
  ${CMAKE_CURRENT_SOURCE_DIR}/test/actors_main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/test/actors_simple.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/test/actors_workers.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/test/actors_bugs.cpp
  PARENT_SCOPE
)

#RULES

#LIBRARIES

add_library(pxcactor STATIC ${PXCACTOR_SOURCE})
target_include_directories(pxcactor PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(pxcactor PUBLIC pxcutils)

if (NOT CMAKE_CROSSCOMPILING)
  add_executable(example example/example.cpp)
  target_link_libraries(example PRIVATE pxcactor)
endif()

install(TARGETS pxcactor EXPORT TdStaticTargets
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)
