cmake_minimum_required(VERSION 3.8)
project(Trees)

set(CMAKE_CXX_FLAGS "-std=c++17")
add_library(getCPUTime STATIC getCPUTime.cpp getCPUTime.hpp)

add_executable(tree main.cpp getCPUTime.hpp TreeBase.hpp AVLtree.hpp RBtree.hpp Profiler.hpp)

target_link_libraries(tree getCPUTime)
