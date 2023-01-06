cmake_minimum_required(VERSION 3.11)
project(Bookstore)

set(CMAKE_CXX_STANDARD 14)

include_directories(${project_source_dir}/src/)

add_executable(code src/UnrolledLinkedList.cpp src/AccSystem.cpp src/LogSystem.cpp src/BookSystem.cpp src/main.cpp)
