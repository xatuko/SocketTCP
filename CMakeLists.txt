cmake_minimum_required(VERSION 3.0.0)
project(SocketTCP VERSION 0.1.0)

find_package(Threads REQUIRED)

add_executable(SocketTCP main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

if(WIN32)
    add_compile_definitions(WINDOWS)
    add_subdirectory(libsocktcp)
    target_link_libraries(SocketTCP PRIVATE SocketTCP_library Threads::Threads wsock32 ws2_32)
else()
    add_compile_definitions(UNIX)
    add_subdirectory(libsocktcp)
    target_link_libraries(SocketTCP PRIVATE SocketTCP_library Threads::Threads)
endif()