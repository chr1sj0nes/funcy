cmake_minimum_required(VERSION 3.5)
project(funcy)

set(CMAKE_CXX_COMPILER "/network-raid/opt/clang/3.7.1-with-asan-centos6/bin/clang++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

include_directories(/network-raid/opt/clang/3.7.1-with-asan-centos6/include/c++/v1)
link_directories(/network-raid/opt/clang/3.7.1-with-asan-centos6/lib)
link_libraries(c++)

set(SOURCE_FILES main.cpp)
add_executable(funcy ${SOURCE_FILES})