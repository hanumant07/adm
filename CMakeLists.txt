cmake_minimum_required(VERSION 3.6.1)
OPTION(TARGET_ARM64 "BUILD FOR ARM64" OFF)
project (Graphs)
set(CMAKE_BUILD_TYPE DEBUG)


set(CMAKE_C_COMPILER "/usr/bin/clang")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_FLAGS "-std=c++14 -Wall -Werror -Wcast-align")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")

#set include directories
include_directories(include)

#set lib sources
file(GLOB LIB_SOURCES "src/*.cc")

#Generate library
add_library(graphs STATIC ${LIB_SOURCES})

#exe sources
file(GLOB EXE_SOURCES "testtool.c")

add_executable(graphtesttool "testtool.cc")
target_link_libraries(graphtesttool graphs)
