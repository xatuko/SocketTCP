cmake_minimum_required(VERSION 3.0.0)
project(SocketTCP VERSION 0.1.0)

find_package(Threads REQUIRED)

add_executable(SocketTCP main.cpp)

add_subdirectory(libsocktcp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

target_link_libraries(SocketTCP SocketTCP_library Threads::Threads)
