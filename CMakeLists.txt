cmake_minimum_required(VERSION 3.2)

set(PROJECT_VERSION_PATCH $ENV{TRAVIS_BUILD_NUMBER})

project(bulk_server VERSION 0.0.${PROJECT_VERSION_PATCH})

find_package(Boost COMPONENTS system date_time regex REQUIRED)

configure_file(version.h.in version.h @ONLY)

set(SRC
    main.cpp
    handler_connect.cpp
    server.cpp
    
    async/async.cpp
    async/service.cpp
    async/SubjectRawData.cpp
    async/bulkmt/IState.cpp
    async/bulkmt/ISubject.cpp
    )

set(HDR
    handler_connect.h
    server.h
    separator_cmd.h
    
    async/async.h
    async/service.h    
    async/SubjectRawData.h
    async/bulkmt/IState.h
    async/bulkmt/IObserver.h
    async/bulkmt/ISubject.h
    async/bulkmt/threadsafe_queue.h
    async/bulkmt/threadsafe_queue_const.h
    async/bulkmt/ConsoleObserver.h
    async/bulkmt/FileObserver.h
    async/bulkmt/Statistic.h
    async/bulkmt/ThreadObserver.h
    async/bulkmt/FileObserverMT.h
    async/bulkmt/IStreamSubject.h
    async/bulkmt/pack_cmd.h
    )

#program bulk_server
add_executable(bulk_server ${SRC} ${HDR})

target_include_directories(bulk_server PUBLIC ${CMAKE_CURRENT_BINARY_DIR} async/bulkmt async ${Boost_INCLUDE_DIR} )

set_target_properties( bulk_server PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
   # COMPILE_OPTIONS -Wall
)

target_link_libraries(bulk_server 
    ${Boost_LIBRARIES}    
)

if(UNIX)

set_target_properties( bulk_server PROPERTIES
    COMPILE_OPTIONS -pthread
)
target_link_libraries( bulk_server pthread )

endif(UNIX)

install(TARGETS bulk_server RUNTIME DESTINATION bin)


set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT aquila-2004@yandex.ru)

include(CPack)
