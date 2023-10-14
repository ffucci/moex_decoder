cmake_minimum_required(VERSION 3.24.4)

SET(MY_INSTALL_PREFIX "/opt/local/bin/cmake" CACHE PATH "Prefix added to install directories")
SET(CMAKE_INSTALL_PREFIX "${MY_INSTALL_PREFIX}" CACHE INTERNAL "Prefix added to install directories" FORCE)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_C_COMPILER /opt/local/libexec/llvm-16/bin/clang)
set(CMAKE_CXX_COMPILER /opt/local/libexec/llvm-16/bin/clang++)
set(CMAKE_CXX_STANDARD 20)

project(parser_task)

find_package(Threads REQUIRED)

add_subdirectory(lib)
add_subdirectory(exec)
